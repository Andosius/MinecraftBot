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

	bool IsValid() const { return UUID != ""; }

	bool operator==(Operator& u2)
	{
		return this->UUID == u2.UUID && this->Name == u2.Name && this->Level == u2.Level && this->BypassesPlayerLimit == u2.BypassesPlayerLimit;
	}
};

void to_json(json& j, const Operator& o);
void from_json(const json& j, Operator& o);
