/**
* Inspired by:
* https://github.com/Mare-02/RconCpp/blob/master/src/RconCpp.cpp
* https://github.com/Mare-02/RconCpp/blob/master/include/RconCpp.h
*/

// Local dependencies
#include "SourceRcon.hpp"
#include "Log.hpp"

// External dependencies


// Standard Library
#include <chrono>
#include <stdlib.h>

//======================================


constexpr std::chrono::duration TIMEOUT = std::chrono::seconds(5);


bool IsBigEndian()
{
	union {
		uint32_t i;
		char c[4];
	} bint = { 0x1020304 };

	return bint.c[0] == 1;
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

	// Send command
	SourceRconPacket request{};
	request.Type = SourceRconMessageType::SERVERDATA_EXECCOMMAND;
	request.Command = command;
	TransferPacket(request);

	// Get result
	SourceRconPacket result{};
	do
	{
		result = ReceivePacket();
		output += result.Command;
	} while (result.Size >= (4096 - 14)); // Max packet - min packet size

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
	SourceRconPacket request{};
	request.Type = SourceRconMessageType::SERVERDATA_AUTH;
	request.Command = m_Password;
	if (TransferPacket(request))
	{
		SourceRconPacket result = ReceivePacket();

		return result.Id != -1 && request.Id == result.Id;
	}
	return false;
}

bool SourceRcon::TransferPacket(SourceRconPacket& pak)
{
	// 14 Bytes by default:
	// 4 => Size
	// 4 => Id
	// 4 => Type
	// 1 => body end
	// 1 => Command end
	int32_t buffer_size = 14 + static_cast<int32_t>(pak.Command.size());

	// Create buffer and make sure everything is \0 before we start writing to it!
	char* buffer = new char[buffer_size];
	std::memset(buffer, '\0', buffer_size);

	// This is buffer_size subtracted by the already read size
	pak.Size = buffer_size - 4;

	// Set Id and increment counter
	pak.Id = m_Counter++;

	// In case counter is > 1337 => reset it to 1
	if (m_Counter > 1337)
	{
		m_Counter = 1;
	}

	// Change from big to little endian if needed
	if (m_IsBigEndian)
	{
		pak.Size = static_cast<int32_t>(_byteswap_ulong(pak.Size));
		pak.Id = static_cast<int32_t>(_byteswap_ulong(pak.Id));
		pak.Type = static_cast<int32_t>(_byteswap_ulong(pak.Type));
	}

	// Push data to char* buffer
#pragma warning(push, 0)
	std::memcpy(buffer + 0x0, &pak.Size, 4);
	std::memcpy(buffer + 0x4, &pak.Id, 4);
	std::memcpy(buffer + 0x8, &pak.Type, 4);
	std::memcpy(buffer + 0xC, pak.Command.data(), pak.Command.size());
#pragma warning(pop)

	// Write to socket
	asio::error_code error;

	asio::async_write(m_Socket, asio::buffer(buffer, buffer_size),
		[&](const asio::error_code transfer_error, std::size_t)
		{
			error = transfer_error;
		});

	// Wait for TIMEOUT (5 seconds)
	asyncWait();

	// Check for errors
	if (error)
	{
		delete[] buffer;
		return false;
	}

	delete[] buffer;
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
		pak.Size = static_cast<int32_t>(_byteswap_ulong(pak.Size));
		pak.Id = static_cast<int32_t>(_byteswap_ulong(pak.Id));
		pak.Type = static_cast<int32_t>(_byteswap_ulong(pak.Type));
	}

	// Read command body
	int32_t body_len = pak.Size - 0x8; // 0x8 => Id and Type as Size is not included anyway!
	asio::async_read(m_Socket, asio::buffer(buffer + 0xC, body_len), asio::transfer_exactly(body_len),
		[&](const asio::error_code transfer_error, std::size_t)
		{
			error = transfer_error;
		});

	asyncWait();

	// In case something goes wrong, return packet without command
	if (error)
	{
		return pak;
	}

	// Set command and return packet
	pak.Command = std::string(buffer + 0xC);

	return pak;
}
