#pragma once
#include "Vulkan/CelestiaVulkanTypes.hpp"

#include "System/CelestiaTypes.hpp"
#include "Graphics/VertexBufferInfo.hpp"

/*
 *
 *  Tee t�nne kaikki vulkanin puolella olevat homamt
 *
 *
 *
 */
namespace celestia
{
	class VertexBufferImpl
	{
	public:
		VertexBufferImpl();
		// Use custom vertex format
		// template<typename Vertex_t>
		// void create(Vertex_t* vertices, size_t size)
		//{
		// template func so we need to this here Guuuuh.
		//}

		// Create VB by using default Vertex format that CelestiaWorks provides.
		void create(Vertex* vertices, size_t size, DrawType drawType, Usage usage);

		// guh....
		// void resize(size_t size); // TODO: guh

		// Update the VB by using the default Vertex format that CelestiaWorks provides.
		// void update(size_t offset, Vertex* vertices, size_t size); // TODO: p�ivit� buffer jotenki maagisesti

		// Update the VB by using a custom vertex format.
		// template<typename Vertex_t>
		// void update(size_t offset, Vertex_t* vertices, size_t size);

		void freeBuffers() const;

		vk::Mesh* getBufferPairPtr();

	private:
		vk::Mesh mesh;
	};
} // namespace celestia
