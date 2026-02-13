#include <fluxgl/fluxgl.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>

class TransformExample : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        fluxgl::Renderable entity;

    protected:
        void onInit() override {
            entity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
            entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            
            entity.mesh = fluxgl::Mesh::quad();

            entity.transform.scale = {0.5f, 0.5f, 0.5f};
            entity.transform.rotation = glm::vec3(0.0f, 0.0f, 45.0f);
        }

        void onRender() override {
            fluxgl::Renderer::clear();
            fluxgl::Renderer::draw(entity);
        }
};

int main() {
    TransformExample app(800, 600, "Transform Example");
    app.run();

    return 0;
}