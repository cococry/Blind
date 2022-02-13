#pragma once
/* Blind Engine Logging Header */

/* This header is a wrapper around the spdlog logging library https://github.com/gabime/spdlog
   which is used by Blind. The header includes logging macros and 
   a fully static logging class. Asserting macro is defined in the 
   Blind Core header.  */

#include <Blind/Core.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Blind
{
	class  Log
	{
		/* \Logging class is a spdlog wrapper class and its fully static. The main Init method needs to be called before 
		    anything is done with this wrapper. Utility macros for log levels are defined within this header. Use them 
			to log messages to the console:	BLIND_ENGINE_INFO("Example!"); */
	public:
		// Init method
		static void Init();

		// Getter methods
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_Client_logger; }
		inline static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_Engine_logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Engine_logger;
		static std::shared_ptr<spdlog::logger> s_Client_logger;
	};
}

// Utility macros for logging
#define BLIND_ENGINE_INFO(...) ::Blind::Log::GetEngineLogger()->info(__VA_ARGS__)
#define BLIND_ENGINE_TRACE(...) ::Blind::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define BLIND_ENGINE_WARN(...) ::Blind::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define BLIND_ENGINE_ERROR(...) ::Blind::Log::GetEngineLogger()->error(__VA_ARGS__)

#define BLIND_CLIENT_INFO(...) ::Blind::Log::GetClientLogger()->info(__VA_ARGS__)
#define BLIND_CLIENT_TRACE(...) ::Blind::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BLIND_CLIENT_WARN(...) ::Blind::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BLIND_CLIENT_ERROR(...) ::Blind::Log::GetClientLogger()->error(__VA_ARGS__)