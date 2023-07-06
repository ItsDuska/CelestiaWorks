#pragma once
#include "vulkan/Buffer.h"
#include "vulkan/Pipeline.h"

namespace celestia
{
	class Scene
	{
	public:
		Scene(Buffer &buffer, Pipeline& pipeline);
		void loadMeshes();

		std::vector<RenderObject>& getRenderObjects();
		uint32_t getRenderObjectSizes();
	private:
		std::vector<RenderObject> renderables;
		Buffer& buffer;
		Pipeline& pipeline;
	};
}


