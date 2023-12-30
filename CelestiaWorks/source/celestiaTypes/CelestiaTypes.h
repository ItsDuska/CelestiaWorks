#pragma once

#include <stdint.h>
#include "celestiaTypes/Vector.h"
#include "celestiaTypes/Matrix.h"

namespace celestia
{
	typedef Vec<4, uint8_t> Color;

	struct CELESTIA_WORKS Vertex
	{
		Vec2 position;
		Vec2 texCoord;
		Vec3 color;
		uint32_t texIndex;
	};

	struct VertexPositions
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

	struct PUSH_CONSTANTS
	{
		alignas(16) Mat4 projection;
	};
}

