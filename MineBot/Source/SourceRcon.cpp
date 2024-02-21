/**
* Inspired by:
* https://github.com/Mare-02/RconCpp/blob/master/src/RconCpp.cpp
* https://github.com/Mare-02/RconCpp/blob/master/include/RconCpp.h
*/

// Local dependencies
#include "SourceRcon.hpp"
#include "Byteswap.hpp"

// External dependencies


// Standard Library
#include <chrono>
#include <iostream>

//======================================


constexpr std::chrono::duration TIMEOUT = std::chrono::seconds(2);


bool IsBigEndian()
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x1020304 };

	return bint.c[0] == 1;
}

bool IsLastMessage(const SourceRconPacket& pak)
{
	const char expectedBytes[] = { 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 };

	if (pak.Size != 8)
	{
		return false;
	}

	for (int i = 0; i < 8; i++)
	{
		if (pak.Command.data()[i] != expectedBytes[i])
		{
			return false;
		}
	}

	return true;
}



SourceRcon::SourceRcon(const std::string& hostname, const std::string& port, const std::string& password)
	: m_Hostname(hostname), m_Port(port), m_Password(password), m_Socket(m_Context), m_IsBigEndian(IsBigEndian()), m_Counter(1)
{
	m_Counter = 1;
}

SourceRcon::~SourceRcon()
{
	m_Socket.close();
}

bool SourceRcon::Connect()
{
	// Connect to host
	auto results = asio::ip::tcp::resolver(m_Context).resolve(m_Hostname, m_Port);

	// Check for errors while connecting
	asio::error_code error;
	asio::async_connect(m_Socket, results,
		[&](const asio::error_code& connect_error, const asio::ip::tcp::endpoint&)
		{
			error = connect_error;
		});

	asyncWait();

	if (error)
	{
		return false;
	}
	return true;
}

std::string SourceRcon::SendCommand(const std::string& command)
{
	std::string output = "";

	// Command packet
	SourceRconPacket request(m_Counter++, SourceRconMessageType::SERVERDATA_EXECCOMMAND, command);
	// Verification packet
	SourceRconPacket verification(request.Id, SourceRconMessageType::SERVERDATA_RESPONSE_VALUE , "");

	TransferPacket(request);
	TransferPacket(verification);

	// Get result
	SourceRconPacket result = ReceivePacket();
	do
	{
		output += result.Command;
		result = ReceivePacket();
	} while (!IsLastMessage(result));

	std::cout << "Transaction complete" << std::endl;

	return output;
}

void SourceRcon::asyncWait()
{
	m_Context.restart();

	m_Context.run_for(TIMEOUT);

	if (!m_Context.stopped())
	{
		m_Socket.close();
		m_Context.run();
	}
}

bool SourceRcon::Authenticate()
{
	SourceRconPacket request(m_Counter++, SourceRconMessageType::SERVERDATA_AUTH, m_Password);

	if (TransferPacket(request))
	{
		SourceRconPacket result = ReceivePacket();

		return result.Id != -1 && request.Id == result.Id;
	}
	return false;
}

bool SourceRcon::TransferPacket(SourceRconPacket& pak)
{
	// Create buffer - don't worry about potential data, we override it anyway.
	char* buffer = new char[pak.BufferSize];
	std::memset(buffer, '\0', pak.BufferSize);

	if (m_IsBigEndian)
	{
		pak.Size = static_cast<int32_t>(bswap_32(pak.Size));
		pak.Id = static_cast<int32_t>(bswap_32(pak.Id));
		pak.Type = static_cast<int32_t>(bswap_32(pak.Type));
	}

	// Push data to char* buffer
	std::memcpy(buffer + 0x0, &pak.Size, 4);
	std::memcpy(buffer + 0x4, &pak.Id, 4);
	std::memcpy(buffer + 0x8, &pak.Type, 4);
	std::memcpy(buffer + 0xC, pak.Command.data(), pak.Command.size());

	// Write to socket
	asio::error_code error;
	asio::async_write(m_Socket, asio::buffer(buffer, pak.BufferSize),
		[&](const asio::error_code transfer_error, std::size_t)
		{
			error = transfer_error;
		});

	// Wait for TIMEOUT (5 seconds)
	asyncWait();

	// Delete buffer
	delete[] buffer;

	// Check for errors
	if (error)
	{
		return false;
	}
	return true;
}

SourceRconPacket SourceRcon::ReceivePacket()
{
	// Create result field
	SourceRconPacket pak{};

	// Create max length buffer
	constexpr size_t buffer_size = 4096;
	char buffer[buffer_size] = {};

	// Get Size, Id, Type for now
	asio::error_code error;
	asio::async_read(m_Socket, asio::buffer(buffer, buffer_size), asio::transfer_exactly(12),
		[&](const asio::error_code transfer_error, std::size_t)
		{
			error = transfer_error;
		});

	asyncWait();

	// Return packetId of -1 in case something goes wrong
	if (error)
	{
		pak.Id = -1;
		return pak;
	}

	std::memcpy(&pak.Size, buffer + 0x0, 4);
	std::memcpy(&pak.Id, buffer + 0x4, 4);
	std::memcpy(&pak.Type, buffer + 0x8, 4);

	// Transform to big endian if needed
	if (m_IsBigEndian)
	{
		pak.Size = static_cast<int32_t>(bswap_32(pak.Size));
		pak.Id = static_cast<int32_t>(bswap_32(pak.Id));
		pak.Type = static_cast<int32_t>(bswap_32(pak.Type));
	}

	// Read command body
	int32_t body_len = pak.Size - 0x8; // 0x8 => Id and Type as Size is not included anyway!
	asio::async_read(m_Socket, asio::buffer(buffer + 0xC, body_len), asio::transfer_exactly(body_len),
		[&](const asio::error_code transfer_error, std::size_t)
		{
			error = transfer_error;
		});

	asyncWait();

	// In case something goes wrong, return packet without result
	if (error)
	{
		return pak;
	}

	// Set result and return packet
	pak.Command = std::string(buffer + 0xC, pak.Size);

	return pak;
}
