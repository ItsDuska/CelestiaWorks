#include "TextRender.hpp"
#include "Backend/VulkanAPI/Core/Buffer.hpp"
#include "Backend/VulkanAPI/Core/Image.hpp"
#include "Backend/VulkanAPI/Core/Pipeline.hpp"
#include "Backend/VulkanAPI/Core/ShaderObject.hpp"
#include "Backend/VulkanAPI/Resources/FontReader.hpp"
#include "Backend/VulkanAPI/Core/Device.hpp"
#include "Backend/VulkanAPI/Core/SwapChain.hpp"

#define MAIN_BUFFER 0

celestia::TextRender::TextRender(Render& render, uint32_t maxTextObjects, uint32_t maxCharsPerBatch)
	: render(render), descriptors(std::make_unique<Descriptor>()), bufferSize(sizeof(Vec2Aligned) * maxTextObjects),
	  MAX_TEXT_COUNT(maxTextObjects), MAX_VERTEX_COUNT_PER_BATCH(maxCharsPerBatch * 4u),
	  MAX_INDEX_COUNT_PER_BATCH(maxCharsPerBatch * 4u * 6u)
{
	indexCount = 0;
	vertexCount = 0;
	transformationIndexCounter = 0;
	currentTexturePtr = nullptr;
	needsUpdate = false;
	active = false;
	defaultMaterial = {};

	glyphBuffer.resize(MAX_VERTEX_COUNT_PER_BATCH);
	transformationBuffer.resize(10);

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		storageBuffer[i] = buffer::createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}

	RawMesh tempMesh;
	tempMesh.vertices.resize(MAX_VERTEX_COUNT_PER_BATCH);
	tempMesh.indices.resize(MAX_INDEX_COUNT_PER_BATCH);

	int offset = 0;
	for(int i = 0; i < MAX_INDEX_COUNT_PER_BATCH; i += 6)
	{
		tempMesh.indices[static_cast<size_t>(i + 0)] = offset;
		tempMesh.indices[static_cast<size_t>(i + 1)] = 1 + offset;
		tempMesh.indices[static_cast<size_t>(i + 2)] = 2 + offset;

		tempMesh.indices[static_cast<size_t>(i + 3)] = 2 + offset;
		tempMesh.indices[static_cast<size_t>(i + 4)] = 3 + offset;
		tempMesh.indices[static_cast<size_t>(i + 5)] = offset;

		offset += 4;
	}

	// info.descriptors = set;
	info.mesh = buffer::createMesh(tempMesh);
	descriptors->addBinding(0, DescriptorType::COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
	descriptors->addBinding(1, DescriptorType::STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
	descriptors->build();

	info.layout = descriptors->getLayout();

	// descriptors->build(info.descriptors, info.layout);

	ShaderObject shader;
	shader.loadShader(nullptr, ShaderType::VERTEX_SHADER, RenderGroup::TEXT_BATCH, true);
	shader.loadShader(nullptr, ShaderType::FRAGMENT_SHADER, RenderGroup::TEXT_BATCH, true);
	shader.createPushConstants<PUSH_CONSTANTS>(0, ShaderType::VERTEX_SHADER);

	PipelineOptions options{};
	options.blending = true;

	Pipeline pipeline;
	pipeline.createColorBlendAttachment(options.blending);
	pipeline.createInputAssembly(DrawingMode::TRIANGLE);
	pipeline.createMultisampling();
	pipeline.createRasterizer(DrawingMode::TRIANGLE);
	pipeline.createScissors({0, 0}, render.swapChain->extent);
	pipeline.createViewport({0, 0},
	  {static_cast<float>(render.swapChain->extent.width), static_cast<float>(render.swapChain->extent.height)});

	const VkVertexInputBindingDescription bindingDescription = utils::createBindingDescription();
	utils::CustomVertexInputAttributeDescriptionFactory attributeDescriptions;
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexBatch, position));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexBatch, texCoord));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexBatch, color));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32_UINT, offsetof(VertexBatch, texIndex));

	pipeline.createVertexInputStateCreateInfo(attributeDescriptions, bindingDescription, 1);

	info.material =
	  pipeline.createPipeline(shader, DrawingMode::TRIANGLE, &info.layout, render.swapChain->getRenderPass());
}

