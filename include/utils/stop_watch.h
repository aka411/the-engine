#pragma once
#include <chrono>

namespace TheEngine
{

	class StopWatch
	{
	private:

		std::chrono::microseconds m_prevTimeMicroSecond{};

	public:

		StopWatch() = default;

		void start();
		void stop();
	
		double setOriginNowAndGetDeltaTimeInMs();

	};



}