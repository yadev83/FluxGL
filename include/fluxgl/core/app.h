#pragma once

#include <fluxgl/input/input_manager.h>
#include "window.h"

namespace fluxgl {
    class App {
        private:
            fluxgl::Window m_window;
            fluxgl::InputManager m_inputManager;

        protected:
            virtual void onInit() {}
            virtual void onUpdate(float deltaTime) {}
            virtual void onRender() {}
            virtual void onShutdown() {}

        public:
            App(int width, int height, const char *title);
            virtual ~App() = default;

            fluxgl::InputManager& getInput();
            const fluxgl::InputManager& getInput() const;
            fluxgl::Window& getWindow();
            const fluxgl::Window& getWindow() const;

            void run();
    };
}