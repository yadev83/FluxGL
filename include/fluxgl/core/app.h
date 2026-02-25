#pragma once

#include <fluxgl/input/input_manager.h>
#include "window.h"

namespace fluxgl {
    struct AppContext {
        fluxgl::Window& window;
        fluxgl::InputManager& inputManager;
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
            fluxgl::InputManager m_inputManager;

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