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




std::string getLogLevelString(const LogLevel level) const
{
	switch (level)
	{
	case LogLevel::TRACE :  return "TRACE";
	case LogLevel::DEBUG :  return "DEBUG";
	case LogLevel::INFO  :  return "INFO";
	case LogLevel::WARN  :  return "WARN";
    case LogLevel::ERROR :  return "ERROR";
    case LogLevel::FATAL :  return "FATAL";
	default  :              return "UNKNOWN";
	}
}
    
}