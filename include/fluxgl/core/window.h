#pragma once

#include <fluxgl/input/input_manager.h>
#include "error.h"

struct GLFWwindow;

namespace fluxgl {
    class App;

    class Window {
        private:
            GLFWwindow* m_window = nullptr;
            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
            static void errorCallback(int error, const char* description);

            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
            static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
            static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        public:
            Window(int width, int height, const char* title);
            ~Window();
    
            void swapBuffers();
            void pollEvents();
            void bindApp(App& app);
            bool shouldClose() const;
            bool isMouseLocked() const;

            void setMouseLocked(bool value);
            void quit();
    };
}