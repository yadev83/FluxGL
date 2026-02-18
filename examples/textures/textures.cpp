#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class TextureExample : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        fluxgl::Renderable entity;

    protected:
        void onInit() override {
            entity.material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/texture.vert", "assets/shaders/texture.frag");
            entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            entity.material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            
            entity.mesh = fluxgl::Mesh::quad();
        }

        void onUpdate(float deltaTime) override {
            if(getInput().isKeyPressed(GLFW_KEY_ESCAPE)) {
                getWindow().quit();
            }
        }

        void onRender() override {
            fluxgl::Renderer::clear();
            fluxgl::Renderer::draw(entity);
        }
};

int main() {
    TextureExample app(800, 600, "Texture Example");
    app.run();

    return 0;
}