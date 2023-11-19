#pragma once
#include "window/Window.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include "gameObjects/GameObjectTypes.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace celestia {

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
    
    class Keyboard {
    public:
        void move(GLFWwindow* window, float dt,TransformComponent &camera );

    private:
        KeyMappings keys{};
        float moveSpeed{ 1.5f };
        float lookSpeed{ 1.5f };

    };
}


