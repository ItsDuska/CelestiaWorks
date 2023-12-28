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

	struct CELESTIA_WORKS Rect
	{
		Vec2i position;
		int width;
		int height;
	};

	struct PUSH_CONSTANTS
	{
		alignas(16) Mat4 projection;
	};
}

