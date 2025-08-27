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


enum class LogSource
{
    RENDERER,
    PHYSICS,
    ECS

}

class ILogger {
    
public:
    virtual ~ILogger() = default;

    // Core logging method
    virtual void Log(const LogLevel level,LogSource logSource , const std::string& message,
                     const std::source_location location = std::source_location::current()) = 0;


};

}