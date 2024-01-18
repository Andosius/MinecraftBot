#pragma once

// Local dependencies


// External dependencies
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

// Standard Library
#include <memory>
//======================================


// Code from https://github.com/TheCherno/Hazel/


template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}


class Log
{
public:
	static void Init();

	static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

private:
	static Ref<spdlog::logger> s_CoreLogger;

};

#define LOG_TRACE(...)    ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    ::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)     ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__)
