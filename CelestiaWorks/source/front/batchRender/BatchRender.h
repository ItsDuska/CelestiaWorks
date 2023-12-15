#pragma once
#include <iostream>
#include <array>
#include "celestiaTypes/CelestiaTypes.h"

/*
namespace celestia
{
	class Mesh;
	class Buffer;

	class BatchRender
	{
	public:
		BatchRender(Buffer& buffer);
		~BatchRender();

		void beginBatch();
		void endBatch(Buffer* buffer);

		void flush();

		void drawQuad(const Vec2& position, const Vec2& size, const Vec4& color);
		void drawQuad(const Vec2& position, const Vec2& size, const uint32_t textureID);

	private:
		BatchVertex* quadBuffer;
		BatchVertex* quadBufferPtr;

		Mesh mesh;

		uint32_t indexCount;
		std::array<uint32_t, MAX_TEXTURES> textureSlots;
		uint32_t textureSlotIndex;
	};



}

*/