#pragma once

// Local dependencies


// External dependencies
#include <nlohmann/json.hpp>
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

	bool IsValid() { return DiscordId != 0; }
	bool operator==(DiscordUser& u2)
	{
		return this->DiscordId == u2.DiscordId && this->MinecraftName == u2.MinecraftName;
	}
};

inline void to_json(json& j, const DiscordUser& du)
{
	j = json
	{
		{"account_id", du.DiscordId},
		{"name", du.MinecraftName},
	};
}

inline void from_json(const json& j, DiscordUser& du)
{
	j.at("account_id").get_to(du.DiscordId);
	j.at("name").get_to(du.MinecraftName);
}

