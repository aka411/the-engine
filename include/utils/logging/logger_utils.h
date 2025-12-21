#pragma once

namespace TheEngine::Utils
{





enum class LogSource
{
    RENDERER,
    PHYSICS,
    ECS,
    INPUT

};




inline std::string getLogSourceString(const LogSource logSource)
{
	switch (logSource)
	{
	case LogSource::RENDERER :  return "RENDERER";
	case LogSource::PHYSICS  :  return "PHYSICS";
	case LogSource::ECS      :  return "ECS";
	case LogSource::INPUT    :  return "INPUT";
	default  :                  return "UNKNOWN";
	}
}
    

}
