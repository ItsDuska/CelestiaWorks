#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>
#include "VertexBufferInfo.h"

namespace celestia
{
    void generateQuadIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset)
    {
        indices.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            indices.push_back(offset);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset);
            offset += 4;
        }
    }

    void generateTriangleIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset)
    {
        indices.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            indices.push_back(offset);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            offset += 3;
        }
    }

    void generateLineIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset)
    {
        indices.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            indices.push_back(offset);
            indices.push_back(offset + 1);
            offset += 2;
        }
    }

    void generateDotIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset)
    {
        indices.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            indices.push_back(offset);
            offset += 1;
        }
    }



    std::vector<uint16_t> createIndexBufferForDrawType(size_t vertexCount, DrawType type)
    {
        std::vector<uint16_t> indices;
        uint16_t offset = 0;

        switch (type)
        {
        case DrawType::QUAD:
            generateQuadIndices(indices, vertexCount / 4, offset);
            break;
        case DrawType::TRIANGLE:
            generateTriangleIndices(indices, vertexCount / 3, offset);
            break;
        case DrawType::LINES:
            generateLineIndices(indices, vertexCount / 2, offset);
            break;
        case DrawType::DOTS:
            generateDotIndices(indices, vertexCount, offset);
            break;
        default:
            throw std::invalid_argument("Unsupported DrawType");
        }

        return indices;
    }
}
