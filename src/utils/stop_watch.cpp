#include <utils/stop_watch.h>


namespace TheEngine
{


	void StopWatch::start()
	{
	
		m_prevTimePointSteadyClock = std::chrono::steady_clock::now();
	}

	void StopWatch::stop()
	{

	}

	double StopWatch::setOriginNowAndGetDeltaTimeInMs()
	{

		std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
		std::chrono::time_point<std::chrono::steady_clock> prev = m_prevTimePointSteadyClock;
		m_prevTimePointSteadyClock = now;

		return std::chrono::duration<double, std::milli>{now - prev}.count();
	}

}