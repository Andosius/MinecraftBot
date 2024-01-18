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


struct SSH
{
	std::string Hostname;
	std::string Username;
	std::string Password;
};

inline void to_json(json& j, const SSH& s)
{
	j = json
	{ 
		{"Hostname", s.Hostname}, 
		{"Username", s.Username}, 
		{"Password", s.Password}
	};
}

inline void from_json(const json& j, SSH& s)
{
	j.at("Hostname").get_to(s.Hostname);
	j.at("Username").get_to(s.Username);
	j.at("Password").get_to(s.Password);
}


struct Config
{
	std::string Token;
	std::string Directory;
	std::string ScreenSessionName;
	SSH SSH;

	Config() = default;

	static void LoadConfigurationFile();
};


inline void to_json(json& j, const Config& c)
{
	j = json
	{ 
		{"BotToken", c.Token}, 
		{"Directory", c.Directory},
		{"ScreenName", c.ScreenSessionName},
		{"SSH", c.SSH},
	};

}

inline void from_json(const json& j, Config& c)
{
	j.at("BotToken").get_to(c.Token);
	j.at("Directory").get_to(c.Directory);
	j.at("ScreenSessionName").get_to(c.ScreenSessionName);
	j.at("SSH").get_to(c.SSH);
}

inline void from_json(const json& j, Config* c)
{
	j.at("BotToken").get_to(c->Token);
	j.at("Directory").get_to(c->Directory);
	j.at("ScreenSessionName").get_to(c->ScreenSessionName);
	j.at("SSH").get_to(c->SSH);
}


extern std::shared_ptr<Config> AppSettings;