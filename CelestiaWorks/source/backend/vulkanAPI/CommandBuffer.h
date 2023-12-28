#pragma once
#include <vulkan/vulkan.h>
/*
Simple helper functions for managing commandBuffers.
*/

namespace celestia
{
	VkCommandBuffer beginSingleTimeCommands(VkCommandPool cmdPool, VkDevice device);

	void endSingleTimeCommands(VkQueue graphicsQueue,VkCommandPool cmdPool, VkDevice device, VkCommandBuffer buffer);

}