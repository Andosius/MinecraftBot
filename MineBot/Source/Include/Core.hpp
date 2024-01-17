#pragma once

// Local dependencies
#include "Config.hpp"

// External dependencies
#pragma warning(push, 0)
#include <dpp/dpp.h>
#pragma warning(pop)

// Standard Library
#include <memory>

//======================================


class Core
{
public:
	Core();
	~Core();

	void RegisterCallbacks();

	void Run();
	void Stop();

private:
	dpp::cluster m_Bot;
};