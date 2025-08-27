#pragma once
#include <source_location>

//uses c++ 20 feature std::source_location location

namespace TheEngine::Utils
{

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};


enum class LogSource
{
    RENDERER,
    PHYSICS,
    ECS

};

class ILogger {
    
public:
    virtual ~ILogger() = default;

    // Core logging method
    virtual void Log(const LogLevel level,const LogSource logSource , const std::string& message,
                     const std::source_location location = std::source_location::current()) = 0;


};


}

