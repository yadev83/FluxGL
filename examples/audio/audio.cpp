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

            auto& resources = getContext().resourceManager;
            auto& vfs = getContext().vfs;

            resources.addResource<fluxgl::Sound>("solitude", fluxgl::Sound::loadFromMemory(vfs.read("assets/bgm/solitude.wav")));
        }

        void onInit() override {
            entity = createEntity();

            auto sound = getContext().resourceManager.getResource<fluxgl::Sound>("solitude");
            soundHandle = getContext().audioEngine.play(sound);
        }

        void onUpdate(float deltaTime) override {
            if(getContext().inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                getContext().window.setWindowShouldClose();
            }

            if(getContext().inputManager.isKeyPressed(GLFW_KEY_P)) {
                if(getContext().audioEngine.isPlaying(soundHandle)) getContext().audioEngine.pause(soundHandle);
                else getContext().audioEngine.resume(soundHandle);
            }
        }
};

int main() {
    fluxgl::App app(800, 600, "Audio FluxGL");

    fluxgl::SceneManager::get().registerScene<Audio>("Audio");
    app.run();

    return 0;
}