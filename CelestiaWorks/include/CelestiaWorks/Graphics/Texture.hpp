#pragma once
#include "../System/Vector.hpp"
#include <memory>

namespace celestia
{
	namespace vk
	{
		struct RawTexture;
	}

	class Texture
	{
	public:
		CELESTIA_WORKS Texture();
		CELESTIA_WORKS Texture(const vk::RawTexture* rawTexture, Vec2i textureSize, bool ownsTexture = false);
		CELESTIA_WORKS Texture(const Texture&) = delete;
		CELESTIA_WORKS Texture& operator=(const Texture&) = delete;
		CELESTIA_WORKS Texture(Texture&&) = delete;
		CELESTIA_WORKS Texture& operator=(Texture&&) = delete;
		CELESTIA_WORKS ~Texture();

		CELESTIA_WORKS bool loadTexture(const char* filepath);
		CELESTIA_WORKS Vec2i getSize() const;

		const vk::RawTexture* getRawTexturePtr() const;

	private:
		Vec2i size;
		std::unique_ptr<vk::RawTexture> pixels;
		const vk::RawTexture* nonOwningPixels;
		bool ownsTexture;

		friend class WindowHandle;
		friend class RenderTexture;
	};
} // namespace celestia
