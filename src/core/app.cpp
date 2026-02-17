#include <fluxgl/core/app.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>

#include <iostream>
#include <sstream>

namespace fluxgl {
    App::App(int width, int height, const char *title) : m_window(width, height, title) {
        m_window.bindApp(*this);
    }

    void App::run() {
        try {
            onInit();

            float lastTime = 0.0f;
            while (!m_window.shouldClose()) {
                float currentTime = static_cast<float>(glfwGetTime());
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                m_inputManager.beginFrame();
                m_window.pollEvents();

                onUpdate(deltaTime);
                onRender();

                m_window.swapBuffers();
            }

            onShutdown();
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
}