#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>

class ThreeDimensionsExample : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        fluxgl::Renderable entity;
        fluxgl::Camera camera;

    protected:
        void onInit() override {
            entity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            
            entity.mesh = fluxgl::Mesh::cube();

            entity.transform.scale = {0.5f, 0.5f, 0.5f};
            entity.transform.rotation = glm::vec3(-55.0f, 0.0f, 0.0f);

            camera.transform.position = {0.0f, 0.0f, 3.0f};
        }

        void onUpdate(float deltaTime) override {
            entity.transform.rotation.x += 20.0f * deltaTime; // Rotate around Z-axis
            entity.transform.rotation.z += 20.0f * deltaTime; // Rotate around Z-axis
        }

        void onRender() override {
            fluxgl::Renderer::clear();
            fluxgl::Renderer::draw(entity, &camera);
        }
};

int main() {
    ThreeDimensionsExample app(800, 600, "3D Example");
    app.run();

    return 0;
}