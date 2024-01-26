#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <nlohmann/json.hpp>
#pragma warning(pop)
using json = nlohmann::json;

// Standard Library
#include <memory>
#include <string>

//======================================


struct SSHConfig
{
	std::string Hostname;
	std::string Username;
	std::string Password;
};

struct RCONConfig
{
	std::string Hostname;
	std::string Port;
	std::string Password;
};

struct Config
{
	std::string Token;
	std::string Directory;
	std::string ScreenSessionName;
	SSHConfig SSH;
	RCONConfig RCON;

	Config() = default;

	static void LoadConfigurationFile();
};


void to_json(json& j, const SSHConfig& s);
void from_json(const json& j, SSHConfig& s);

void to_json(json& j, const RCONConfig& r);
void from_json(const json& j, RCONConfig& r);

void to_json(json& j, const Config& c);
void from_json(const json& j, Config& c);


extern std::shared_ptr<Config> AppSettings;