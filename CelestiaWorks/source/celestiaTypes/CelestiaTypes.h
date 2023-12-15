#pragma once

#include <stdint.h>
#include "celestiaTypes/Vector.h"


namespace celestia
{
	typedef Vec<4, uint8_t> Color;

	struct CELESTIA_WORKS Vertex
	{
		Vec2 position;
		Vec2 texCoord;
		Vec3 color;
	};

	struct CELESTIA_WORKS BatchVertex
	{
		Vec2 position;
		Vec2 texCoord;
		Vec4 color;
		float texIndex;
	};

}

