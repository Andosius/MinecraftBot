﻿#pragma once

// Local dependencies
#include "Operator.hpp"
#include "DiscordUser.hpp"
#include "WhitelistUser.hpp"

// External dependencies


// Standard Library
#include <string>

//======================================


namespace Utility
{
	 std::string SendCommand(const std::string& command);
	 void SendMinecraftCommand(const std::string& command);

	 bool IsServerRunning();
	 void ShutdownMinecraftServer();

	 std::vector<Operator> GetServerOperators();
	 std::vector<WhitelistUser> GetWhitelistedUsers();
	 std::vector<DiscordUser> GetKnownUsers();

	 DiscordUser GetKnownDiscordUserByUsername(const std::string& name);
	 DiscordUser GetKnownDiscordUserByAccountID(const uint64_t account_id);

	 bool IsValidMinecraftName(const std::string& name);
	 bool IsMinecraftPlayerWhitelisted(const std::string& name);

	 WhitelistUser GetMinecraftPlayerByUsername(const std::string& name);
	 WhitelistUser GetMinecraftPlayerByUUID(const std::string& uuid);

	 Operator GetMinecraftOperatorByUsername(const std::string& name);

	 void AddDiscordUserToCache(const DiscordUser& du);
	 void RemoveDiscordUserFromCache(const DiscordUser& du);
}