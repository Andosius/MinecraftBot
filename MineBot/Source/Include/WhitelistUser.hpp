#pragma once

// Local dependencies


// External dependencies
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Standard Library
#include <string>

//======================================


struct WhitelistUser
{
	std::string UUID;
	std::string MinecraftName;

	WhitelistUser() = default;

	inline bool IsValid() { return UUID != ""; }

	bool operator==(WhitelistUser& u2)
	{
		return this->UUID == u2.UUID && this->MinecraftName == u2.MinecraftName;
	}
};

inline void to_json(json& j, const WhitelistUser& wu)
{
	j = json
	{
		{"uuid", wu.UUID},
		{"name", wu.MinecraftName},
	};
}

inline void from_json(const json& j, WhitelistUser& wu)
{
	j.at("uuid").get_to(wu.UUID);
	j.at("name").get_to(wu.MinecraftName);
}

