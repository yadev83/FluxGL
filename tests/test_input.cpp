#include <doctest/doctest.h>

#include <fluxgl/input/input_manager.h>

TEST_CASE("Input: key pressed / down / released state machine") {
    fluxgl::InputManager input;

    CHECK_FALSE(input.isKeyDown(GLFW_KEY_A));
    CHECK_FALSE(input.isKeyPressed(GLFW_KEY_A));

    input.setKey(GLFW_KEY_A, true);
    CHECK(input.isKeyDown(GLFW_KEY_A));
    CHECK(input.isKeyPressed(GLFW_KEY_A));
    CHECK_FALSE(input.isKeyReleased(GLFW_KEY_A));

    input.beginFrame();
    CHECK(input.isKeyDown(GLFW_KEY_A));
    CHECK_FALSE(input.isKeyPressed(GLFW_KEY_A)); // held -> no longer "pressed"

    input.setKey(GLFW_KEY_A, false);
    CHECK_FALSE(input.isKeyDown(GLFW_KEY_A));
    CHECK(input.isKeyReleased(GLFW_KEY_A));
}

TEST_CASE("Input: mouse buttons follow the same transitions") {
    fluxgl::InputManager input;

    input.setMouseButton(GLFW_MOUSE_BUTTON_LEFT, true);
    CHECK(input.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT));
    CHECK(input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT));

    input.beginFrame();
    CHECK_FALSE(input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT));

    input.setMouseButton(GLFW_MOUSE_BUTTON_LEFT, false);
    CHECK(input.isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT));
}

TEST_CASE("Input: out-of-range keys are ignored") {
    fluxgl::InputManager input;

    input.setKey(-1, true);
    input.setKey(GLFW_KEY_LAST + 10, true);

    CHECK_FALSE(input.isKeyDown(-1));
    CHECK_FALSE(input.isKeyDown(GLFW_KEY_LAST + 10));
}

TEST_CASE("Input: scroll accumulates and resets each frame") {
    fluxgl::InputManager input;

    input.addScroll(1.0, 2.5);
    input.addScroll(1.0, 0.5);
    CHECK(input.getScrollX() == doctest::Approx(2.0));
    CHECK(input.getScrollY() == doctest::Approx(3.0));

    input.beginFrame();
    CHECK(input.getScrollX() == doctest::Approx(0.0));
    CHECK(input.getScrollY() == doctest::Approx(0.0));
}

TEST_CASE("Input: mouse position is tracked") {
    fluxgl::InputManager input;

    input.setMousePosition(320.5, 240.25);
    CHECK(input.getMouseX() == doctest::Approx(320.5));
    CHECK(input.getMouseY() == doctest::Approx(240.25));
}