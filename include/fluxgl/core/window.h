#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "error.h"

namespace fluxgl {
    class Window {
        private:
            GLFWwindow* m_window = nullptr;

        public:
            Window(int width, int height, const char* title);
            ~Window();
    
            void swapBuffers();
            void pollEvents();
            bool shouldClose() const;
    };
}