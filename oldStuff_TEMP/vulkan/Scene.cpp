#include "Scene.h"

celestia::Scene::Scene(Buffer& buffer, Pipeline& pipeline)
	: buffer{buffer}, pipeline{pipeline}
{
	buffer.createMesh();
	loadMeshes();
}

void celestia::Scene::loadMeshes()
{
	RenderObject temp;
	temp.mesh = buffer.get_mesh("temp");
	temp.material = pipeline.getMaterial("default");
	temp.transformMatrix = glm::mat4{ 1.0f };
	renderables.push_back(temp);
}

std::vector<celestia::RenderObject> &celestia::Scene::getRenderObjects()
{
	return renderables;
}

uint32_t celestia::Scene::getRenderObjectSizes()
{
	return (uint32_t)renderables.size();
}
