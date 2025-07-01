#include "the-engine/utils/logging/file_logger.h"  
#include <iomanip>  
#include <iostream>  

FileLogger::FileLogger()
{
	m_logFile.open(m_filename, std::ios::app);
	if (!m_logFile.is_open())
	{
		std::cerr << "Warning: Could not open error log file: " << m_filename << std::endl;
		// Fallback to console output  
	}
}

FileLogger::~FileLogger()
{
	if (m_logFile.is_open())
	{
		m_logFile.close();
	}
}

void FileLogger::report(LogLevel level, const std::string& message)  
{
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);

	if (m_logFile.is_open())
	{
		m_logFile << "[" << getLevelString(level) << "] ";
		m_logFile << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " - ";
		m_logFile << message << std::endl;
	}
	else
	{
		std::cerr << "Warning: Log file is not open. Logging to console instead." << std::endl;
		std::cerr << "[" << getLevelString(level) << "] ";
		std::cerr << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " - ";
		std::cerr << message << std::endl;
	}
}

std::string FileLogger::getLevelString(LogLevel level) const
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
