#pragma once
#include <cstdint>
#include <vector>
#include "Graphics/VertexBufferInfo.hpp"

namespace celestia
{
	void generateQuadIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset);

	void generateTriangleIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset);

	void generateLineIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset);

	void generateDotIndices(std::vector<uint16_t>& indices, size_t count, uint16_t& offset);

	std::vector<uint16_t> createIndexBufferForDrawType(size_t vertexCount, DrawType type);
} // namespace celestia
