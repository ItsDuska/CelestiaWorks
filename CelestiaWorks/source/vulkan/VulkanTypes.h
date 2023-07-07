#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
#include <functional>
#include <deque>
#include "vk_mem_alloc.h" // TÄN VOIS SIIRTÄÄ MUUALLE???


namespace celestia {

	const int MAX_FRAMES_IN_FLIGHT = 2;

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

	struct AllocatedImage {
		VkImage image;
		VmaAllocation allocation;
	};

	struct CameraData {
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewproj;
	};

	struct FrameData {
		AllocatedBuffer cameraBuffer;
		VkDescriptorSet globalDescriptor;
	};

	struct MeshPushConstants {
		glm::vec4 data;
		glm::mat4 renderMatrix;
	};
}