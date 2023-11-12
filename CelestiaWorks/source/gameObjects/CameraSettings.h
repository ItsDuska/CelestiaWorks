#pragma once
#include <glm/glm.hpp>
#include <cmath>

namespace celestia {
	const glm::vec2 WIN_RES = { 800,600 };

	const float ASPECT_RATIO = WIN_RES.x / WIN_RES.y;
	const float FOW_DEG = 50;
	const float V_FOV = glm::radians(FOW_DEG);
	const float H_FOV = 2 * std::atan(std::tan(V_FOV * 0.5f) * ASPECT_RATIO); // horizontal FOV

	const float NEAR = 0.1;
	const float FAR = 2000.0;
	const float PITCH_MAX = glm::radians(89.f);

	const float PLAYER_SPEED = 0.5f; // 0.005f
	const float PLAYER_ROTATION_SPEED = 0.003f;
	const glm::vec3 PLAYER_POSITION = { 0.f,0.f,0.f };
	const float MOUSE_SENSITIVITY = 0.002f;
}
