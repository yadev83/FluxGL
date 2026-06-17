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

            auto& resources = context->resourceManager;
            auto& vfs = context->vfs;

            resources.addSound(
                "solitude", 
                fluxgl::Sound::loadFromMemory(
                    vfs.read("assets/bgm/solitude.wav"), 
                    fluxgl::AudioEngine::getEngine()
                )
            );
        }

        void onInit() override {
            entity = createEntity();
            auto& audioSource = entity.addComponent<fluxgl::AudioSource>();
            audioSource.sound = "solitude";
            audioSource.shouldPlay = true;
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            if(context->inputManager.isKeyPressed(GLFW_KEY_DOWN)) {
                fluxgl::AudioEngine::get().setMasterVolume(fluxgl::AudioEngine::get().getMasterVolume() - 0.1f);
            }

            if(context->inputManager.isKeyPressed(GLFW_KEY_UP)) {
                fluxgl::AudioEngine::get().setMasterVolume(fluxgl::AudioEngine::get().getMasterVolume() + 0.1f);
            }
        }
};

int main() {
    fluxgl::App app(800, 600, "Audio FluxGL");

    fluxgl::SceneManager::get().registerScene<Audio>("Audio");
    app.run();

    return 0;
}