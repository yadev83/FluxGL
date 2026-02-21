#pragma once

#include <fluxgl/fluxgl.h>

class FirstPersonController : public fluxgl::Behavior {
    private:
        float speed = 5.0f;

    public:
        void onUpdate(float dt) override {
            auto& cameraTransform = getComponent<fluxgl::Transform>();
            auto& inputManager = getContext().inputManager;
            auto& window = getContext().window;

            float velocity = speed * dt;
            if(inputManager.isKeyDown(GLFW_KEY_W)) cameraTransform.position += cameraTransform.front() * velocity;
            if(inputManager.isKeyDown(GLFW_KEY_S)) cameraTransform.position -= cameraTransform.front() * velocity;
            if(inputManager.isKeyDown(GLFW_KEY_A)) cameraTransform.position -= cameraTransform.right() * velocity;
            if(inputManager.isKeyDown(GLFW_KEY_D)) cameraTransform.position += cameraTransform.right() * velocity;

            if(window.isMouseLocked()) {
                static double lastX = inputManager.getMouseX(), lastY = inputManager.getMouseY();
                double offsetX = inputManager.getMouseX() - lastX;
                double offsetY = lastY - inputManager.getMouseY();
                lastX = inputManager.getMouseX();
                lastY = inputManager.getMouseY();
                float sensitivity = 0.1f;

                cameraTransform.rotation.y += offsetX * sensitivity;
                cameraTransform.rotation.x += offsetY * sensitivity;

                if(cameraTransform.rotation.x > 89.0f) cameraTransform.rotation.x = 89.0f;
                if(cameraTransform.rotation.x < -89.0f) cameraTransform.rotation.x = -89.0f;
            }
        }
};