#include "the-engine/utils/logging/console_logger.h"
#include <iostream>


void ConsoleLogger::report(LogLevel level, const std::string& message) 
{
	std::cerr << "[" << getLevelString(level) << "] " << message << std::endl;
}


std::string ConsoleLogger::getLevelString(LogLevel level) const
{
	switch (level)
	{
	case LogLevel::Info:     return "INFO";
	case LogLevel::Warning:  return "WARNING";
	case LogLevel::Error:    return "ERROR";
	case LogLevel::Critical: return "CRITICAL";
	default:                 return "UNKNOWN";
	}
}
