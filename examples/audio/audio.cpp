#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>

class Audio : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;

    public:
        void onInit() override {
            fluxgl::AudioEngine& audioEngine = fluxgl::AudioEngine::get();
            audioEngine.init();
            fluxgl::SoundID sound = audioEngine.loadSound("assets/bgm/solitude.wav", fluxgl::SoundType::BGM);

            audioEngine.play(sound);
        }

        void onUpdate(float deltaTime) override {
            if(context->inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
                context->window.setWindowShouldClose();
            }

            fluxgl::AudioEngine& audioEngine = fluxgl::AudioEngine::get();
            audioEngine.update();
        }
};

int main() {
    fluxgl::App app(800, 600, "Audio FluxGL");

    fluxgl::SceneManager::get().registerScene<Audio>("Audio");
    app.run();

    return 0;
}