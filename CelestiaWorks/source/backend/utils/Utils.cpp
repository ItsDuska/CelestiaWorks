#include "Utils.h"
#include <fstream>
#include "vulkan/vulkan.h"

#define MAX_COLOR_VALUE 255.f;

const std::vector<uint32_t> celestia::utils::readFile(const char* filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        std::string text = "Failed to open file " + std::string(filename) + "\n";
        throw std::runtime_error(text);
    }
    const size_t fileSize = (const size_t)file.tellg(); 
    const size_t bufferSize = fileSize / sizeof(uint32_t); 

    std::vector<uint32_t> buffer(bufferSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();

    return buffer;
}

VkVertexInputBindingDescription celestia::utils::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 4> celestia::utils::getAttributeDescription()
{
    std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
    //position
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);
    //texCoord
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, texCoord);
    //color
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, color);

    //textureIndex
    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32_UINT;
    attributeDescriptions[3].offset = offsetof(Vertex, texIndex);

    return attributeDescriptions;
}

//Turn 8bit 0-255 color to a float ranging between 0-1. 
celestia::Vec4 celestia::utils::normalizeColor(Color& color)
{
    Vec4 newColor{};
    newColor.x = color.r / MAX_COLOR_VALUE;
    newColor.y = color.g / MAX_COLOR_VALUE;
    newColor.z = color.b / MAX_COLOR_VALUE;
    newColor.w = color.a / MAX_COLOR_VALUE;
    return newColor;
}


