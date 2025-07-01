#pragma once

#include"the-engine/utils/logging/console_logger.h"
#include "the-engine/utils/logging/ilogger.h"

//consider a separate cpp file for this to kep the header clean

//FIXME: whole class need to be reworked
class Logger // Static Factory Method
{ 

private:
	// Singleton pattern enforcement
	Logger() = default; // Prevent direct instantiation
	~Logger() = default; // Prevent external deletion


	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;


	static ILogger& m_instance; // Singleton

public:


	static ILogger& getInstance()
	{
		static ConsoleLogger  m_instance; // Default to ConsoleLogger 
		return m_instance;
	}

	// Optional: Method to set a different implementation (for flexibility)

	static void setInstance(ILogger& reporter)//bad need better way
	{
		
		m_instance = reporter;
	}

};
