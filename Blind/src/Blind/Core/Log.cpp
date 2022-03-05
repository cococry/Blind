#include <blindpch.h>
#include "Log.h"

namespace Blind
{
	// Translation unit static class declarations 
	std::shared_ptr<spdlog::logger> Log::s_Client_logger;
	std::shared_ptr<spdlog::logger> Log::s_Engine_logger;

	void Log::Init()
	{
		/* \Setting the pattern in which spdlog loggers will log
		   messages. Custom Formating information: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		   The Init method initializes the Engine- and Client side loggers colors and formating.*/

		spdlog::set_pattern("%^[%T] %n: %v%$");
		 
		s_Engine_logger = spdlog::stdout_color_mt("BLIND");
		s_Engine_logger->set_level(spdlog::level::trace);

		s_Client_logger = spdlog::stdout_color_mt("CLIENT");
		s_Client_logger->set_level(spdlog::level::trace);
	}
}