#include "Player.h"

celestia::Player::Player()
	: Camera(PLAYER_POSITION,-90.f,0.f)
{
}

void celestia::Player::updatePos(GLFWwindow* window, float dt)
{
	float velocity = PLAYER_SPEED * dt;

	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
	{
		moveForward(velocity);
	}

	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
	{
		moveBack(velocity);
	}

	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
	{
		moveRight(velocity);
	}

	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
	{
		moveLeft(velocity);
	}

	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
	{
		moveUp(velocity);
	}

	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
	{
		moveDown(velocity);
	}

	updateMouse(window);

	if (prevMouseX)
	{
		rotateYaw(prevMouseX * MOUSE_SENSITIVITY);
	}
	if (prevMouseY)
	{
		rotatePitch(prevMouseY * MOUSE_SENSITIVITY);
	}

	update();
	
}

glm::mat4 celestia::Player::getViewMatrix()
{
	return getView();
}

glm::mat4 celestia::Player::getProjectionMatrix()
{
	return getProjection();
}


void celestia::Player::updateMouse(GLFWwindow* window)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state != GLFW_PRESS)
	{
		return;
	}

	glfwGetCursorPos(window, &currentMouseX, &currentMouseY);

	prevMouseX = currentMouseX - prevMouseX;
	prevMouseY = currentMouseY - prevMouseY;
}
