#pragma once

// Local dependencies
#include "Application.hpp"
#include "Operator.hpp"
#include "DiscordUser.hpp"
#include "WhitelistUser.hpp"

// External dependencies
#pragma warning(push, 0)
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#pragma warning(pop)

// Standard Library
#include <regex>
#include <string>

//======================================


class Utility
{
public:
	static std::string SendCommand(const std::string& command);
	static void SendMinecraftCommand(const std::string& command);

	static bool IsServerRunning();

	static std::vector<Operator> GetServerOperators();
	static std::vector<WhitelistUser> GetWhitelistedUsers();
	static std::vector<DiscordUser> GetKnownUsers();

	static DiscordUser GetKnownDiscordUserByUsername(const std::string& name);
	static DiscordUser GetKnownDiscordUserByAccountID(const uint64_t account_id);

	static bool IsValidMinecraftName(const std::string& name);
	static bool IsMinecraftPlayerWhitelisted(const std::string& name);

	static WhitelistUser GetMinecraftPlayerByUsername(const std::string& name);
	static WhitelistUser GetMinecraftPlayerByUUID(const std::string& uuid);

	static Operator GetMinecraftOperatorByUsername(const std::string& name);

	static void AddDiscordUserToCache(const DiscordUser du);
	static void RemoveDiscordUserFromCache(const DiscordUser du);
};
