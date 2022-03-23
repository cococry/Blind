#pragma once

#include <string>

namespace Blind
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, const char* dir = "assets");
		static std::string SaveFile(const char* filter);
	};
}