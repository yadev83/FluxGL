#pragma once

#include <system_error>
#include <string>

namespace fluxgl {
    enum class ErrorCode {
        Success = 0,
        InvalidArgument,
        FileNotFound,
        OpenGLError,
        ShaderCompilationFailed,
        WindowCreationFailed
    };

    struct Error {
        std::error_code code;
        std::string message;

        explicit operator bool() const noexcept {
            return static_cast<bool>(code);
        }
    };

    // Error category for fluxgl
    const std::error_category& fluxgl_error_category() noexcept;

    // Helper to create an error code from ErrorCode enum
    std::error_code make_error_code(ErrorCode code) noexcept;
}

namespace std {
    template <>
    struct is_error_code_enum<fluxgl::ErrorCode> : true_type {};
}