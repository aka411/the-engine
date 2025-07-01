#pragma once

#include <string>


enum class LogLevel
{
	Info,
	Warning,
	Error,
	Critical
};



class ILogger
{

public:
	virtual ~ILogger() = default; // Important for polymorphism

	virtual void report(LogLevel level, const std::string& message) = 0;
	/**should this be const ? it does file operations and does write to std::cerr,
	* in terms of class member modification it does not modify the state of the class,
	* but it does cause side effects (writing to std::cerr) or writing to file.
	*
	* NEW FIDNINGS
	*
	* writing to file does cause error,but using std::cerr does not cause a compile time error
	* writing to file is considered a change of state by compiler.
	* For now to circumvent this issue, I have made the file mutable.(may not be the best design choice).
	**/
};
