#pragma once

#include "window.h"

namespace fluxgl {
    class App {
        private:
            fluxgl::Window m_window;

        protected:
            virtual void onInit() {}
            virtual void onUpdate(float deltaTime) {}
            virtual void onRender() {}
            virtual void onShutdown() {}

        public:
            App(int width, int height, const char *title);
            virtual ~App() = default;

            void run();
    };
}