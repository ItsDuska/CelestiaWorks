#include <iostream>
#include <Windows.h>
#include <stdexcept>
#include "front/RenderFront.h"


int main() {
	bool running = true;

	try
	{
		celestia::WindowHandle window({ 800,600 }, "Among Us Gaming");

		while (running)
		{
			if (!window.isOpen())
			{
				running = false;
				std::cout << "CLOSING\n";
			}

			window.beginRenderPass();

			window.draw();

			window.endRenderPass();

			Sleep(10);
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

/*
	Miten vois toimia:

	celestia::WindowHandle window(widht,height,name);

	celestia::Texture texture;
	if (!texture.loadTexture(imagePath))
	{
		std::cout << "ERROR LOADING TEXTURE";
	}

	celestia::RenderObject player(width,height,pos,color,texture);

	celestia::Event event;

	while (window.isOnline())
	{
		window.pollEvent(event);
		if (event.type == celestia::Event::Shutdown)
		{
			window.close();
		}

		window.clear(celestia::Color::BLACK);
		window.beginRendering();

		window.draw(player);

		window.endRendering();
	}

	
*/