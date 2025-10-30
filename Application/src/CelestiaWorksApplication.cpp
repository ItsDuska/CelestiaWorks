#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include "CelestiaWorks/Graphics/Text.hpp"
#include "CelestiaWorks/Graphics/WindowHandle.hpp"
#include "CelestiaWorks/Graphics/Sprite.hpp"
#include "CelestiaWorks/System/Keyboard.hpp"
#include "CelestiaWorks/System/Mouse.hpp"
#include "CelestiaWorks/Graphics/VertexBuffer.hpp"

static std::vector<celestia::Vertex> vertices = {{{600.0f, 350.0f}, {0.f, 0.f}, {0.0f, 0.5f, 1.0f}}, // Vertex 8
  {{900.0f, 350.0f}, {1.f, 0.f}, {0.2f, 0.8f, 0.2f}},												 // Vertex 9
  {{900.0f, 750.0f}, {1.f, 1.f}, {0.8f, 0.2f, 0.2f}},												 // Vertex 10
  {{600.0f, 750.0f}, {0.f, 1.f}, {0.2f, 0.2f, 0.8f}}};

/*
 *  TODO:
 *  TEE EXPORT CLASS JOKA SISÄLTÄÄ SEN DEC HOMMA RÖPÖN
 */

int main()
{
	try
	{
		celestia::Vec2i windowSize = {900, 800};

		celestia::WindowHandle window(windowSize, "CelestiaWorks testbed");
		window.setFrameRateLimit(60);

		window.createTextRenderer(10, 400);

		celestia::Font font;
		if(!font.loadFont("assets/yoster.ttf", 32))
		{
			std::cout << "FAILED TO LOAD FONT FROM MAIN!\n";
		}

		celestia::VertexBuffer buffer(vertices.size(), celestia::Usage::STATIC_INDEXED, celestia::DrawType::QUAD);
		buffer.create(vertices.data(), vertices.size());

		celestia::Text text({100.f, 700.f}, "First text object   guhhhh..", {22, 45, 67, 255}, &font);
		celestia::Text text1({600.f, 100.f}, "Second text object :p", {155, 9, 1, 100}, &font);

		celestia::Color color = {78, 128, 81, 255};

		window.setClearColor(color);

		celestia::Texture textures[4];
		textures[0].loadTexture("assets/test.png");
		textures[1].loadTexture("assets/TempAsset1.png");
		textures[2].loadTexture("assets/RatSpriteSheet.png");
		textures[3].loadTexture("assets/cats.jpg");

		celestia::RenderPipeline pipeline{};
		pipeline.shader = nullptr;
		pipeline.texture = &textures[3];

		std::vector<celestia::Sprite> sprites;

		float tempX = windowSize.x / 5.0f;
		float tempY = windowSize.y / 5.0f;

		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				celestia::Vec2 position = {j * tempX, i * tempY};
				celestia::Vec2 size = {155.f, 155.f};

				celestia::Sprite sprite(position, size);
				sprite.setTexture(textures[j % 2]);

				sprites.push_back(sprite);
			}
		}

		celestia::Sprite testSprite({200.f, 600.f}, {50.f, 50.f});
		testSprite.setTexture(textures[2]);
		testSprite.setTextureRectSize({32.f, 32.f});

		int currentTime = 0;

		const int updateFrameNumber = 8;
		int rectPositionX = 0;

		celestia::Vec2i mousePos;

		while(window.isOpen())
		{
			currentTime++;

			if(celestia::Keyboard::isKeyPressed(celestia::Keyboard::Key::A))
			{
				std::cout << "A PRESSED OMG\n";
			}

			mousePos = celestia::Mouse::mousePosition();
			mousePos = window.screenSpaceToWindowSpace(mousePos);
			// std::cout << "x: " << mousePos.x << " y: " << mousePos.y << "\n";

			window.beginRenderPass();

			for(celestia::Sprite& currentSprite : sprites)
			{
				window.draw(currentSprite);
			}

			if(currentTime >= updateFrameNumber)
			{
				rectPositionX++;
				rectPositionX %= 18;
				currentTime = 0;
				testSprite.setTextureRectPosition({rectPositionX, 0});
			}

			// window.draw(text);

			// window.draw(testSprite);
			// window.draw(text1);

			// window.draw(buffer, pipeline);

			window.endRenderPass();

			// Sleep(10);
		}
	}
	catch(const std::exception& e)
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
TEE TAPA LUODA STORAGE BUFFEREITA JA S�IL� NE ESIM RENDERI HOMMASSA TAI JOSSAIN MUUALLA.

*/
