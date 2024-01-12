#pragma once

// Local dependencies
#include "command.hpp"


// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library
#include <cstdio>
#include <iostream>
#include <memory>
#include <array>
#include <string>
//======================================


class Core
{
public:
	Core(const std::string& token, CommandMap commands);
	~Core();

	void RegisterCallbacks();
	void SetRemoteAccessData(const std::string& host, const std::string& user, const std::string& pass);

	void Run();
	void Stop();

	std::string SendCommand(const std::string& command);

//private:
//	bool CreateRemoteConnection(ssh::Session& session);

private:
	dpp::cluster m_Bot;
	CommandMap m_Commands;

	std::string m_Hostname;
	std::string m_Username;
	std::string m_Password;
};