#include "BatchRender.h"
#include "vulkanAPI/Buffer.h"
/*
constexpr int MAX_QUAD_COUNT = 200;
constexpr int MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
constexpr int MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
const int MAX_TEXTURES = 16;

//init vertex && index buffers
celestia::BatchRender::BatchRender(Buffer& buffer)
{
	quadBuffer = new BatchVertex[MAX_QUAD_COUNT];

	RawMesh tempMesh;
	tempMesh.vertices.resize(MAX_VERTEX_COUNT);
	tempMesh.indices.resize(MAX_INDEX_COUNT);

	uint32_t offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
	{
		tempMesh.indices[i + 0] =     offset;
		tempMesh.indices[i + 1] = 1 + offset;
		tempMesh.indices[i + 2] = 2 + offset;

		tempMesh.indices[i + 3] = 2 + offset;
		tempMesh.indices[i + 4] = 3 + offset;
		tempMesh.indices[i + 5] =     offset;

		offset += 4;
	}
	
	buffer.createMesh(tempMesh, mesh);
	
	WHITE TEXTURE 1x1 tähä
	
}

//clean everything
celestia::BatchRender::~BatchRender()
{
	delete[] quadBuffer;
}

void celestia::BatchRender::beginBatch()
{
	quadBufferPtr = quadBuffer;
}

//the real draw command in nutshell...
void celestia::BatchRender::endBatch(Buffer* buffer)
{
	uint32_t size = (uint8_t)quadBufferPtr - (uint8_t)quadBuffer;
	buffer->updateBatchBuffer(mesh.vertexBuffer, 0, size, quadBuffer);

}

//the real draw command in nutshell...
void celestia::BatchRender::flush()
{
	
	//for (uint32_t i = 0; i< textureSlotIndex; i++)
	//{
	//	glBindTextureUnit(i,textureSlots[i]);
	//}
	
	//bind vertexArray
	// drawIndexed()
	indexCount = 0;
	textureSlotIndex = 1;
}

void celestia::BatchRender::drawQuad(const Vec2& position, const Vec2& size, const Vec4& color)
{
	if (indexCount >= MAX_INDEX_COUNT)
	{
		endBatch(nullptr);
		flush();
		beginBatch();
	}

	float textureIndex = 0.f;

	quadBufferPtr->position = position;
	quadBufferPtr->texCoord = { 0.f,0.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->position = { position.x + size.x, position.y};
	quadBufferPtr->texCoord = { 1.f,0.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->position = { position.x + size.x, position.y + size.y };
	quadBufferPtr->texCoord = { 1.f,1.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->position = { position.x, position.y + size.y };
	quadBufferPtr->texCoord = { 0.f,1.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	indexCount += 6;
}

void celestia::BatchRender::drawQuad(const Vec2& position, const Vec2& size, const uint32_t textureID)
{
	if (indexCount >= MAX_INDEX_COUNT || textureSlotIndex >= MAX_TEXTURES)
	{
		endBatch(nullptr);
		flush();
		beginBatch();
	}

	Color color = { 1.f,1.f,1.f,1.f };
	float textureIndex = 0.f;

	for (uint32_t index = 1; index < textureSlotIndex; index++)
	{
		if (textureSlots[index] == textureID)
		{
			textureIndex = static_cast<float>(index);
			break;
		}
	}

	if (textureIndex == 0.f)
	{
		textureIndex = static_cast<float>(textureSlotIndex);
		textureSlotIndex++;
	}
	

	quadBufferPtr->position = position;
	quadBufferPtr->texCoord = { 0.f,0.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->position = { position.x + size.x, position.y };
	quadBufferPtr->texCoord = { 1.f,0.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->position = { position.x + size.x, position.y + size.y };
	quadBufferPtr->texCoord = { 1.f,1.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	quadBufferPtr->position = { position.x, position.y + size.y };
	quadBufferPtr->texCoord = { 0.f,1.f };
	quadBufferPtr->color = color;
	quadBufferPtr->texIndex = textureIndex;
	quadBufferPtr++;

	indexCount += 6;
}
*/