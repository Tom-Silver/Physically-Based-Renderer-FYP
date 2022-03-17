#include "DeltaTimeClock.h"

namespace TSFYP
{
	DeltaTimeClock::DeltaTimeClock() :
		mCurrentFPS(0.0f),
		mLastUpdate(Clock::now()),
		mFrameCount(0),
		mAccFrameTime(std::chrono::seconds(0))
	{
	}

	DeltaTimeClock::~DeltaTimeClock()
	{
	}

	float DeltaTimeClock::GetDeltaTime()
	{
		const Clock::time_point now = Clock::now();

		const FrameTime deltaTime = std::chrono::duration_cast<FrameTime>(now - mLastUpdate);
		mAccFrameTime += deltaTime;

		mLastUpdate = now;

		// return deltaTime in seconds
		return deltaTime.count();
	}

	void DeltaTimeClock::Update()
	{
		mFrameCount++;

		if (mAccFrameTime > INTERVAL) // Every interval, calc fps
		{
			mCurrentFPS = mFrameCount;

			// Reset frame count/time
			mFrameCount = 0;
			mAccFrameTime = FrameTime(0);
		}
	}
}