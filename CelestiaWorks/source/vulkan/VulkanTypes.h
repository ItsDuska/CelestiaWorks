#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <functional>
#include <deque>

namespace celestia {

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription();
		
		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescription();
	};

	struct DeletionQueue
	{
		std::deque<std::function<void()>> deletors;

		void pushFunction(std::function<void()>&& function);

		void flush();
	};

	struct AllocatedBuffer
	{
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
	};

	
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
		AllocatedBuffer vertexAllocation;
		AllocatedBuffer indiciesAllocation;
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


}