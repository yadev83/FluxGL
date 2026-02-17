#pragma once

#include <glfw/glfw3.h>
#include <array>

namespace fluxgl {
    class InputManager {
        private:
            std::array<bool, GLFW_KEY_LAST + 1> m_keys = {false};
            std::array<bool, GLFW_KEY_LAST + 1> m_prevKeys = {false};

            std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> m_mouseButtons = {false};
            std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> m_prevMouseButtons = {false};

            double m_mouseX = 0.0;
            double m_mouseY = 0.0;
            double m_prevMouseX = 0.0;
            double m_prevMouseY = 0.0;

            double m_scrollX = 0.0;
            double m_scrollY = 0.0;

        public:
            void beginFrame();

            void setKey(int key, bool value);
            void setMouseButton(int button, bool value);
            void setMousePosition(double x, double y);
            void addScroll(double xoffset, double yoffset);

            bool isKeyDown(int key) const;
            bool isKeyPressed(int key) const;
            bool isKeyReleased(int key) const;

            bool isMouseButtonDown(int button) const;
            bool isMouseButtonPressed(int button) const;
            bool isMouseButtonReleased(int button) const;

            double getMouseX() const;
            double getMouseY() const;
            double getScrollX() const;
            double getScrollY() const;
    };
}