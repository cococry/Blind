#pragma once
#include <Blind/Core/Core.h>

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
	BL_PROFILE_BEGIN_SESSION("Startup", "BlindProfile-Startup.json");
	auto app = Blind::CreateApplication();
	BL_PROFILE_END_SESSION();

	BL_PROFILE_BEGIN_SESSION("Runtime", "BlindProfile-Runtime.json");
	app->Run();
	BL_PROFILE_END_SESSION();

	BL_PROFILE_BEGIN_SESSION("Shutdown", "BlindProfile-Shutdown.json");
	delete app;
	BL_PROFILE_END_SESSION();
}
#endif