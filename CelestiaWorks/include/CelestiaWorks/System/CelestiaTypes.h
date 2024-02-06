#pragma once
#include <stdint.h>
#include "Vector.h"
#include "Matrix.h"


namespace celestia
{
	typedef Vec<4, uint8_t> Color; // Maybe delete this?

	struct CELESTIA_WORKS Vertex
	{
		Vec2 position;
		Vec2 texCoord;
		Vec3 color;
		uint32_t texIndex;
	};

	struct CELESTIA_WORKS VertexPositions
	{
		Vec2 position;
		Vec2 texCoord;
	};

	struct CELESTIA_WORKS Rect
	{
		Vec2i position;
		Vec2i size;
	};

	struct CELESTIA_WORKS FloatRect
	{
		Vec2 position;
		Vec2 size;
	};

	struct PUSH_CONSTANTS //TODO: Yeet this somewhere else.
	{
		alignas(16) Mat4 projection;
	};
}

