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


struct Operator
{
	std::string UUID;
	std::string Name;
	uint8_t Level;
	bool BypassesPlayerLimit;

	Operator() = default;
};

inline void to_json(json& j, const Operator& o)
{
	j = json
	{
		{"uuid", o.UUID},
		{"name", o.Name},
		{"level", o.Level},
		{"bypassesPlayerLimit", o.BypassesPlayerLimit}
	};
}

inline void from_json(const json& j, Operator& o)
{
	j.at("uuid").get_to(o.UUID);
	j.at("name").get_to(o.Name);
	j.at("level").get_to(o.Level);
	j.at("bypassesPlayerLimit").get_to(o.BypassesPlayerLimit);
}

