/**
* Inspired by:
* https://github.com/Mare-02/RconCpp/blob/master/src/RconCpp.cpp
* https://github.com/Mare-02/RconCpp/blob/master/include/RconCpp.h
*/

#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <asio.hpp>
#pragma warning(pop)

// Standard Library
#include <stdint.h>
#include <string>

//======================================


#define MAX_RCON_MESSAGE_BODY		4096
#define INVALID_AUTH				-1


enum SourceRconMessageType
{
	/**
	* @brief Contains the rcon password as c string.
	*/
	SERVERDATA_AUTH = 3,

	/**
	* @brief Contains the command to be executed.
	*/
	SERVERDATA_EXECCOMMAND = 2,

	/**
	* @brief Contains the SERVERDATA_AUTH package ID as ID, otherwise -1 (0xFFFFFFFF).
	*/
	SERVERDATA_AUTH_RESPONSE = 2,

	/**
	* @brief Contains the ID of the original message as ID and may have an empty response body.
	*/
	SERVERDATA_RESPONSE_VALUE = 0
};

struct SourceRconPacket
{
	int32_t Size = -1;
	int32_t Id = -1;
	int32_t Type = -1;
	std::string Command;

	int32_t BufferSize = -1;

	SourceRconPacket() = default;
	SourceRconPacket(const int32_t id, const int32_t type, const std::string& command)
	{
		Id = id;
		Type = type;
		Command = command;

		// 14 = 4 Size, 4 Id, 4 Type, 1 Body-'\0', 1 Packet-'\0'
		BufferSize = 14 + static_cast<int32_t>(Command.size());

		// Length of everything except Size itself
		Size = BufferSize - 4;
	}
};


class SourceRcon
{
public:
	SourceRcon() = delete;
	SourceRcon(const std::string& hostname, const std::string& port, const std::string& password);
	~SourceRcon();

	bool Authenticate();
	bool Connect();
	std::string SendCommand(const std::string& command);

private:
	bool TransferPacket(SourceRconPacket& pak);
	SourceRconPacket ReceivePacket();

	void asyncWait();

private:
	std::string m_Hostname;
	std::string m_Port;
	std::string m_Password;

	bool m_IsBigEndian;

	asio::io_context m_Context;
	asio::ip::tcp::socket m_Socket;
	int32_t m_Counter;
};
