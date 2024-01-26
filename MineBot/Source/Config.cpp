// Local dependencies
#include "Config.hpp"
#include "Log.hpp"

// External dependencies


// Standard Library
#include <memory>
#include <fstream>

//======================================


std::shared_ptr<Config> AppSettings;


void Config::LoadConfigurationFile()
{
	try
	{
		std::ifstream config_file("config.json", std::ios_base::app);
		std::string json_string(std::istreambuf_iterator<char>{config_file}, {});

		json data = nlohmann::json::parse(json_string);

		AppSettings = std::make_shared<Config>(data.template get<Config>());
	}
	catch (std::exception& e)
	{
		LOG_ERROR(e.what());
	}
}

void to_json(json& j, const SSHConfig& s)
{
	j = json
	{
		{"Hostname", s.Hostname},
		{"Username", s.Username},
		{"Password", s.Password}
	};
}

void from_json(const json& j, SSHConfig& s)
{
	j.at("Hostname").get_to(s.Hostname);
	j.at("Username").get_to(s.Username);
	j.at("Password").get_to(s.Password);
}

void to_json(json& j, const RCONConfig& r)
{
	j = json
	{
		{"Hostname", r.Hostname},
		{"Port", r.Port},
		{"Password", r.Password}
	};
}

void from_json(const json& j, RCONConfig& r)
{
	j.at("Hostname").get_to(r.Hostname);
	j.at("Port").get_to(r.Port);
	j.at("Password").get_to(r.Password);
}

void to_json(json& j, const Config& c)
{
	j = json
	{
		{"BotToken", c.Token},
		{"Directory", c.Directory},
		{"ScreenName", c.ScreenSessionName},
		{"SSH", c.SSH},
	};

}

void from_json(const json& j, Config& c)
{
	j.at("BotToken").get_to(c.Token);
	j.at("Directory").get_to(c.Directory);
	j.at("ScreenSessionName").get_to(c.ScreenSessionName);
	j.at("SSH").get_to(c.SSH);
}