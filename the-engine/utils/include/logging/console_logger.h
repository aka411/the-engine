#pragma once

#include <string>
#include "the-engine/utils/logging/ilogger.h"

class ConsoleLogger : public ILogger
{
public:

	ConsoleLogger() = default;
	void report(LogLevel level, const std::string& message) override;


private:
	std::string getLevelString(LogLevel level) const;

};