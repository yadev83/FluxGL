#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fluxgl/core/window.h>
#include <fluxgl/core/error.h>
#include <fluxgl/core/log.h>
#include <fluxgl/core/app.h>

#include <fluxgl/graphics/renderer.h>

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

        // Setup callbacks
        glfwSetErrorCallback(errorCallback);
        glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetCursorPosCallback(m_window, cursorPositionCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, scrollCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwDestroyWindow(m_window);
            glfwTerminate();
            throw Error{ErrorCode::OpenGLError, "Failed to initialize GLAD"};
        }
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
        Renderer::setFramebufferSize(fbWidth, fbHeight);

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

    void Window::bindApp(App& app) {
        glfwSetWindowUserPointer(m_window, &app);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }
    
    void Window::setWindowShouldClose(bool value) {
        glfwSetWindowShouldClose(m_window, value);
    }

    bool Window::isMouseLocked() const {
        return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
    }

    void Window::setMouseLocked(bool value) {
        glfwSetInputMode(m_window, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    /** STATIC callbacks */
    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if(!app) return;

        Renderer::setFramebufferSize(width, height);
    }

    void Window::errorCallback(int error, const char* description) {
        FLUXGL_LOG_ERROR("GLFW error (" + std::to_string(error) + "): " + std::string(description));
    }

    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if(!app) return;

        if(action == GLFW_PRESS) {
            FLUXGL_LOG_TRACE("Pressed key: " + std::to_string(key));
            app->getInput().setKey(key, true);
        } else if(action == GLFW_RELEASE) {
            FLUXGL_LOG_TRACE("Released key: " + std::to_string(key));
            app->getInput().setKey(key, false);
        }
    }

    void Window::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if(!app) return;
        
        app->getInput().setMousePosition(xpos, ypos);
    }
    
    void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if(!app) return;

        if(action == GLFW_PRESS) {
            FLUXGL_LOG_TRACE("Pressed mouse button: " + std::to_string(button) + ": " + std::to_string(app->getInput().getMouseX()) + ", " + std::to_string(app->getInput().getMouseY()));
            app->getInput().setMouseButton(button, true);
        } else if(action == GLFW_RELEASE) {
            FLUXGL_LOG_TRACE("Released mouse button: " + std::to_string(button) + ": " + std::to_string(app->getInput().getMouseX()) + ", " + std::to_string(app->getInput().getMouseY()));
            app->getInput().setMouseButton(button, false);
        }
    }

    void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if(!app) return;

        FLUXGL_LOG_TRACE("Mouse scrolled: (" + std::to_string(xoffset) + ", " + std::to_string(yoffset) + ")");
        app->getInput().addScroll(xoffset, yoffset);
    }
}