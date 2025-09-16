#pragma once
#include <string>
#include <source_location>
#include "logger_utils.h"
//uses c++ 20 feature std::source_location location

namespace TheEngine::Utils
{


 class ILogger {
    public:
        virtual ~ILogger() = default;


        virtual void trace(const LogSource logSource, const std::string message,
                           const std::source_location location = std::source_location::current()) = 0;

        virtual void debug(const LogSource logSource, const std::string message,
                           const std::source_location location = std::source_location::current()) = 0;

        virtual void info(const LogSource logSource, const std::string message,
                          const std::source_location location = std::source_location::current()) = 0;

        virtual void warn(const LogSource logSource, const std::string message,
                          const std::source_location location = std::source_location::current()) = 0;

        virtual void error(const LogSource logSource, const std::string message,
                           const std::source_location location = std::source_location::current()) = 0;

        virtual void fatal(const LogSource logSource, const std::string message,
                           const std::source_location location = std::source_location::current()) = 0;
    };


}


