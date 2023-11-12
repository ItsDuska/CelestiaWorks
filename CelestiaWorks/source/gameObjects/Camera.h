#pragma once
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <cassert>
#include <limits>
#include "CameraSettings.h"
#include <glm/gtx/transform.hpp>


namespace celestia {
    class Camera {
    public:
        Camera(glm::vec3 position, float yaw, float pitch);

        void update();

        void updateViewMatrix();

        void updateVectors();

        void rotatePitch(int deltaY);

        void rotateYaw(int deltaX);

        void moveLeft(float velocity);

        void moveRight(float velocity);

        void moveUp(float velocity);

        void moveDown(float velocity);

        void moveForward(float velocity);

        void moveBack(float velocity);

        //VANHA
        void setOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        void setViewDirection(
            glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
        void setViewTarget(
            glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

        const glm::mat4& getProjection() const { return projectionMatrix; }
        const glm::mat4& getView() const { return viewMatrix; }

    private:
        //glm::mat4 projectionMatrix{ glm::perspective(V_FOV, ASPECT_RATIO,NEAR,FAR) };
        glm::mat4 projectionMatrix{ 1.f };
        glm::mat4 viewMatrix{1.f};

        glm::vec3 position;
        float yaw;
        float pitch;

        // CONSTANTS???

        glm::vec3 up{0,1,0};
        glm::vec3 right{1, 0, 0};
        glm::vec3 forward{0, 1, -1};

    };
}  // namespace lve
