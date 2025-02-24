#include "BatchRender.h"
#include "backend/vulkanAPI/core/Buffer.h"
#include "backend/vulkanAPI/core/Descriptor.h"
#include "backend/vulkanAPI/core/Image.h"
#include "backend/vulkanAPI/core/Pipeline.h"
#include "backend/vulkanAPI/core/ShaderObject.h"
#include "backend/vulkanAPI/core/SwapChain.h"

#include <iostream>


celestia::BatchSpriteRender::BatchSpriteRender(Render& render, uint32_t maxTexturesInShader, uint32_t maxQuadsPerBatch)
	: render(render),
	MAX_TEXTURES_IN_SHADER(maxTexturesInShader),
	MAX_QUAD_COUNT(maxQuadsPerBatch),
	MAX_VERTEX_COUNT_PER_BATCH(maxQuadsPerBatch * 4u),
	MAX_INDEX_COUNT_PER_BATCH(maxQuadsPerBatch * 4u * 6u),
	descriptors(std::make_unique<DescriptorFactory>()),
	set()
{
	textureSlotIndex = 1;
	indexCount = 0;
	vertexCount = 0;
	textureSlots = {};

	quadBuffer.resize(MAX_VERTEX_COUNT_PER_BATCH);

	RawMesh tempMesh;
	tempMesh.vertices.resize(MAX_VERTEX_COUNT_PER_BATCH);
	tempMesh.indices.resize(MAX_INDEX_COUNT_PER_BATCH);

	uint32_t offset = 0;
	for (int i = 0; i < MAX_VERTEX_COUNT_PER_BATCH; i += 6)
	{
		tempMesh.indices[static_cast<size_t>(i + 0)] =     offset;
		tempMesh.indices[static_cast<size_t>(i + 1)] = 1 + offset;
		tempMesh.indices[static_cast<size_t>(i + 2)] = 2 + offset;

		tempMesh.indices[static_cast<size_t>(i + 3)] = 2 + offset;
		tempMesh.indices[static_cast<size_t>(i + 4)] = 3 + offset;
		tempMesh.indices[static_cast<size_t>(i + 5)] =     offset;

		offset += 4;
	}
	
	info.mesh = buffer::createMesh(tempMesh);
	info.descriptors = set;

	textures[0] = render.image->defaultTexture.imageView;

	descriptors->createDescriptor();
	descriptors->addBinding(0, DescriptorType::BINDLESS_IMAGE, VK_SHADER_STAGE_FRAGMENT_BIT, MAX_TEXTURES_IN_SHADER, MAX_TEXTURES_IN_SHADER);
	descriptors->build(info.descriptors, info.layout);

	ShaderObject shader;
	shader.loadShader(nullptr, ShaderType::VERTEX_SHADER, RenderGroup::SPRITE_BATCH, true);
	shader.loadShader(nullptr, ShaderType::FRAGMENT_SHADER, RenderGroup::SPRITE_BATCH, true);
	shader.createPushConstants<PUSH_CONSTANTS>(0, ShaderType::VERTEX_SHADER);

	PipelineOptions options{};
	options.blending = false;

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
	
	pipeline.createVertexInputStateCreateInfo(attributeDescriptions, bindingDescription,1);

	info.material = pipeline.createPipeline(shader, DrawingMode::TRIANGLE, &info.layout, render.swapChain->getRenderPass());
}

//clean everything
celestia::BatchSpriteRender::~BatchSpriteRender()
{
	delete info.mesh;
	//cleanUp();
}

void celestia::BatchSpriteRender::beginBatch()
{
	indexCount = 0;
	vertexCount = 0;
}

void celestia::BatchSpriteRender::endBatch()
{
	//vertex buffer updateing..
	size_t size = vertexCount * sizeof(Vertex);
	buffer::updateBuffer(info.mesh->vertexBuffer, 0, size, quadBuffer.data());
}

//the real draw command in nutshell...
void celestia::BatchSpriteRender::flush()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		descriptors->updateTexture(textures, render.image->textureSampler, 0, textureSlotIndex, info.descriptors[i]);
		descriptors->updateSets();
	}

	textureSlotIndex = 1;
	info.amountToDraw = indexCount;
	render.drawNew(info);
}

void celestia::BatchSpriteRender::drawQuad(const Vec2& position, const Vec2& size, const Vec3& color)
{
	if (indexCount >= MAX_VERTEX_COUNT_PER_BATCH)
	{
		endBatch();
		flush();
		beginBatch();
	}

	const uint32_t textureIndex = 0;

	quadBuffer[vertexCount].position = position;
	quadBuffer[vertexCount].texCoord = { 0.f,0.f };
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	quadBuffer[vertexCount].position = { position.x + size.x, position.y};
	quadBuffer[vertexCount].texCoord = { 1.f,0.f };
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	quadBuffer[vertexCount].position = { position.x + size.x, position.y + size.y };
	quadBuffer[vertexCount].texCoord = { 1.f,1.f };
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	quadBuffer[vertexCount].position = { position.x, position.y + size.y };
	quadBuffer[vertexCount].texCoord = { 0.f,1.f };
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	indexCount += 6;
}

void celestia::BatchSpriteRender::drawQuad(const VertexPositions* quad, const RawTexture* texture)
{
	if (quad == nullptr)
	{
		return;
	}

	if (indexCount >= MAX_INDEX_COUNT_PER_BATCH || textureSlotIndex >= NUMBER_OF_TEXTURE_IN_SHADER)
	{
		endBatch();
		flush();
		beginBatch();
	}

	uint32_t textureIndex = 0;

	for (int index = 1; index < textureSlotIndex; index++)
	{
		if (textureSlots[index] == (int)texture->textureID)
		{
			textureIndex = index;
			break;
		}
	}

	if (textureIndex == 0)
	{
		textureIndex = textureSlotIndex;
		textureSlots[textureSlotIndex] = texture->textureID;
		textures[textureSlotIndex] = texture->imageView;
		textureSlotIndex++;
	}

	const Vec3 color = { 1.f,1.f,1.f };

	quadBuffer[vertexCount].position = quad[0].position;
	quadBuffer[vertexCount].texCoord = quad[0].texCoord;
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	quadBuffer[vertexCount].position = quad[1].position;
	quadBuffer[vertexCount].texCoord = quad[1].texCoord;
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	quadBuffer[vertexCount].position = quad[2].position;
	quadBuffer[vertexCount].texCoord = quad[2].texCoord;
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	quadBuffer[vertexCount].position = quad[3].position;
	quadBuffer[vertexCount].texCoord = quad[3].texCoord;
	quadBuffer[vertexCount].color = color;
	quadBuffer[vertexCount].texIndex = textureIndex;
	vertexCount++;

	indexCount += 6;
}
