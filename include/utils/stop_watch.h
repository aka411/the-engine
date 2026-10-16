#pragma once
#include <chrono>

namespace TheEngine
{

	class StopWatch
	{
	private:
		//Note: using steady_clock as it is monotonic, high_resolution clock may or may not be, 
		std::chrono::time_point<std::chrono::steady_clock> m_prevTimePointSteadyClock;

	public:

		StopWatch() = default;

		void start();
		void stop();
	
		double setOriginNowAndGetDeltaTimeInMs();

	};



}