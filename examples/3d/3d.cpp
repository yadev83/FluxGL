#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <vector>

#define MAX_ENTITIES 10

class ThreeDimensionsExample : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        std::vector<fluxgl::Renderable> entities;
        fluxgl::Camera camera;

    protected:
        void onInit() override {           
            for(int i = 0; i < MAX_ENTITIES; i++) {
                entities.emplace_back();
                auto& entity = entities.back();
                
                bool isEven = i % 2 == 0;

                entity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
                entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
                if(isEven) entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
                
                entity.mesh = isEven ? fluxgl::Mesh::sphere() : fluxgl::Mesh::cube();
                // Randomize position
                entity.transform.position = glm::vec3((float)rand() / (float)RAND_MAX * 6.0f - 3.0f, (float)rand() / (float)RAND_MAX * 6.0f - 3.0f, (float)rand() / (float)RAND_MAX * 6.0f - 3.0f);
                //entity.transform.scale = glm::vec3((float)rand() / (float)RAND_MAX * 2.0f + 0.5f, (float)rand() / (float)RAND_MAX * 2.0f + 0.5f, (float)rand() / (float)RAND_MAX * 2.0f + 0.5f);
            }
            
            camera.transform.position = {0.0f, 0.0f, 10.0f};
        }

        void onUpdate(float deltaTime) override {
            if(getInput().isKeyPressed(GLFW_KEY_ESCAPE)) getWindow().isMouseLocked() ? getWindow().setMouseLocked(false) : getWindow().quit();
            if(getInput().isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) getWindow().setMouseLocked(true);

            float velocity = 5.0f * deltaTime;
            if(getInput().isKeyDown(GLFW_KEY_W)) camera.transform.position += camera.transform.front() * velocity;
            if(getInput().isKeyDown(GLFW_KEY_S)) camera.transform.position -= camera.transform.front() * velocity;
            if(getInput().isKeyDown(GLFW_KEY_A)) camera.transform.position -= camera.transform.right() * velocity;
            if(getInput().isKeyDown(GLFW_KEY_D)) camera.transform.position += camera.transform.right() * velocity;

            if(getWindow().isMouseLocked()) {
                static double lastX = getInput().getMouseX(), lastY = getInput().getMouseY();
                double offsetX = getInput().getMouseX() - lastX;
                double offsetY = lastY - getInput().getMouseY();
                lastX = getInput().getMouseX();
                lastY = getInput().getMouseY();
                float sensitivity = 0.1f;

                camera.transform.rotation.y += offsetX * sensitivity;
                camera.transform.rotation.x += offsetY * sensitivity;

                if(camera.transform.rotation.x > 89.0f) camera.transform.rotation.x = 89.0f;
                if(camera.transform.rotation.x < -89.0f) camera.transform.rotation.x = -89.0f;
            }

            for(fluxgl::Renderable& entity : entities) {
                entity.transform.rotation.x += 20.0f * deltaTime; // Rotate around X-axis
                entity.transform.rotation.z += 20.0f * deltaTime; // Rotate around Z-axis
            }
        }

        void onRender() override {
            fluxgl::Renderer::clear();
            for(fluxgl::Renderable& entity : entities) fluxgl::Renderer::draw(entity, &camera);
        }
};

int main() {
    ThreeDimensionsExample app(800, 600, "3D Example");
    app.run();

    return 0;
}