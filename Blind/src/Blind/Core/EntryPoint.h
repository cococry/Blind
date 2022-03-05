#pragma once
#include <Blind/Core/Log.h>

/* Blind Engine Entry Point Header */

/* This header defines the main function and with that, it is the entry point to the engine.
   It creates a Blind application that is declared engine side and implemented engine side.*/

// 'CreateApplication' is declared engine side and defined client side
extern Blind::Application* Blind::CreateApplication(); // Getting the extern function CreateApplication()

// Only run the entry point if Blind is running on windows.
#ifdef BLIND_PLATFORM_WINDOWS
int main(int argc, char** argv) // Win-Main
{
	// Instantiating the client application when program starts
	Blind::Log::Init();
	auto app = Blind::CreateApplication();
	app->Run();
	delete app;
}
#endif