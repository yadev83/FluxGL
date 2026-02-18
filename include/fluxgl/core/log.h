#pragma once

#include <string>
#include <source_location>

namespace fluxgl {
    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warning,
        Error
    };

    class Logger {
        private:
            Logger() = default; ~Logger() = default;
            static LogLevel s_logLevel;

        public: 
            static void init(LogLevel logLevel = LogLevel::Info, const std::string& logFileName = "log");
            static void shutdown();

            static void log(LogLevel level,
                            const std::string& message,
                            const std::source_location& location = std::source_location::current());
    };
}

#define FLUXGL_LOG_TRACE(msg) ::fluxgl::Logger::log(::fluxgl::LogLevel::Trace, msg)
#define FLUXGL_LOG_DEBUG(msg)   ::fluxgl::Logger::log(::fluxgl::LogLevel::Debug, msg)
#define FLUXGL_LOG_INFO(msg)    ::fluxgl::Logger::log(::fluxgl::LogLevel::Info, msg)
#define FLUXGL_LOG_WARNING(msg) ::fluxgl::Logger::log(::fluxgl::LogLevel::Warning, msg)
#define FLUXGL_LOG_ERROR(msg)   ::fluxgl::Logger::log(::fluxgl::LogLevel::Error, msg)