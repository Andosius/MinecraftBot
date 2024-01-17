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