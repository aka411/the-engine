#include <utils/stop_watch.h>


namespace TheEngine
{


	void StopWatch::start()
	{
		//Note: using steady_clock as it is monotonic, high_resolution clock may or may not be, 
		m_prevTimeMicroSecond = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch());
	}

	void StopWatch::stop()
	{

	}

	double StopWatch::setOriginNowAndGetDeltaTimeInMs()
	{
		std::chrono::microseconds now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch());
		std::chrono::microseconds prev = m_prevTimeMicroSecond;
		
		m_prevTimeMicroSecond = now;

		return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count());

	}

}