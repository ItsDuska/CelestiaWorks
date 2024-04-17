#include "BatchRender.h"
#include "backend/vulkanAPI/core/Buffer.h"
#include "backend/vulkanAPI/core/Descriptor.h"
#include "backend/vulkanAPI/core/Image.h"
#include "backend/vulkanAPI/core/Pipeline.h"

#include <iostream>


celestia::BatchSpriteRender::BatchSpriteRender(Render& render)
	: render(render)
{
	textureSlotIndex = 1;
	indexCount = 0;
	vertexCount = 0;
	textureSlots = {};

	quadBuffer.resize(MAX_VERTEX_COUNT);

	RawMesh tempMesh;
	tempMesh.vertices.resize(MAX_VERTEX_COUNT);
	tempMesh.indices.resize(MAX_INDEX_COUNT);

	uint32_t offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
	{
		tempMesh.indices[static_cast<size_t>(i + 0)] =     offset;
		tempMesh.indices[static_cast<size_t>(i + 1)] = 1 + offset;
		tempMesh.indices[static_cast<size_t>(i + 2)] = 2 + offset;

		tempMesh.indices[static_cast<size_t>(i + 3)] = 2 + offset;
		tempMesh.indices[static_cast<size_t>(i + 4)] = 3 + offset;
		tempMesh.indices[static_cast<size_t>(i + 5)] =     offset;

		offset += 4;
	}
	
	render.buffer->createMesh(tempMesh, info.mesh);
	
	textures[0] = render.image->defaultTexture.imageView;
	info.material = render.pipeline->getDefaultMaterial();
	info.descriptors = render.descriptor->getDefaultSpriteDescriptorSets();
}

//clean everything
celestia::BatchSpriteRender::~BatchSpriteRender()
{
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

	render.buffer->updateBatchBuffer(info.mesh.vertexBuffer, 0, size, quadBuffer.data());
}

//the real draw command in nutshell...
void celestia::BatchSpriteRender::flush()
{
	render.descriptor->updateDescriptorSetTextures(textures, render.image->textureSampler, 0, textureSlotIndex, render.currentFrame);
	render.descriptor->updateAllDescriptorSets();

	textureSlotIndex = 1;

	info.amountToDraw = indexCount;
	//render.draw(info.mesh,indexCount);
	render.drawNew(info);
}

void celestia::BatchSpriteRender::drawQuad(const Vec2& position, const Vec2& size, const Vec3& color)
{
	if (indexCount >= MAX_INDEX_COUNT)
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

	if (indexCount >= MAX_INDEX_COUNT || textureSlotIndex >= NUMBER_OF_TEXTURE_IN_SHADER)
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
