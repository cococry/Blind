#pragma once

#include <stdint.h>

class ConsoleUtil
{
public:
	static void WriteTitle(char c, const char* title);

	static void WriteCharNTimes(char c, uint32_t n);
private:
};