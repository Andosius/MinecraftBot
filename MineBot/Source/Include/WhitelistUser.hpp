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


struct WhitelistUser
{
	std::string UUID;
	std::string MinecraftName;

	WhitelistUser() = default;

	bool IsValid() const { return UUID != ""; }

	bool operator==(WhitelistUser& u2)
	{
		return this->UUID == u2.UUID && this->MinecraftName == u2.MinecraftName;
	}
};

void to_json(json& j, const WhitelistUser& wu);
void from_json(const json& j, WhitelistUser& wu);
