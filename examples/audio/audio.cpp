#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class Audio : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;

    public:
        void onLoad() override {
            registerSystem<fluxgl::RenderSystem>();
            registerSystem<fluxgl::AudioSystem>();
        }

        void onInit() override {
            entity = createEntity();
            auto& audioSource = entity.addComponent<fluxgl::AudioSource>();
            audioSource.sound.path = "assets/bgm/solitude.wav";
            audioSource.sound.type = fluxgl::SoundType::BGM;
            audioSource.loop = true;
            audioSource.shouldPlay = true;
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }
        }
};

int main() {
    fluxgl::App app(800, 600, "Audio FluxGL");

    fluxgl::SceneManager::get().registerScene<Audio>("Audio");
    app.run();

    return 0;
}