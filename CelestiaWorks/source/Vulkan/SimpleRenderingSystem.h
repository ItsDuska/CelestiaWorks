#pragma once

/*
Simple rendering system:
	public:
	constructor(&device, renderpass);

	void renderGameObjects(VkCommandBuffer commandBuffer, vector<GameObject> &gameObjects);

	private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);

	Device &device;
	unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;


*/

/*

Pipelinee pit�s pysty� muokkaamaan eritt�in helposti
pipline heitet��n rendering classille

*/
class SimpleRenderingSystem
{
};
