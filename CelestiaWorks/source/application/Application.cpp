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
	TransformComponent cameraObject{};

	auto currentTime = std::chrono::high_resolution_clock::now();
	while (!window.shouldClose())
	{
		glfwPollEvents();
		// do stuff
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime =
			std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		keyboard.move(window.getGLFWwindow(), frameTime, cameraObject);
		camera.setViewYXZ(cameraObject.translation, cameraObject.rotation);

		//update game logic
		
		//render
		render.drawObjects(scene.getRenderObjects(), scene.getRenderObjectSizes(),camera);
		
	}
	vkDeviceWaitIdle(device.getDevice());
}
