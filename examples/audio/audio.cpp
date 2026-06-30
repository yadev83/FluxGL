#include <iostream>
#include <sstream>

#include <fluxgl/fluxgl.h>
#include <miniaudio.h>

class Audio : public fluxgl::Scene {
    private:
        fluxgl::Entity entity;

        ma_engine audioEngine;
        ma_audio_buffer audioBuffer;
        ma_sound soundInstance;

    public:
        void onLoad() override {
            registerSystem<fluxgl::RenderSystem>();

            auto& resources = context->resourceManager;
            auto& vfs = context->vfs;

            resources.addResource<fluxgl::Sound>("solitude", fluxgl::Sound::loadFromMemory(vfs.read("assets/bgm/solitude.wav")));
        }

        void onInit() override {
            entity = createEntity();

            if (ma_engine_init(nullptr, &audioEngine) != MA_SUCCESS) {
                throw fluxgl::Error{fluxgl::ErrorCode::AudioEngineError, "failed to create audio engine\n"};
            }

            auto sound = getContext().resourceManager.getResource<fluxgl::Sound>("solitude");

            ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
                ma_format_f32,
                sound->getChannels(),
                sound->getFrameCount(),
                sound->getSamples().data(),
                nullptr
            );

            if(ma_audio_buffer_init(&bufferConfig, &audioBuffer) != MA_SUCCESS) {
                throw fluxgl::Error{fluxgl::ErrorCode::AudioEngineError, "failed to load audio buffer"};
            }

            if(ma_sound_init_from_data_source(
                &audioEngine,
                &audioBuffer,
                0,
                nullptr,
                &soundInstance) != MA_SUCCESS
            ) {
                throw fluxgl::Error{fluxgl::ErrorCode::AudioEngineError, "failed to init sound instance from audio buffer"};
            }

            ma_sound_start(&soundInstance);
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