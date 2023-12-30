#include "Sprite.h"

celestia::Sprite::Sprite(Vec2 position, Vec2 size)
	: texture(nullptr), textureRect({})
{
	spriteRect.position = position;
	spriteRect.size = size;
	updateQuadPosition();
}

void celestia::Sprite::setTexture(Texture& texture)
{
	this->texture = &texture;
	setTextureRectPosition({ 0.f,0.f });
	const Vec2i temp = texture.getSize();
	setTextureRectSize({ temp.x,temp.y });
}

const celestia::Texture* celestia::Sprite::getTexture() const
{
	return texture;
}

CELESTIA_WORKS void celestia::Sprite::setTextureRectPosition(const Vec2 position)
{
	textureRect.position = position;
	updateQuadTexCoord();
}

CELESTIA_WORKS celestia::Vec2 celestia::Sprite::getTextureRectPosition() const
{
	return textureRect.position;
}

void celestia::Sprite::setTextureRectSize(const Vec2 size)
{
	textureRect.size = size;
	updateQuadTexCoord();
}

celestia::Vec2 celestia::Sprite::getTextureRectSize() const
{
	return textureRect.size;
}

void celestia::Sprite::setPosition(const Vec2 newPosition)
{
	this->spriteRect.position = newPosition;
	updateQuadPosition();
}

celestia::Vec2 celestia::Sprite::getPosition() const
{
	return spriteRect.position;
}

void celestia::Sprite::setSize(const Vec2 newSize)
{
	spriteRect.size = newSize;
	updateQuadPosition();
}

celestia::Vec2 celestia::Sprite::getSize() const
{
	return spriteRect.size;
}

void celestia::Sprite::updateQuadPosition()
{
	quad[0].position =   spriteRect.position;
	quad[1].position = { spriteRect.position.x + spriteRect.size.x, spriteRect.position.y };
	quad[2].position = { spriteRect.position.x + spriteRect.size.x, spriteRect.position.y + spriteRect.size.y };
	quad[3].position = { spriteRect.position.x,						spriteRect.position.y + spriteRect.size.y };
}

void celestia::Sprite::updateQuadTexCoord()
{
	quad[0].texCoord = {  textureRect.position.x * textureRect.size.x,  textureRect.position.y * textureRect.size.y };
	quad[1].texCoord = { (textureRect.position.x + 1.0f) * textureRect.size.x,  textureRect.position.y * textureRect.size.y };
	quad[2].texCoord = { (textureRect.position.x + 1.0f) * textureRect.size.x, (textureRect.position.y + 1.0f) * textureRect.size.y };
	quad[3].texCoord = {  textureRect.position.x * textureRect.size.x, (textureRect.position.y + 1.0f) * textureRect.size.y };

	const Vec2i textureSize = texture->getSize();

	quad[0].texCoord /= textureSize;
	quad[1].texCoord /= textureSize;
	quad[2].texCoord /= textureSize;
	quad[3].texCoord /= textureSize;
}

