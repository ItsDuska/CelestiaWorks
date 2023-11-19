#pragma once
#include "Camera.h"
#include "window/Window.h"

namespace celestia
{
	struct KeyMappings {
		int moveLeft = GLFW_KEY_A;
		int moveRight = GLFW_KEY_D;
		int moveForward = GLFW_KEY_W;
		int moveBackward = GLFW_KEY_S;
		int moveUp = GLFW_KEY_SPACE;
		int moveDown = GLFW_KEY_LEFT_SHIFT;
		int lookLeft = GLFW_KEY_LEFT;
		int lookRight = GLFW_KEY_RIGHT;
		int lookUp = GLFW_KEY_UP;
		int lookDown = GLFW_KEY_DOWN;
	};



	class Player : protected Camera
	{
	public:
		Player();

		void updatePos(GLFWwindow* window, float dt);
		//get matrix func

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
	private:
		void updateMouse(GLFWwindow* window);
		double prevMouseX = 0.0;
		double prevMouseY = 0.0;
		double currentMouseX;
		double currentMouseY;
		KeyMappings keys{};
	};
}


