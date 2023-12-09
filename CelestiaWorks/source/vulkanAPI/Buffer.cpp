#include "Buffer.h"
#include "vulkanAPI/Device.h"


celestia::Buffer::Buffer(Device& device)
    :device(device)
{
    RawMesh mesh{};
    mesh.vertices = {
        {{-0.5,-0.5},{0,0},{1,0,0}},
        {{0.5,-0.5},{0,0}, {0,1,0} },
        {{0.5,0.5 },{ 0,0 },{ 0,0,1 } },
        {{-0.5,0.5 },{ 0,0 },{ 0.5,0.5,1 } }
    };

    mesh.indices = { 0,1,2,2,3,0 };
    
    createMesh(mesh, defaultMesh);
}

celestia::Buffer::~Buffer()
{
}

celestia::AllocatedBuffer celestia::Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    AllocatedBuffer buffer{};

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.device, &bufferInfo, nullptr, &buffer.buffer) != VK_SUCCESS)
    {
	    throw std::runtime_error("Failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirments;
    vkGetBufferMemoryRequirements(device.device, buffer.buffer, &memRequirments);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirments.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirments.memoryTypeBits, properties);

    if (vkAllocateMemory(device.device, &allocInfo, nullptr, &buffer.memory) != VK_SUCCESS)
    {
	    throw std::runtime_error("failed to allocate vertex buffer memory");
    }

    vkBindBufferMemory(device.device, buffer.buffer, buffer.memory, 0);

    return buffer;
}

void celestia::Buffer::createMesh(RawMesh& rawMesh, Mesh& mesh)
{
    mesh.indexBufferSize = static_cast<uint32_t>(rawMesh.indices.size());
    mesh.indexBuffer = createIndexBuffer(rawMesh);
    mesh.vertexBufferSize = static_cast<uint32_t>(rawMesh.vertices.size());
    mesh.vertexBuffer = createVertexBuffer(rawMesh);
}

celestia::Mesh& celestia::Buffer::getDefaultMesh()
{
    return defaultMesh;
}

/*
template<typename DataType> void celestia::Buffer::mapBuffer(const DataType *data, AllocatedBuffer& buffer,VkDeviceSize size)
{
    void* tempData;
    vkMapMemory(device.device, buffer.memory, 0, size, 0, &tempData);
    memcpy(tempData, data, static_cast<size_t>(size));
    vkUnmapMemory(device.device, buffer.memory);
}
*/

celestia::AllocatedBuffer celestia::Buffer::createVertexBuffer(RawMesh& rawMesh)
{
    VkDeviceSize bufferSize = sizeof(rawMesh.vertices[0]) * rawMesh.vertices.size();
    
    AllocatedBuffer stagingBuffer = createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //mapBuffer<Vertex>(rawMesh.vertices.data(), stagingBuffer, bufferSize);

    void* data;
    vkMapMemory(device.device, stagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data,rawMesh.vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.device, stagingBuffer.memory);

    AllocatedBuffer vertexBuffer = createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copyBuffer(stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);

    vkDestroyBuffer(device.device, stagingBuffer.buffer, nullptr);
    vkFreeMemory(device.device, stagingBuffer.memory, nullptr);

    device.deletionQueue.pushFunction([=]() {
        vkDestroyBuffer(device.device, vertexBuffer.buffer, nullptr);
        vkFreeMemory(device.device, vertexBuffer.memory, nullptr);
        }
    );

    return vertexBuffer;
}

celestia::AllocatedBuffer celestia::Buffer::createIndexBuffer(RawMesh& rawMesh)
{
    VkDeviceSize bufferSize = sizeof(uint16_t) * rawMesh.indices.size();
    AllocatedBuffer stagingBuffer = createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


    //mapBuffer<uint16_t>(rawMesh.indices.data(), stagingBuffer, bufferSize);

    void* data;
    vkMapMemory(device.device, stagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, rawMesh.indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.device, stagingBuffer.memory);


    /*
    void* data;
    vkMapMemory(device.device, stagingBuffer.memory, 0, bufferSize, 0, &data);
    memcpy(data, mesh.indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.getDevice(), stagingBufferMemory);
    */

    AllocatedBuffer indexBuffer = createBuffer(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    copyBuffer(stagingBuffer.buffer, indexBuffer.buffer, bufferSize);

    vkDestroyBuffer(device.device, stagingBuffer.buffer, nullptr);
    vkFreeMemory(device.device, stagingBuffer.memory, nullptr);

    device.deletionQueue.pushFunction([=]() {
        vkDestroyBuffer(device.device, indexBuffer.buffer, nullptr);
        vkFreeMemory(device.device, indexBuffer.memory, nullptr);
        });
    
    return indexBuffer;
}


void celestia::Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device.commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.graphicsQueue);
    vkFreeCommandBuffers(device.device, device.commandPool, 1, &commandBuffer);
}

uint32_t celestia::Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(device.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}
