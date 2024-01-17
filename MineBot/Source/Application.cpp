// Local dependencies
#include "Application.hpp"
#include "Core.hpp"
#include "Config.hpp"
#include "Log.hpp"

// External dependencies


// Standard Library
#include <memory>

//======================================


void Application::Run()
{
	Log::Init();

	Config::LoadConfigurationFile();
	Core core = Core();

	try
	{
		core.Run();
	}
	catch (std::exception&)
	{
		core.Stop();
	}
}
