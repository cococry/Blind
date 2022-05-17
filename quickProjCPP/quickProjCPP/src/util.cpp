#include "util.h"

#include <iostream>

void ConsoleUtil::WriteTitle(char c, const char* title)
{
	for (uint32_t i = 0; i < 25; i++) 
	{
		std::cout << c;
	}
	std::cout << title;
	for (uint32_t i = 0; i < 25; i++)
	{
		std::cout << c;
	}
	std::cout << "\n";
}

void ConsoleUtil::WriteCharNTimes(char c, uint32_t n)
{
	for (uint32_t i = 0; i < n; i++)
	{
		std::cout << c;
	}
	std::cout << "\n";
}
