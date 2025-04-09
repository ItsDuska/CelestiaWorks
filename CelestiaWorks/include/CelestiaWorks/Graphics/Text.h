#pragma once
#include "Font.h"
#include <string>
#include <vector>
#include "../System/CelestiaTypes.h"
#include "Drawable.h"

namespace celestia
{
	class Text : public Drawable
	{
	public:
		CELESTIA_WORKS Text(const Vec2 position, const char* symbols,const Color color, Font* font);
		CELESTIA_WORKS Text(const Text&) = delete;
		CELESTIA_WORKS Text& operator = (const Text&) = delete;
		//CELESTIA_WORKS ~Text();

		CELESTIA_WORKS Font* getFont();

		
	private:
		void draw(const RendererHandler& renderer, RenderPipeline* pipeline) const override;

		void updatePosition();

		Vec2 getTextSize();

		//CELESTIA_WORKS setText(const std::string& text);

		void disableDirtyFlag();



	private:
		Vec2 position; 
		std::string symbols;
		Color color;
		Font* font;
		bool dirty;
		Vec2 size;
		int id;
		
		std::vector<VertexBatch> vertices;

		//friend class WindowHandle;
		//unsigned int characterSize; //????
	};

	
}