#include "Sprite.h"

celestia::Sprite::Sprite(Vec2 position, Vec2 size)
	: position(position), size(size), texture(nullptr), textureRect({})
{
}

void celestia::Sprite::setTexture(Texture& texture)
{
	this->texture = &texture;
	Rect temp{};
	temp.position = { 0,0 };
	temp.width = texture.getSize().x;
	temp.height = texture.getSize().y;
	setTextureRect(temp);
}

const celestia::Texture* celestia::Sprite::getTexture() const
{
	return texture;
}

void celestia::Sprite::setTextureRect(Rect& rect)
{
	this->textureRect = rect;
}

celestia::Rect celestia::Sprite::getTextureRect() const
{
	return textureRect;
}

void celestia::Sprite::setPosition(Vec2 newPosition)
{
	this->position = newPosition;
}

celestia::Vec2 celestia::Sprite::getPosition() const
{
	return position;
}

void celestia::Sprite::setSize(Vec2 newSize)
{
	this->size = newSize;
}

celestia::Vec2 celestia::Sprite::getSize() const
{
	return size;
}

