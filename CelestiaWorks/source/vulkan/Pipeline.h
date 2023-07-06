#pragma once
#include "BuildPipeline.h"
#include "vulkan/SwapChain.h"
#include "VulkanTypes.h"
/*

pipeline tulee tallentuun unordered mappiin

struct Mesh {
	std::vector<Vertex> _vertices;

	AllocatedBuffer _vertexBuffer;
};


struct Material {
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct RenderObject {
	Mesh* mesh;

	Material* material;

	glm::mat4 transformMatrix;
};

std::vector<RenderObject> _renderables;

std::unordered_map<std::string,Material> _materials;
std::unordered_map<std::string,Mesh> _meshes;


Material* VulkanEngine::create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name)
{
	Material mat;
	mat.pipeline = pipeline;
	mat.pipelineLayout = layout;
	_materials[name] = mat;
	return &_materials[name];
}

Material* VulkanEngine::get_material(const std::string& name)
{
	//search for the object, and return nullptr if not found
	auto it = _materials.find(name);
	if (it == _materials.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}


Mesh* VulkanEngine::get_mesh(const std::string& name)
{
	auto it = _meshes.find(name);
	if (it == _meshes.end()) {
		return nullptr;
	}
	else {
		return &(*it).second;
	}
}


void VulkanEngine::draw_objects(VkCommandBuffer cmd,RenderObject* first, int count)
{
	//empty for now
}




*/

namespace celestia
{
	class Pipeline
	{
	public:
		Pipeline(Device &device, SwapChain &swapChain);
		~Pipeline();
		void initPipelines();
		/*
		func getPipeline 
		func getPipelineLayout
		*/
		VkPipeline getPipeline() { return meshPipeline; }
		// tänne get material ja mappi siitä
		Material* createMaterial(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);

		Material* getMaterial(const std::string& name);

	private:
		VkShaderModule createShaderModule(const std::vector<char>& code);
		VkPipelineShaderStageCreateInfo createShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule);
		VkPipelineInputAssemblyStateCreateInfo createInputAssembly(VkPrimitiveTopology primitiveTopology);
		VkViewport createViewport();
		VkRect2D createScissors();
		VkPipelineDynamicStateCreateInfo createDynamicState();
		VkPipelineRasterizationStateCreateInfo createRasterizer(VkPolygonMode polygonMode);
		VkPipelineMultisampleStateCreateInfo createMultisampling();
		VkPipelineColorBlendAttachmentState createColorBlendAttachment();
		VkPipelineLayoutCreateInfo createLayoutInfo();

		std::vector<char> readFile(const std::string& filename);

		Device& device;
		SwapChain& swapChain;

		VkPipelineLayout meshLayout;
		VkPipeline meshPipeline;

		std::unordered_map <std::string, Material> materials;
	};
}