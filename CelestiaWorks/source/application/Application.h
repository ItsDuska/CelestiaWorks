#pragma once
#include "window/Window.h"
#include "vulkan/Device.h"
#include "vulkan/SwapChain.h"
#include "vulkan/Pipeline.h"
#include "vulkan/Render.h"
#include "vulkan/Buffer.h"
#include "vulkan/Scene.h"
#include "vulkan/Descriptor.h"
//#include "Keyboard/Keyboard.h"
#include "gameObjects/Player.h"

namespace celestia
{
	class Application
	{
	public:
		Application();
		~Application();
		void run();

	private:
		Player player;
		Window window{ 800,600,"CelestiaWorks" };
		Device device{window};
		SwapChain swapChain{device};
		Buffer buffer{ device };
		Descriptor descriptor{ device, buffer };
		Pipeline pipeline{ device,swapChain, descriptor };
		Scene scene{ buffer, pipeline };
		Render render{ device,swapChain, descriptor};



	};
	
}



/*
Mit‰ pit‰isi olla:

Application class:
	member vars:
		-main loop
		-window
		-renderer (class joka sis‰lt‰‰ kaiken rendering hommat eli vulkan hommat)
		-logic (class joka hoitaa pelin hommat?)
		-delta time?

	functions:
		-run()
		-render?
		-update?

Renderer class:
	member vars:
		-vulkan shartit
		-device
		-swapchain
		-pipeline
		...


TODO YLEINEN:
-classien muokkaaminen 

- OMA CLASS RENDER_OBJECTIEN LUOMISEEN


TODO VULKAN OSAT:
UNIFORM BUFFERS
TEXTURES??
*/