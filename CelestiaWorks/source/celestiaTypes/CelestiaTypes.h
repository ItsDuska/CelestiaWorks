#pragma once

#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

namespace celestia
{
	struct CELESTIA_WORKS Vec2i
	{
		int x;
		int y;
	};

	struct CELESTIA_WORKS Vec2f
	{
		float x;
		float y;
	};

	struct CELESTIA_WORKS Vec3f
	{
		float r;
		float g;
		float b;
	};

	struct CELESTIA_WORKS Vertex
	{
		Vec2f position;
		Vec2f texCoord;
		Vec3f color;
	};

}

