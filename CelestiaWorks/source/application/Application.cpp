#include "Application.h"

celestia::Application::Application()
{
}

celestia::Application::~Application()
{
}

// main loop
void celestia::Application::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();
		// do stuff

		//update game logic
		
		//render

		
	}
}
