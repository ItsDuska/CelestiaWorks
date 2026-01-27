#include "DefaultSingleRender.hpp"
#include "Vulkan/Image.hpp"
#include "Vulkan/Pipeline.hpp"
#include "Vulkan/ShaderObject.hpp"
#include "Vulkan/SwapChain.hpp"

celestia::DefaultSingleRenderer::DefaultSingleRenderer(Render& render) : render(render), info({})
{
	descriptor.addBinding(0, vk::DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
	descriptor.build();

	info.layout = descriptor.getLayout();

	vk::ShaderObject shader;
	shader.loadShader(nullptr, ShaderType::VERTEX_SHADER, RenderGroup::BASIC, true);
	shader.loadShader(nullptr, ShaderType::FRAGMENT_SHADER, RenderGroup::BASIC, true);
	shader.createPushConstants<vk::PUSH_CONSTANTS>(0, ShaderType::VERTEX_SHADER);

	vk::PipelineOptions options{};
	options.blending = true;

	vk::Pipeline pipeline;
	pipeline.createColorBlendAttachment(options.blending);
	pipeline.createInputAssembly(vk::DrawingMode::TRIANGLE);
	pipeline.createMultisampling();
	pipeline.createRasterizer(vk::DrawingMode::TRIANGLE);
	pipeline.createScissors({0, 0}, render.swapChain->extent);
	pipeline.createViewport({0, 0},
	  {static_cast<float>(render.swapChain->extent.width), static_cast<float>(render.swapChain->extent.height)});

	const VkVertexInputBindingDescription bindingDescription = utils::createBindingDescription(0, sizeof(Vertex));

	utils::CustomVertexInputAttributeDescriptionFactory attributeDescriptions;
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));

	pipeline.createVertexInputStateCreateInfo(attributeDescriptions, bindingDescription, 1);

	info.material =
	  pipeline.createPipeline(shader, vk::DrawingMode::TRIANGLE, &info.layout, render.swapChain->getRenderPass());
}

celestia::DefaultSingleRenderer::~DefaultSingleRenderer()
{
}

void celestia::DefaultSingleRenderer::draw(vk::Mesh* mesh, const uint32_t amountToDraw, const vk::RawTexture* texture)
{
	descriptor.updateTexture(0, &texture->imageView, render.image->textureSampler, 1, render.currentFrame);
	descriptor.flushWrites();

	info.mesh = mesh;
	info.amountToDraw = amountToDraw;
	info.descriptor = descriptor.getDescriptorSet(render.currentFrame);
	render.submitIndexedDraw(info);
}
