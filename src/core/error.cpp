#include <fluxgl/core/error.h>

namespace fluxgl {
    class FluxGLErrorCategory : public std::error_category {
        public:
            const char *name() const noexcept override {
                return "fluxgl";
            }

            std::string message(int code) const override {
                switch (static_cast<ErrorCode>(code)) {
                    case ErrorCode::Success: return "Success";
                    case ErrorCode::InvalidArgument: return "Invalid argument";
                    case ErrorCode::FileNotFound: return "File not found";
                    case ErrorCode::OpenGLError: return "OpenGL error";
                    case ErrorCode::ShaderCompilationFailed: return "Shader compilation failed";
                    case ErrorCode::WindowCreationFailed: return "Window creation failed";
                }
                return "Unknown error";
            }
    };

    // Error category for fluxgl
    const std::error_category& fluxgl_error_category() noexcept {
        static const FluxGLErrorCategory instance;
        return instance;
    }

    // Helper to create an error code from ErrorCode enum
    std::error_code make_error_code(ErrorCode code) noexcept {
        return std::error_code(static_cast<int>(code), fluxgl_error_category());
    }
}