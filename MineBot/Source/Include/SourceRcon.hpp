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
	int32_t Size;
	int32_t Id;
	int32_t Type;
	std::string Command;
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

#ifdef _MSC_VER

#include <stdlib.h>
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else

#include <byteswap.h>

#endif