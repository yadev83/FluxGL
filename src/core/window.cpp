#include <fluxgl/core/window.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>

namespace fluxgl {
    Window::Window(int width, int height, const char* title) {
        FLUXGL_LOG_INFO("Creating GLFW window");

        if(!glfwInit()) {
            throw Error{ErrorCode::WindowCreationFailed, "Failed to initialize GLFW"};
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        mWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if(!mWindow) {
            glfwTerminate();
            throw Error{ErrorCode::WindowCreationFailed, "Failed to create GLFW window"};
        }
        glfwMakeContextCurrent(mWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(mWindow);
            glfwTerminate();
            throw Error{ErrorCode::OpenGLError, "Failed to initialize GLAD"};
        }
        glViewport(0, 0, width, height);

        FLUXGL_LOG_INFO("GLFW window created successfully");
    }

    Window::~Window() {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(mWindow);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(mWindow);
    }
}