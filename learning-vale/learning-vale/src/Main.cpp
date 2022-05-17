#include <iostream>
#include "Math.h"
int main()
{
	Game::Application application;
	Game::Window window(1920, 1080, "Nuke Engine");

	application.SetWindow(window);
	window.currentState = Game::Window::WindowState::Opened;
	application.GetWindow().Print();
	application.Run();
}