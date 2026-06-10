#include <fluxgl/fluxgl.h>
#include <iostream>
#include <sstream>

class Sprites : public fluxgl::Scene {
    fluxgl::Entity sprite;
    fluxgl::Entity camera;

    public:
        void onLoad() override {
            registerSystem<fluxgl::RenderSystem>();

            context->resourceManager.addShader("sprite_unlit", fluxgl::Shader::spriteUnlit());
            context->resourceManager.addTexture("awesomeface", fluxgl::Texture::loadFromMemory(context->vfs.read("assets/textures/awesomeface.png")));
        }

        void onInit() override {
            sprite = createEntity();
            sprite.addComponent<fluxgl::Transform>();
            fluxgl::Sprite& spriteComponent = sprite.addComponent<fluxgl::Sprite>();
            spriteComponent.shader = "sprite_unlit";
            spriteComponent.texture = "awesomeface";
            spriteComponent.size = {1.0f, 1.0f};

            camera = createEntity();
            camera.addComponent<fluxgl::Camera>();
            auto& cameraTransform = camera.addComponent<fluxgl::Transform>();
            cameraTransform.position = {0.0f, 0.0f, 3.0f};
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }
        }
};

int main() {
    fluxgl::App app(800, 600, "Sprites Example");

    fluxgl::SceneManager::get().registerScene<Sprites>("Sprites");
    app.run();

    return 0;
}