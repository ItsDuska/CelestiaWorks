#pragma once
#include "../System/Vector.hpp"
#include <memory>

namespace celestia
{
	struct RawTexture;

	class Texture
	{
	public:
		CELESTIA_WORKS Texture();
		CELESTIA_WORKS Texture(const Texture&) = delete;
		CELESTIA_WORKS Texture& operator=(const Texture&) = delete;
		CELESTIA_WORKS ~Texture();

		CELESTIA_WORKS bool loadTexture(const char* filepath);
		CELESTIA_WORKS Vec2i getSize() const;

		const RawTexture* getRawTexturePtr() const;

	private:
		Vec2i size;
		std::unique_ptr<RawTexture> pixels;

		friend class WindowHandle;
	};
} // namespace celestia