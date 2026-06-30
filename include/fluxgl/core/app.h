#pragma once

#include <fluxgl/input/input_manager.h>
#include <fluxgl/audio/audio_engine.h>
#include <fluxgl/assets/resource_manager.h>
#include <fluxgl/assets/prefab_loader.h>
#include "window.h"
#include "virtual_file_system.h"

namespace fluxgl {
    struct VFSSettings {
        const char* root;
        bool recursive = false;
    };
    
    struct AppContext {
        fluxgl::Window& window;
        fluxgl::AudioEngine& audioEngine;
        fluxgl::VirtualFileSystem& vfs;

        fluxgl::InputManager& inputManager;
        fluxgl::ResourceManager& resourceManager;
        fluxgl::PrefabLoader& prefabLoader;
    };

    /**
     * @brief Main application class
     * 
     * The entry point of any FluxGL application should be there
     * It automatically starts an openGL window and sets up an input manager and the Audio Engine.
     * The main loop is then handled by the run() method
     */
    class App {
        private:
            fluxgl::Window m_window;
            fluxgl::AudioEngine m_audioEngine;
            fluxgl::VirtualFileSystem m_vfs;
            
            fluxgl::InputManager m_inputManager;
            fluxgl::ResourceManager m_resourceManager;
            fluxgl::PrefabLoader m_prefabLoader;

        public:
            App(int width, int height, const char* title, VFSSettings vfsSettings = {"assets.fgld", false});
            virtual ~App() = default;

            fluxgl::AudioEngine& getAudio();
            const fluxgl::AudioEngine& getAudio() const;
            fluxgl::InputManager& getInput();
            const fluxgl::InputManager& getInput() const;
            fluxgl::Window& getWindow();
            const fluxgl::Window& getWindow() const;
            fluxgl::ResourceManager& getResourceManager();
            const fluxgl::ResourceManager& getResourceManager() const;

            void run();
    };
}