// Local dependencies
#include "Operator.hpp"

// External dependencies


// Standard Library


//======================================


void to_json(json& j, const Operator& o)
{
	j = json
	{
		{"uuid", o.UUID},
		{"name", o.Name},
		{"level", o.Level},
		{"bypassesPlayerLimit", o.BypassesPlayerLimit}
	};
}

void from_json(const json& j, Operator& o)
{
	j.at("uuid").get_to(o.UUID);
	j.at("name").get_to(o.Name);
	j.at("level").get_to(o.Level);
	j.at("bypassesPlayerLimit").get_to(o.BypassesPlayerLimit);
}
