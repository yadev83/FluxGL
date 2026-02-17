#include <fluxgl/input/input_manager.h>
#include <fluxgl/core/log.h>
#include <string>

namespace fluxgl {
    void InputManager::beginFrame() {
        m_prevKeys = m_keys;
        m_prevMouseButtons = m_mouseButtons;

        m_prevMouseX = m_mouseX;
        m_prevMouseY = m_mouseY;

        m_scrollX = 0.0;
        m_scrollY = 0.0;
    }

    void InputManager::setKey(int key, bool value) {
        if(key < 0 || key > GLFW_KEY_LAST) return;
        m_keys[key] = value;
    }

    void InputManager::setMouseButton(int button, bool value) {
        if(button < 0 || button > GLFW_KEY_LAST) return;
        m_mouseButtons[button] = value;
    }

    void InputManager::setMousePosition(double x, double y) {
        m_mouseX = x;
        m_mouseY = y;
    }

    void InputManager::addScroll(double xoffset, double yoffset) {
        m_scrollX += xoffset;
        m_scrollY += yoffset;
    }

    bool InputManager::isKeyDown(int key) const {
        if(key < 0 || key > GLFW_KEY_LAST) return false;
        return m_keys[key];
    }

    bool InputManager::isKeyPressed(int key) const {
        if(key < 0 || key > GLFW_KEY_LAST) return false;

        return m_keys[key] && !m_prevKeys[key];
    }

    bool InputManager::isKeyReleased(int key) const {
        if(key < 0 || key > GLFW_KEY_LAST) return false;
        return !m_keys[key] && m_prevKeys[key];
    }

    bool InputManager::isMouseButtonDown(int button) const {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
        return m_mouseButtons[button];
    }

    bool InputManager::isMouseButtonPressed(int button) const {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
        return m_mouseButtons[button] && !m_prevMouseButtons[button];
    }

    bool InputManager::isMouseButtonReleased(int button) const {
        if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
        return !m_mouseButtons[button] && m_prevMouseButtons[button];
    }

    double InputManager::getMouseX() const {
        return m_mouseX;
    }

    double InputManager::getMouseY() const {
        return m_mouseY;
    }

    double InputManager::getScrollX() const {
        return m_scrollX;
    }

    double InputManager::getScrollY() const {
        return m_scrollY;
    }
}