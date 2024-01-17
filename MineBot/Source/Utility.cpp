// Local dependencies
#include "Utility.hpp"
#include "Config.hpp"
#include "Log.hpp"

// External dependencies
#define LIBSSH_STATIC 1
#define SSH_NO_CPP_EXCEPTIONS
#include <libssh/libsshpp.hpp>

// Standard Library
#include <vector>
#include <fstream>

//======================================

std::string Utility::SendCommand(const std::string& command)
{
	ssh::Session session;
	std::string output;

	session.setOption(SSH_OPTIONS_PASSWORD_AUTH, 1);
	session.setOption(SSH_OPTIONS_HOST, AppSettings->SSH.Hostname.c_str());
	session.setOption(SSH_OPTIONS_USER, AppSettings->SSH.Username.c_str());
	session.setOption(SSH_OPTIONS_PORT, 22);

	int err = session.connect();

	if (err == SSH_ERROR)
	{
		LOG_CRITICAL("SSH Error, can't create connection!");
		return "";
	}

	err = session.userauthPassword(AppSettings->SSH.Password.c_str());

	if (err == SSH_ERROR)
	{
		LOG_CRITICAL("SSH Error, can't authenticate with password!");
		return "";
	}

	if (err == SSH_AUTH_SUCCESS)
	{
		ssh::Channel channel(session);
		channel.openSession();

		channel.requestExec(command.c_str());

		char buffer[1024] = {};
		int nbytes = 0;

		while (!channel.isEof())
		{
			nbytes = channel.read(&buffer, sizeof(buffer), false, 1);
			if (nbytes > 0)
				output.append(buffer, nbytes);
		}

		channel.sendEof();
		channel.close();
	}
	else
	{
		LOG_CRITICAL("Unable to create connection!");
	}

	return output;
}

void Utility::SendMinecraftCommand(const std::string& command)
{
	SendCommand(fmt::format("/usr/bin/screen -p 0 -S {0} -X eval 'stuff \"{1}\"\\015'", AppSettings->ScreenSessionName, command));
}

bool Utility::IsServerRunning()
{
	std::string result = SendCommand(fmt::format("screen -ls | grep \"{}\"", AppSettings->ScreenSessionName));

	if (result == "")
		return false;

	return true;
}

std::vector<Operator> Utility::GetServerOperators()
{
	std::string content = SendCommand(fmt::format("cat {0}/ops.json", AppSettings->Directory));
	json j = json::parse(content);

	return j.template get<std::vector<Operator>>();
}

std::vector<WhitelistUser> Utility::GetWhitelistedUsers()
{
	std::string content = SendCommand(fmt::format("cat {0}/whitelist.json", AppSettings->Directory));
	json j = json::parse(content);

	return j.template get<std::vector<WhitelistUser>>();
}

std::vector<DiscordUser> Utility::GetKnownUsers()
{
	std::ifstream accounts("accounts.json", std::ios_base::app);
	std::string content(std::istreambuf_iterator<char>{accounts}, {});

	json j = json::parse(content);

	return j.template get<std::vector<DiscordUser>>();
}

bool Utility::IsValidMinecraftName(const std::string& name)
{
	// Check for name length (3-16 chars)
	if (name.length() < 3 || name.length() > 16) {
		return false;
	}

	// A-Z, a-z, 0-9 and _ are allowed
	std::regex regex("[A-Za-z0-9_]+");
	if (!std::regex_match(name, regex)) {
		return false;
	}

	return true;
}

bool Utility::IsMinecraftPlayerWhitelisted(const std::string& name)
{
	std::vector< WhitelistUser> users = GetWhitelistedUsers();
	for(WhitelistUser& user : users)
	{
		if (user.MinecraftName == name)
			return true;
	}
	return false;
}

DiscordUser Utility::GetKnownDiscordUserByUsername(const std::string& name)
{
	std::vector<DiscordUser> users = GetKnownUsers();

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].MinecraftName == name)
			return users[i];
	}
	return DiscordUser();
}

DiscordUser Utility::GetKnownDiscordUserByAccountID(const uint64_t account_id)
{
	std::vector<DiscordUser> users = GetKnownUsers();

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].DiscordId == account_id)
			return users[i];
	}
	return DiscordUser();
}

WhitelistUser Utility::GetMinecraftPlayerByUsername(const std::string& name)
{
	std::vector<WhitelistUser> users = GetWhitelistedUsers();

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].MinecraftName == name)
			return users[i];
	}
	return WhitelistUser();
}

WhitelistUser Utility::GetMinecraftPlayerByUUID(const std::string& uuid)
{
	std::vector<WhitelistUser> users = GetWhitelistedUsers();

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].UUID == uuid)
			return users[i];
	}
	return WhitelistUser();
}

void Utility::AddDiscordUserToCache(const DiscordUser du)
{
	std::vector<DiscordUser> users = GetKnownUsers();
	users.push_back(du);

	std::ofstream accounts("accounts.json", std::ios_base::app);

	json j = users;
	std::string data = j.dump();

	accounts.write(data.c_str(), data.size());
}

void Utility::RemoveDiscordUserFromCache(const DiscordUser du)
{
	std::vector<DiscordUser> users = GetKnownUsers();

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].MinecraftName == du.MinecraftName)
		{
			users.erase(users.begin() + i);
			break;
		}
	}

	std::ofstream accounts("accounts.json", std::ios_base::app);

	json j = users;
	std::string data = j.dump();

	accounts.write(data.c_str(), data.size());
}

/*
#include <cstdio>
#include <iostream>
#include <memory>
#include <array>
#include <string>

#ifdef _WIN64
#define pclose	_pclose
#define popen	_popen
#endif

std::string Core::SendCommand(const std::string& command)
{
	std::array<char, 128> buffer;
	std::string result;

	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

	if (!pipe)
	{
		LOG_CRITICAL("popen() failed! Message: {}", command);
		return "";
	}

	while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}

	return result;
}
*/
