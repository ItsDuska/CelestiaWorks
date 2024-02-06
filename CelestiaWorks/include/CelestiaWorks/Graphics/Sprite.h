#pragma once
#include "../System/CelestiaTypes.h"
#include "Texture.h"

namespace celestia
{
	class Sprite
	{
	public:
		CELESTIA_WORKS Sprite(Vec2 position, Vec2 size);

		CELESTIA_WORKS void setTexture(Texture& texture);
		CELESTIA_WORKS const Texture* getTexture() const;

		CELESTIA_WORKS void setTextureRectPosition(const Vec2 position);
		CELESTIA_WORKS Vec2 getTextureRectPosition() const;

		CELESTIA_WORKS void setTextureRectSize(const Vec2 size);
		CELESTIA_WORKS Vec2 getTextureRectSize() const;

		CELESTIA_WORKS void setPosition(const Vec2 newPosition);
		CELESTIA_WORKS Vec2 getPosition() const;

		CELESTIA_WORKS void setSize(const Vec2 newSize);
		CELESTIA_WORKS Vec2 getSize() const;
	private:
		void updateQuadPosition();
		void updateQuadTexCoord();

		friend class WindowHandle;
	private:
		Texture* texture;
		FloatRect textureRect;
		FloatRect spriteRect;

		VertexPositions quad[4];
	};
}

