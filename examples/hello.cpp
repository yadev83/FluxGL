#include <fluxgl/fluxgl.h>
#include <iostream>

int main() {
    #if FLUXGL_DEBUG
        std::cout << "Running in debug mode" << std::endl;
    #endif

    try {
        fluxgl::Window window = fluxgl::Window(800, 600, "Hello FluxGL");
        while (!window.shouldClose()) {
            window.pollEvents();
            window.swapBuffers();
        }
    } catch (const fluxgl::Error& error) {
        std::cerr << "fluxGL error: " << error.code << " - " << error.message << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }

    return 0;
}