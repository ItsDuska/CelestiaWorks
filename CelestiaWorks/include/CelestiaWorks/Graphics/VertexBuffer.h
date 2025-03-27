#pragma once
#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif

#include "System/CelestiaTypes.h"
#include <memory>
#include "backend/core/VertexBufferInfo.h"
#include "Drawable.h"

namespace celestia
{

	/* T‰h‰n voisi laittaa template classin.
	* Voisimme antaa k‰ytt‰j‰lle t‰yden vallan luoda oma vertex type.
	* wa wa waa Miten t‰‰ vois saada j‰rkev‰sti.  
	*/
	class VertexBufferImpl;
	/*
	* Bufferi p‰ivitet‰‰n vain kun sit‰ kutsutaan p‰vitys funktiossa, muuten ei koskaan.
	*/

	class VertexBuffer : public Drawable
	{
	public:
		// TODO: Siirr‰ alemmalle layerille.
		

	public:
		CELESTIA_WORKS VertexBuffer() = delete;
		CELESTIA_WORKS VertexBuffer(size_t size, Usage usage, DrawType type);
		CELESTIA_WORKS VertexBuffer(Usage usage, DrawType type);
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

		void setDrawType(DrawType type);


		size_t getVertexCount() const;

		void draw(const RendererHandler& renderer) const override;
		void draw(const RendererHandler& renderer, uint32_t amountToDraw, );

	private:

		size_t size;
		Usage usage;
		DrawType type;
		
		std::unique_ptr<VertexBufferImpl> bufferImpl;
	};
}