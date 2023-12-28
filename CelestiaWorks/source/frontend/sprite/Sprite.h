#pragma once
#include "celestiaTypes/CelestiaTypes.h"
#include "frontend/texture/Texture.h"

namespace celestia
{
	class Sprite
	{
	public:
		CELESTIA_WORKS Sprite(Vec2 position, Vec2 size);

		CELESTIA_WORKS void setTexture(Texture& texture);
		CELESTIA_WORKS const Texture* getTexture() const;

		CELESTIA_WORKS void setTextureRect(Rect& rect);
		CELESTIA_WORKS Rect getTextureRect() const;

		CELESTIA_WORKS void setPosition(Vec2 newPosition);
		CELESTIA_WORKS Vec2 getPosition() const;

		CELESTIA_WORKS void setSize(Vec2 newSize);
		CELESTIA_WORKS Vec2 getSize() const;

	private:
		Texture* texture;
		Rect textureRect;

		Vec2 position;
		Vec2 size;
	};
}

