#pragma once

// bit goofy but we go with it xd.
#ifdef CELESTIA_DEBUG
#define ENABLE_VALIDATION_LAYER
#endif // DEBUG

constexpr int MAX_FRAMES_IN_FLIGHT = 2;
constexpr int NUMBER_OF_TEXTURE_IN_SHADER = 5;
constexpr int NUMBER_OF_UNIFORM_BUFFERS = 1;

