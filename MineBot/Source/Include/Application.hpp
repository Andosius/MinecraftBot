#pragma once

// Local dependencies
#include "command.hpp"
#include "Core.hpp"

// External dependencies


// Standard Library
#include <string>
#include <mutex>
#include <memory>
//======================================


class Application
{
public:
	Application(const std::string& config_file, CommandMap command_map);

	Application(const Application&&) = delete;

	~Application();

	void Run();

	static Application* Get();
	std::shared_ptr<Core> GetCore() { return m_Core; }

	static std::string SendCommand(const std::string& command);

	void ReloadConfig();

private:
	void LoadConfigurationFile();

public:
	std::mutex m_AccountsMutex;
	std::mutex m_DatabaseMutex;

	std::string m_BotToken;
	std::string m_RemoteHostname;
	std::string m_RemoteUsername;
	std::string m_RemotePassword;
	std::string m_RemoteScreenName;
	std::string m_RemoteScreenLocation;

private:
	std::string m_ConfigFile;
	std::shared_ptr<Core> m_Core;
};