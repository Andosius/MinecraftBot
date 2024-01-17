#pragma once

// Local dependencies



// External dependencies


// Standard Library


//======================================


class Application
{
public:
	Application() = delete;
	Application(const Application&&) = delete;
	~Application() = default;

	static void Run();
};