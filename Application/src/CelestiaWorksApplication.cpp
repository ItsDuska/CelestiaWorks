#include <iostream>
#include <Windows.h>
#include <stdexcept>
#include "front/RenderFront.h"


#ifdef TIMING
#include <chrono>
#endif // TIMING

int main() {
	bool running = true;

	try
	{
		celestia::WindowHandle window({ 800,600 }, "Among Us Gaming");

		celestia::Color color = { 128,128,51,255 };

		window.setClearColor(color);

		while (running)
		{
			#ifdef TIMING
			std::chrono::time_point<std::chrono::system_clock> start, end;
			start = std::chrono::system_clock::now();
			#endif

			if (!window.isOpen())
			{
				running = false;
				std::cout << "CLOSING\n";
			}

			window.beginRenderPass();

			window.draw();

			window.endRenderPass();

			#ifdef TIMING
			//end = std::chrono::system_clock::now();
			//std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " microseconds\n";
			#endif
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