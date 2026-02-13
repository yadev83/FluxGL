#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class TextureExample : public fluxgl::App {
    public: using fluxgl::App::App; // Inherit constructors

    private:
        fluxgl::Mesh quad;
        fluxgl::Material material;

    protected:
        void onInit() override {
            material.shader = fluxgl::Shader::loadFromFiles("assets/shaders/texture.vert", "assets/shaders/texture.frag");
            material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/container.jpg"));
            material.albedoTextures.push_back(fluxgl::Texture::loadFromFile("assets/textures/awesomeface.png"));
            
            quad = fluxgl::Mesh::quad();
        }

        void onRender() override {
            fluxgl::Renderer::clear();
            fluxgl::Renderer::draw(quad, material);
        }
};

int main() {
    TextureExample app(800, 600, "Texture Example");
    app.run();

    return 0;
}