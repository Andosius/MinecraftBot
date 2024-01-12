// Local dependencies
#include "Application.hpp"
#include "Log.hpp"

// External dependencies
#include <nlohmann/json.hpp>

// Standard Library
#include <fstream>
#include <filesystem>
//======================================

#include "Log.hpp"


static Application* s_Application = nullptr;


Application::Application(const std::string& config_file, CommandMap command_map)
	: m_ConfigFile(config_file)
{
	LoadConfigurationFile();
	Log::Init();

	m_Core = std::make_shared<Core>(m_BotToken, command_map);
	m_Core->SetRemoteAccessData(m_RemoteHostname, m_RemoteUsername, m_RemotePassword);
	m_Core->RegisterCallbacks();

	s_Application = this;
}

Application::~Application()
{

}

void Application::Run()
{
	m_Core->Run();
}

Application* Application::Get()
{
	return s_Application;
}

std::string Application::SendCommand(const std::string& command)
{
	std::string output;

	auto core = s_Application->GetCore();

	if (s_Application != nullptr && core != nullptr)
	{
		output = core->SendCommand(command);
	}

	return output;
}

void Application::ReloadConfig()
{
	LoadConfigurationFile();
}

void Application::LoadConfigurationFile()
{
	nlohmann::json config;
	

	std::ifstream config_file(m_ConfigFile, std::ios_base::app);

	std::string content;
	std::string line;
	while (std::getline(config_file, line))
		content += line;

	config = nlohmann::json::parse(content);

	m_BotToken = config["bot_token"].get<std::string>();
	m_RemoteHostname = config["remote_hostname"].get<std::string>();
	m_RemoteUsername = config["remote_username"].get<std::string>();
	m_RemotePassword = config["remote_password"].get<std::string>();
	m_RemoteScreenName = config["screen_name"].get<std::string>();
	m_RemoteScreenLocation = config["screen_loc"].get<std::string>();

	if (m_Core != nullptr)
		m_Core->SetRemoteAccessData(m_RemoteHostname, m_RemoteUsername, m_RemotePassword);
}
