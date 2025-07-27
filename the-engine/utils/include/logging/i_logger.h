#pragma once

//uses c++ 20 feature std::source_location location

namespace TheEngine::Utils
{

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATEL
};


class ILogger {
    
public:
    virtual ~ILogger() = default;

    // Core logging method
    virtual void Log(LogLevel level, const std::string& message,
                     const std::source_location location = std::source_location::current()) = 0;


};

}