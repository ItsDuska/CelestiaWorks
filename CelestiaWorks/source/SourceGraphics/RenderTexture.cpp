#include "Graphics/RenderTexture.hpp"
#include "Vulkan/FrameBuffer.hpp"
#include "Vulkan/CelestiaVulkanTypes.hpp"
#include "Backend/VulkanAPI/RenderBack/RendererHandler.hpp"
#include "System/Vector.hpp"
#include <stdexcept>

celestia::RenderTexture::RenderTexture(Vec2i size, bool hasDepthBuffer) : size(size), hasDepth(hasDepthBuffer)
{
	if(size.x <= 0 || size.y <= 0)
	{
		throw std::invalid_argument("RenderTexture size must be greater than 0 in both dimensions");
	}
	initialize();
}

celestia::RenderTexture::~RenderTexture()
{
	cleanup();
}

void celestia::RenderTexture::initialize()
{
	// Use SwapChain-compatible format for pipeline compatibility
	frameBuffer = std::make_unique<vk::FrameBuffer>(size, hasDepth, VK_FORMAT_B8G8R8A8_SRGB);
	if(!frameBuffer->isValid())
	{
		throw std::runtime_error("Failed to initialize RenderTexture: The internal FrameBuffer is not valid.");
	}

	const auto* colorTex = frameBuffer->getColorTexture();
	if(!colorTex)
	{
		throw std::runtime_error("Failed to initialize RenderTexture: FrameBuffer returned a null color texture.");
	}

	// Create a non-owning texture that references the FrameBuffer's texture
	if(colorTex && colorTex->allocatedImage.image != VK_NULL_HANDLE)
	{
		m_texture = std::make_unique<Texture>(colorTex, size, false);
	}
	else
	{
		throw std::runtime_error("Failed to initialize RenderTexture: FrameBuffer color texture is invalid.");
	}
}

void celestia::RenderTexture::cleanup()
{
	// FrameBuffer will clean up its own textures
	// Reset our texture wrapper
	m_texture.reset();
}

celestia::Vec2i celestia::RenderTexture::getSize() const
{
	return size;
}

const celestia::Texture* celestia::RenderTexture::getTexture() const
{
	if(!m_texture)
	{
		throw std::runtime_error("RenderTexture: Internal texture is not initialized!");
	}
	return m_texture.get();
}

void celestia::RenderTexture::resize(Vec2i newSize)
{
	if(newSize.x > 0 && newSize.y > 0)
	{
		size = newSize;
		frameBuffer->resize(size);

		// Update the texture wrapper to reference the new framebuffer texture
		const vk::RawTexture* colorTex = frameBuffer->getColorTexture();
		if(colorTex && colorTex->allocatedImage.image != VK_NULL_HANDLE)
		{
			m_texture = std::make_unique<Texture>(colorTex, size, false);
		}
		else
		{
			throw std::runtime_error("RenderTexture resize failed: FrameBuffer color texture is invalid.");
		}
	}
}

void celestia::RenderTexture::activateAsRenderTarget()
{
	RendererHandler::getInstance().setActiveRenderTarget(this);
}

const celestia::vk::RawTexture* celestia::RenderTexture::getRawTexture() const
{
	return frameBuffer ? frameBuffer->getColorTexture() : nullptr;
}

VkRenderPass celestia::RenderTexture::getRenderPass() const
{
	return frameBuffer->getRenderPass();
}

VkFramebuffer celestia::RenderTexture::getFramebuffer() const
{
	return frameBuffer->getFramebuffer();
}
