#include <fluxgl/core/log.h>
#include <fluxgl/core/error.h>

#include <fstream>
#include <filesystem>
#include <mutex>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fluxgl {
    std::ofstream g_file;
    std::mutex g_mutex;

    const char* logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Error: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    std::string timestamp(bool safe = false) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        if(safe) {
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
            return ss.str();
        }

        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }

    void Logger::init() {
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            namespace fs = std::filesystem;
            
            fs::path logDir = "logs";
            if (!fs::exists(logDir)) {
                std::error_code ec;
                if(!fs::create_directory(logDir, ec)) {
                    throw Error{ErrorCode::IOError, "Failed to create log directory: " + logDir.string() + " - " + ec.message()};
                }
            }

            std::string logFilePath = (logDir / ("log-" + timestamp(true) + ".log")).string();
            
            g_file.open(logFilePath, std::ios::out | std::ios::app);
            if (!g_file.is_open()) {
                throw Error{ErrorCode::IOError, "Failed to create log file: " + logFilePath};
            }
        }

        log(LogLevel::Info, "Logger initialized");
        if(FLUXGL_DEBUG) log(LogLevel::Debug, "Running in debug mode");
    }

    void Logger::shutdown() {
        std::lock_guard<std::mutex> lock(g_mutex);

        if (g_file.is_open()) {
            g_file.close();
        }
    }

    void Logger::log(LogLevel level, const std::string& message, const std::source_location& location) {
        if(!FLUXGL_DEBUG && level == LogLevel::Debug) {
            return; // Skip debug messages in release builds
        }

        // Build message
        std::ostringstream oss;
        oss << "[" << timestamp() << "] "
            << logLevelToString(level) << " "
            << location.file_name() << ":" << location.line() << " - "
            << message;

        if(FLUXGL_DEBUG) {
            std::cerr << oss.str() << std::endl;
        }

        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_file.is_open()) {
            g_file << oss.str() << std::endl;
        }
    }
}