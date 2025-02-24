#include "TextRender.h"
#include "backend/vulkanAPI/core/Buffer.h"
#include "backend/vulkanAPI/core/Image.h"
#include "backend/vulkanAPI/core/Pipeline.h"
#include "backend/vulkanAPI/core/ShaderObject.h"
#include "backend/vulkanAPI/resources/FontReader.h"
#include "backend/vulkanAPI/core/Device.h"
#include "backend/vulkanAPI/core/SwapChain.h"

#define MAIN_BUFFER 0

celestia::TextRender::TextRender(Render& render, uint32_t maxTextObjects, uint32_t maxCharsPerBatch)
	: render(render),
	descriptors(std::make_unique<DescriptorFactory>()),
	bufferSize(sizeof(Vec2Aligned)* maxTextObjects),
	set(),
	MAX_TEXT_COUNT(maxTextObjects),
	MAX_VERTEX_COUNT_PER_BATCH(maxCharsPerBatch*4u),
	MAX_INDEX_COUNT_PER_BATCH(maxCharsPerBatch * 4u*6u)
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

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		storageBuffer[i] = buffer::createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	}
	
	RawMesh tempMesh;
	tempMesh.vertices.resize(MAX_VERTEX_COUNT_PER_BATCH);
	tempMesh.indices.resize(MAX_INDEX_COUNT_PER_BATCH);


	int offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT_PER_BATCH; i += 6)
	{
		tempMesh.indices[static_cast<size_t>(i + 0)] = offset;
		tempMesh.indices[static_cast<size_t>(i + 1)] = 1 + offset;
		tempMesh.indices[static_cast<size_t>(i + 2)] = 2 + offset;

		tempMesh.indices[static_cast<size_t>(i + 3)] = 2 + offset;
		tempMesh.indices[static_cast<size_t>(i + 4)] = 3 + offset;
		tempMesh.indices[static_cast<size_t>(i + 5)] = offset;

		offset += 4;
	}

	info.descriptors = set;
	//info.material = &defaultMaterial;
	info.mesh = buffer::createMesh(tempMesh);
	
	descriptors->createDescriptor();
	descriptors->addBinding(0, DescriptorType::IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT);
	descriptors->addBinding(1, DescriptorType::STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
	descriptors->build(info.descriptors, info.layout);

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
	pipeline.createScissors({ 0,0 }, render.swapChain->extent);
	pipeline.createViewport({ 0,0 }, { static_cast<float>(render.swapChain->extent.width),static_cast<float>(render.swapChain->extent.height) });

	const VkVertexInputBindingDescription bindingDescription = utils::createBindingDescription(); // create using default values.
	utils::CustomVertexInputAttributeDescriptionFactory attributeDescriptions;
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));
	attributeDescriptions.pushDescription(0, VK_FORMAT_R32_UINT, offsetof(Vertex, texIndex));

	pipeline.createVertexInputStateCreateInfo(attributeDescriptions, bindingDescription, 1);

	info.material = pipeline.createPipeline(shader, DrawingMode::TRIANGLE, &info.layout, render.swapChain->getRenderPass());
}

celestia::TextRender::~TextRender()
{
	delete info.mesh;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
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
	//jos batch lista on eri kuin edellisen framen lista eli...
	//joku teksti on muuttunut, poistunut tai lisätty.
	//silloin päivitetään vertex buffer sekä storage buffer.
	//jos vain storage bufferin arvoja on muutettu niin päivitetään vain se.
	//eli tällöin tekstit ovat pysyneet samoina, mutta niiden sijainti on vaihtunut.

	//storage buffer memory mapping (updating values)

	//if (!needsUpdate)
	//{
		//return;
	//}

	//needsUpdate = false;
	 
	size_t transformationSize = transformationIndexCounter * sizeof(Vec2Aligned);

	void* data;
	vkMapMemory(Device::context.device,
		storageBuffer[render.currentFrame].memory,
		0,
		transformationSize,
		0,
		&data
	);
	std::memcpy(data, transformationBuffer.data(), transformationSize);
	vkUnmapMemory(Device::context.device, storageBuffer[render.currentFrame].memory);

	size_t vertexSize = vertexCount * sizeof(Vertex);
	buffer::updateBuffer(info.mesh->vertexBuffer, 0, vertexSize, glyphBuffer.data());
}

void celestia::TextRender::flush()
{
	info.amountToDraw = indexCount;
	render.drawNew(info);
}

void celestia::TextRender::drawText(const std::vector<Vertex>& vertices, const int size, const Vec2& position, const Font_t& font, bool dirty, const int id)
{
	if (!needsUpdate)
	{
		needsUpdate = dirty; // TODO: Finish this one.
	}
	

	// TODO: Tee tämä hassu hassu loppuun myös.
	if (currentTexturePtr != &font.texture)
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

		for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			//descriptors->updateBuffer(storageBuffer[i].buffer, bufferSize, info.descriptors[i]);
			//descriptors->updateTexture(currentTexturePtr->imageView, render.image->textureSampler, info.descriptors[i]);
			//descriptors->updateSets();

			descriptors->updateTexture(&currentTexturePtr->imageView, render.image->textureSampler, 0, 1, info.descriptors[i]);
			descriptors->updateBuffer(&storageBuffer[i].buffer, bufferSize, 1, 1, info.descriptors[i]);
			descriptors->updateSets();
		}
	}
	
	indexCount += size * 6;

	if (indexCount >= MAX_INDEX_COUNT_PER_BATCH)
	{
		end();
		flush();
		begin();
		indexCount += size * 6;
	}

	transformationBuffer[transformationIndexCounter].vec = position;

	for (const Vertex& vertex : vertices)
	{
		glyphBuffer[vertexCount] = vertex;
		glyphBuffer[vertexCount].texIndex = transformationIndexCounter;
		vertexCount++;
	}

	if (transformationIndexCounter == MAX_TEXT_COUNT - 1u)
	{
		return;
	}

	transformationIndexCounter++;
}

bool celestia::TextRender::isActive()
{
	return active;
}
