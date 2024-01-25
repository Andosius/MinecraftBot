// Local dependencies
#include "WhitelistUser.hpp"

// External dependencies


// Standard Library


//======================================


void to_json(json& j, const WhitelistUser& wu)
{
	j = json
	{
		{"uuid", wu.UUID},
		{"name", wu.MinecraftName},
	};
}

void from_json(const json& j, WhitelistUser& wu)
{
	j.at("uuid").get_to(wu.UUID);
	j.at("name").get_to(wu.MinecraftName);
}

