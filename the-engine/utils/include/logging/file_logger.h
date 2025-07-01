#pragma once

#include "the-engine/utils/logging/ilogger.h"

#include <string>
#include <iostream>
#include <ctime>
#include <fstream>




class FileLogger : public ILogger
{
private:
	std::ofstream m_logFile;
	std::string m_filename = "error.log";

public:
	FileLogger();

	~FileLogger() override;


	void report(LogLevel level, const std::string& message)  override;


private:
	std::string getLevelString(LogLevel level) const;

};