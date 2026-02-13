#include <fluxgl/core/window.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>

namespace fluxgl {
    Window::Window(int width, int height, const char* title) {
        // Logger initialisation (based on build type)
        #if FLUXGL_DEBUG
            fluxgl::Logger::init(fluxgl::LogLevel::Trace);
        #else
            fluxgl::Logger::init(fluxgl::LogLevel::Info);
        #endif

        FLUXGL_LOG_INFO("Creating GLFW window...");

        if(!glfwInit()) {
            throw Error{ErrorCode::WindowCreationFailed, "Failed to initialize GLFW"};
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if(!m_window) {
            glfwTerminate();
            throw Error{ErrorCode::WindowCreationFailed, "Failed to create GLFW window"};
        }
        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(m_window);
            glfwTerminate();
            throw Error{ErrorCode::OpenGLError, "Failed to initialize GLAD"};
        }
        glViewport(0, 0, width, height);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        FLUXGL_LOG_INFO("GLFW window created successfully");
    }

    Window::~Window() {
        if(m_window) {
            FLUXGL_LOG_INFO("Destroying GLFW window...");
            glfwDestroyWindow(m_window);
            glfwTerminate();
        } else { 
            FLUXGL_LOG_ERROR("Attempted to destroy window, but m_window is nullptr"); 
        }
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_window);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }
}