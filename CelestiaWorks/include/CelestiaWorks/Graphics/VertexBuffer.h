#pragma once
#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include "System/CelestiaTypes.h"
#include <memory>


namespace celestia
{

	/* T‰h‰n voisi laittaa template classin.
	* Voisimme antaa k‰ytt‰j‰lle t‰yden vallan luoda oma vertex type.
	* wa wa waa Miten t‰‰ vois saada j‰rkev‰sti.  
	*/
	class IndexBuffer; // Guh do something about this. Pit‰‰kˆ meid‰n oikeasti k‰ytt‰‰ t‰t‰.
	class VertexBufferImpl;
	/*
	* Bufferi p‰ivitet‰‰n vain kun sit‰ kutsutaan p‰vitys funktiossa, muuten ei koskaan.
	*/


	class VertexBuffer
	{
	public:
		/// Tells the vulkan api how often the data will change.
		// TODO: Siirr‰ alemmalle layerille.
		enum class Usage : uint8_t
		{
			STATIC, // Almost never changes || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			STATIC_INDEXED, // Almost never changes. Also uses Index Buffer || USE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			STREAM, // Changes often || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			STREAM_INDEXED // Changes often with Index Buffer. || USE VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ja VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

	public:
		CELESTIA_WORKS VertexBuffer() = default;
		CELESTIA_WORKS VertexBuffer(size_t size, Usage usage);
		CELESTIA_WORKS VertexBuffer(Usage usage);
		CELESTIA_WORKS VertexBuffer(const VertexBuffer&) = delete;
		CELESTIA_WORKS VertexBuffer& operator = (const VertexBuffer&) = delete;
		CELESTIA_WORKS ~VertexBuffer();

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
		void update(size_t offset, Vertex* vertices, size_t size); // TODO: p‰ivit‰ buffer jotenki maagisesti

		// Update the VB by using a custom vertex format.
		template<typename Vertex_t>
		void update(size_t offset, Vertex_t* vertices, size_t size);

		// set the vertex usage type duhhh..... 
		void setUsage(Usage usage);

	private:

		size_t size;
		Usage currentUsage; 

		std::unique_ptr<VertexBufferImpl> bufferImpl;

		IndexBuffer* indexBuffer; // T‰t‰ ei v‰ltt‰m‰tt‰ tarvita joten se voi olla nullptr.
	};
}