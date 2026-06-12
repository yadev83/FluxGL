#include <fluxgl/core/app.h>
#include <fluxgl/core/directory_storage_provider.h>
#include <fluxgl/core/package_storage_provider.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>
#include <fluxgl/core/scene_manager.h>
#include <fluxgl/audio/audio_engine.h>
#include <fluxgl/graphics/debug_renderer.h>
#include <fluxgl/assets/resource_manager.h>

#include <iostream>
#include <sstream>

namespace fluxgl {
    #if FLUXGL_USE_ASSET_PACKS
        App::App(int width, int height, const char* title, VFSSettings vfsSettings) : m_window(width, height, title), m_vfs(new PackageStorageProvider(vfsSettings.root, vfsSettings.recursive)) {
            m_window.bindApp(*this);
        }
    #else
        App::App(int width, int height, const char* title, VFSSettings vfsSettings) : m_window(width, height, title), m_vfs(new DirectoryStorageProvider()) {
            m_window.bindApp(*this);
        }
    #endif

    void App::run() {
        try {
            AppContext ctx { m_window, m_vfs, m_inputManager, m_resourceManager, m_prefabLoader };

            // Initialize subsystems
            SceneManager::get().setContext(&ctx);
            AudioEngine::get().init();

            // Debug only subsystems
            #if FLUXGL_DEBUG
                DebugRenderer::enable();
            #endif

            float lastTime = 0.0f;
            while (!m_window.shouldClose()) {
                float currentTime = static_cast<float>(glfwGetTime());
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                m_inputManager.beginFrame();
                m_window.pollEvents();

                SceneManager::get().update(deltaTime);

                m_window.swapBuffers();
            }

            AudioEngine::get().shutdown();
        } catch (const fluxgl::Error& error) {
            std::stringstream oss;
            oss << "Error: " << error.code << " - " << error.message;
            FLUXGL_LOG_ERROR(oss.str());
            exit(1);
        } catch (const std::exception& e) {
            FLUXGL_LOG_ERROR("std::exception: " + std::string(e.what()));
            exit(1);
        } catch (...) {
            FLUXGL_LOG_ERROR("Unknown error occurred");
            exit(1);
        }
    }

    fluxgl::InputManager& App::getInput() { 
        return m_inputManager; 
    }

    const fluxgl::InputManager& App::getInput() const { 
        return m_inputManager;
    }

    fluxgl::Window& App::getWindow() { 
        return m_window; 
    }

    const fluxgl::Window& App::getWindow() const { 
        return m_window; 
    }

    fluxgl::ResourceManager& App::getResourceManager() {
        return m_resourceManager;
    }

    const fluxgl::ResourceManager& App::getResourceManager() const {
        return m_resourceManager;
    }
}