celestia::TextRender::~TextRender()
{
	delete info.mesh;

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroyBuffer(Device::context.device, storageBuffer[i].buffer, nullptr);
		vkFreeMemory(Device::context.device, storageBuffer[i].memory, nullptr);
	}
}

void celestia::TextRender::begin()
{
	indexCount = 0;
	vertexCount = 0;
	transformationIndexCounter = 0;
}

void celestia::TextRender::end()
{
	// jos batch lista on eri kuin edellisen framen lista eli...
	// joku teksti on muuttunut, poistunut tai lis�tty.
	// silloin p�ivitet��n vertex buffer sek� storage buffer.
	// jos vain storage bufferin arvoja on muutettu niin p�ivitet��n vain se.
	// eli t�ll�in tekstit ovat pysyneet samoina, mutta niiden sijainti on vaihtunut.

	// storage buffer memory mapping (updating values)

	// if (!needsUpdate)
	//{
	// return;
	//}

	// needsUpdate = false;

	size_t transformationSize = transformationIndexCounter * sizeof(Vec2Aligned);

	void* data;
	vkMapMemory(Device::context.device, storageBuffer[render.currentFrame].memory, 0, transformationSize, 0, &data);
	std::memcpy(data, transformationBuffer.data(), transformationSize);
	vkUnmapMemory(Device::context.device, storageBuffer[render.currentFrame].memory);

	size_t vertexSize = vertexCount * sizeof(VertexBatch);
	buffer::updateBuffer(info.mesh->vertexBuffer, 0, vertexSize, glyphBuffer.data());


}

void celestia::TextRender::flush()
{
	info.amountToDraw = indexCount;
	info.descriptor = descriptors->getDescriptorSet(render.currentFrame);
	render.submitIndexedDraw(info);
}

void celestia::TextRender::drawText(const std::vector<VertexBatch>& vertices, const int size, const Vec2& position,
  const Font_t& font, bool dirty, const int id)
{
	if(!needsUpdate)
	{
		needsUpdate = dirty; // TODO: Finish this one.
	}

	// TODO: Tee t�m� hassu hassu loppuun my�s.
	if(currentTexturePtr != &font.texture)
	{
		/*
		const auto& index = fontTypeBufferMapKeys.find(font.texture.textureID);
		if (index == fontTypeBufferMapKeys.end())
		{
			// didn't find the item
			fontTypeBufferMapKeys[font.texture.textureID] = lastIndexInMap;
			lastIndexInMap++;
		}
		*/

		active = true;
		currentTexturePtr = &font.texture;

		/*
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			//descriptors->updateBuffer(storageBuffer[i].buffer, bufferSize, info.descriptors[i]);
			//descriptors->updateTexture(currentTexturePtr->imageView, render.image->textureSampler,
		info.descriptors[i]);
			//descriptors->updateSets();

			descriptors->updateTexture(&currentTexturePtr->imageView, render.image->textureSampler, 0, 1,
		info.descriptors[i]); descriptors->updateBuffer(&storageBuffer[i].buffer, bufferSize, 1, 1,
		info.descriptors[i]); descriptors->updateSets();
		}
		*/
		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			descriptors->updateBuffer(1, &storageBuffer[i].buffer, bufferSize, 1, i);
			descriptors->updateTexture(0, &currentTexturePtr->imageView, render.image->textureSampler, 1, i);
		}
		descriptors->flushWrites();
	}

	indexCount += size * 6;

	if(indexCount >= MAX_INDEX_COUNT_PER_BATCH)
	{
		end();
		flush();
		begin();
		indexCount += size * 6;
	}

	transformationBuffer[transformationIndexCounter].vec = position;

	for(const VertexBatch& vertex : vertices)
	{
		glyphBuffer[vertexCount] = vertex;
		glyphBuffer[vertexCount].texIndex = transformationIndexCounter;
		vertexCount++;
	}

	if(transformationIndexCounter == MAX_TEXT_COUNT - 1u)
	{
		return;
	}

	transformationIndexCounter++;
}

bool celestia::TextRender::isActive()
{
	return active;
}
