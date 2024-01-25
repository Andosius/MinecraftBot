// Local dependencies
#include "DiscordUser.hpp"

// External dependencies


// Standard Library


//======================================


void to_json(json& j, const DiscordUser& du)
{
	j = json
	{
		{"account_id", du.DiscordId},
		{"name", du.MinecraftName},
	};
}

void from_json(const json& j, DiscordUser& du)
{
	j.at("account_id").get_to(du.DiscordId);
	j.at("name").get_to(du.MinecraftName);
}
