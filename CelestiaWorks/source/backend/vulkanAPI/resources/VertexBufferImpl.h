#pragma once
#include "backend/vulkanAPI/Core/Buffer.h"
#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include "System/CelestiaTypes.h"


/*
* 
*  Tee tänne kaikki vulkanin puolella olevat homamt
* 
* 
* 
*/
namespace celestia
{
	class VertexBufferImpl
	{
	public:
		// Use custom vertex format
		template<typename Vertex_t>
		void create(Vertex_t* vertices, size_t size)
		{
			// template func so we need to this here Guuuuh.
		}

		// Create VB by using default Vertex format that CelestiaWorks provides.
		void create(Vertex* vertices, size_t size);

		// guh....
		void resize(size_t size); // TODO: guh

		// Update the VB by using the default Vertex format that CelestiaWorks provides.
		void update(size_t offset, Vertex* vertices, size_t size); // TODO: päivitä buffer jotenki maagisesti

		// Update the VB by using a custom vertex format.
		template<typename Vertex_t>
		void update(size_t offset, Vertex_t* vertices, size_t size);

	private:

		AllocatedBuffer vertexBuffer;
	};
}

