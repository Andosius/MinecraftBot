#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <nlohmann/json.hpp>
#pragma warning(pop)
using json = nlohmann::json;

// Standard Library
#include <string>

//======================================


struct DiscordUser
{
	uint64_t DiscordId;
	std::string MinecraftName;

	DiscordUser() : DiscordId(0) {};
	DiscordUser(uint64_t account_id, std::string name) : DiscordId(account_id), MinecraftName(name) {};

	bool IsValid() const { return DiscordId != 0; }
	bool operator==(DiscordUser& u2)
	{
		return this->DiscordId == u2.DiscordId && this->MinecraftName == u2.MinecraftName;
	}
};

void to_json(json& j, const DiscordUser& du);
void from_json(const json& j, DiscordUser& du);
