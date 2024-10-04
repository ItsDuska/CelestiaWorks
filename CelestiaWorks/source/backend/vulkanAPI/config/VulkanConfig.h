#pragma once

#ifdef CELESTIA_DEBUG
#define ENABLE_VALIDATION_LAYER
#endif // DEBUG

constexpr int MAX_FRAMES_IN_FLIGHT = 2;
constexpr int NUMBER_OF_TEXTURE_IN_SHADER = 5;
constexpr int NUMBER_OF_UNIFORM_BUFFERS = 1;
//constexpr int MAX_QUAD_COUNT = 110;

//constexpr int MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
//constexpr int MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
//constexpr int MAX_TEXT_COUNT = 10;
