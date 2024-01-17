// Local dependencies
#include "Config.hpp"

// External dependencies


// Standard Library
#include <memory>
#include <fstream>

//======================================


std::shared_ptr<Config> AppSettings = std::make_shared<Config>();


void Config::LoadConfigurationFile()
{
	try
	{
		std::ifstream config_file("config.json", std::ios_base::app);
		std::string json_string(std::istreambuf_iterator<char>{config_file}, {});

		nlohmann::json data = nlohmann::json::parse(json_string);
		from_json(data, AppSettings.get());
	}
	catch (std::exception& e)
	{
	}
}