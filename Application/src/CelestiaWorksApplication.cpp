#include <iostream>
#include <vector>
#include <stdexcept>

#include <Windows.h>

#include "frontend/window/RenderFront.h"
#include "frontend/sprite/Sprite.h"



int main() {
	bool running = true;

	try
	{
		celestia::WindowHandle window({ 800,800 }, "Among Us Gaming");

		celestia::Color color = { 128,128,51,255 };

		window.setClearColor(color);

		celestia::Texture textures[2];
		textures[0].loadTexture("../assets/test.png");
		textures[1].loadTexture("../assets/TempAsset1.jpg");
		
		std::vector<celestia::Sprite> sprites;
		
		float tempX = 800.f / 5;
		float tempY = 800.f / 5;

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				celestia::Vec2 position = { j * tempX, i * tempY };
				celestia::Vec2 size = { 125.f,125.f };

				celestia::Sprite sprite(position, size);
				sprite.setTexture(textures[j % 2]);
				
				sprites.push_back(sprite);
			}
		}

		while (running)
		{
			if (!window.isOpen())
			{
				running = false;
				std::cout << "CLOSING\n";
			}

			window.beginRenderPass();

			for (celestia::Sprite& currentSprite : sprites)
			{
				window.draw(currentSprite);
			}

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