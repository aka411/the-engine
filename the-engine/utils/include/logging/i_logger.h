#pragma once
#include <string>
#include <source_location>
#include <logger_utils.h>
//uses c++ 20 feature std::source_location location

namespace TheEngine::Utils
{


class ILogger {
    
public:
    virtual ~ILogger() = default;

    // Core logging method
    virtual void Log(const LogLevel level,const LogSource logSource , const std::string& message,
                     const std::source_location location = std::source_location::current()) = 0;


};


}


