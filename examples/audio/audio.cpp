#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>
#include <miniaudio.h>

class Audio : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;
        fluxgl::AudioVoiceHandle soundHandle;

    public:
        void onLoad() override {
            registerSystem<fluxgl::RenderSystem>();
            registerSystem<fluxgl::AudioSystem>();

            auto& resources = getContext().resourceManager;
            auto& vfs = getContext().vfs;

            resources.addResource<fluxgl::Sound>("solitude", fluxgl::Sound::loadFromMemory(vfs.read("assets/bgm/solitude.wav")));
        }

        void onInit() override {
            entity = createEntity();

            auto& source = entity.addComponent<fluxgl::AudioSource>();
            source.sound = "solitude";
        }

        void onUpdate(float deltaTime) override {
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                getContext().window.setWindowShouldClose();
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_P)) {
                auto& audioSource = entity.getComponent<fluxgl::AudioSource>();
                audioSource.shouldPause = true;
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_O)) {
                auto& audioSource = entity.getComponent<fluxgl::AudioSource>();
                audioSource.shouldPlay = true;
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_S)) {
                auto& audioSource = entity.getComponent<fluxgl::AudioSource>();
                audioSource.shouldStop = true;
            }
        }
};

int main() {
    fluxgl::App app(800, 600, "Audio FluxGL");

    fluxgl::SceneManager::get().registerScene<Audio>("Audio");
    app.run();

    return 0;
}