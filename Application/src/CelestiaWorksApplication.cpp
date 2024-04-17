#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include "CelestiaWorks/Graphics/Text.h"
#include "CelestiaWorks/Graphics/WindowHandle.h"
#include "CelestiaWorks/Graphics/Sprite.h"
#include "CelestiaWorks/System/Keyboard.h"


int main()
{
	try
	{
		celestia::WindowHandle window({ 800,800 }, "Among Us Gaming");
		window.setFrameRateLimit(60);

		celestia::Font font;
		if (!font.loadFont("../assets/yoster.ttf", 32))
		{
			std::cout << "FAILED TO LOAD FONT FROM MAIN!\n";
		}
		// { 220,232,224,255 }
		std::string stringText = "Tuukka my beloved";
		celestia::Text text({ 100.f,400.f }, stringText, { 255,15,1,255 }, &font);

		celestia::Color color = { 128,128,51,255 };

		window.setClearColor(color);

		celestia::Texture textures[3];
		textures[0].loadTexture("../assets/test.png");
		textures[1].loadTexture("../assets/TempAsset1.jpg");
		textures[2].loadTexture("../assets/RatSpriteSheet.png");
		
		std::vector<celestia::Sprite> sprites;
		
		float tempX = 800.f / 5;
		float tempY = 800.f / 5;

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				celestia::Vec2 position = { j * tempX, i * tempY };
				celestia::Vec2 size = { 155.f,155.f };

				celestia::Sprite sprite(position, size);
				sprite.setTexture(textures[j % 2]);
				
				sprites.push_back(sprite);
			}
		}

		celestia::Sprite testSprite({ 200.f,600.f }, { 50.f,50.f });
		testSprite.setTexture(textures[2]);
		testSprite.setTextureRectSize({ 32.f,32.f });

		int currentTime = 0;

		const int updateFrameNumber = 8;
		int rectPositionX = 0;
		
		while (window.isOpen())
		{
			currentTime++;

			if (celestia::Keyboard::isKeyPressed(celestia::Keyboard::Key::A))
			{
				std::cout << "A PRESSED OMG\n";
			}


			window.beginRenderPass();

			
			for (celestia::Sprite& currentSprite : sprites)
			{
				window.draw(currentSprite);
			}
			

			if (currentTime >= updateFrameNumber)
			{
				rectPositionX++;
				rectPositionX %= 18;
				currentTime = 0;
				testSprite.setTextureRectPosition({ rectPositionX,0 });
			}

			window.draw(testSprite);
			window.draw(text);

			window.endRenderPass();

			//Sleep(10);
		}
		
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout << "CLOSING\n";

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


/*

TODO: TEXT RENDERING!
TEE TAPA LUODA STORAGE BUFFEREITA JA SÄILÖ NE ESIM RENDERI HOMMASSA TAI JOSSAIN MUUALLA.

*/