#pragma once
#include <string>
#include <source_location>
#include "i_logger.h"

namespace TheEngine::Utils
{

class NullLogger : public ILogger {

public:
    void trace(const LogSource logSource, const std::string message, const std::source_location location = std::source_location::current()) override {}
    void debug(const LogSource logSource, const std::string message, const std::source_location location = std::source_location::current()) override {}
    void info(const LogSource logSource,  const std::string message, const std::source_location location = std::source_location::current()) override {}
    void warn(const LogSource logSource,  const std::string message, const std::source_location location = std::source_location::current()) override {}
    void error(const LogSource logSource, const std::string message, const std::source_location location = std::source_location::current()) override {}
    void fatal(const LogSource logSource, const std::string message, const std::source_location location = std::source_location::current()) override {}
};

}