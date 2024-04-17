#pragma once
#include "Vector.h"
#include "Matrix.h"


namespace celestia
{
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
}

