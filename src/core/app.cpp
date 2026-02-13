#include <fluxgl/core/app.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>

#include <iostream>
#include <sstream>

namespace fluxgl {
    App::App(int width, int height, const char *title) : m_window(width, height, title) { }

    void App::run() {
        try {
            onInit();

            float lastTime = 0.0f;
            while (!m_window.shouldClose()) {
                float currentTime = static_cast<float>(glfwGetTime());
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

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
}