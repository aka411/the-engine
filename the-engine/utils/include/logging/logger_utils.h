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
	case LogLevel::Info:     return "INFO";
	case LogLevel::Warning:  return "WARNING";
	case LogLevel::Error:    return "ERROR";
	case LogLevel::Critical: return "CRITICAL";
	default:                 return "UNKNOWN";
	}
}
    
}