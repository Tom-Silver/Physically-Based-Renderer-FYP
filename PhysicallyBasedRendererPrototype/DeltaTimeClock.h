#pragma once

// External includes
#include <chrono>

namespace TSFYP
{
	class DeltaTimeClock
	{
		using Clock = std::chrono::steady_clock;
		using FrameCount = uint32_t;
		using FrameTime = std::chrono::duration<float>;

		static constexpr FrameTime INTERVAL{ FrameTime(1) };
	public:
		DeltaTimeClock();
		~DeltaTimeClock();

		float GetDeltaTime();
		void Update();
		float GetFPS() const { return mCurrentFPS; }

	private:
		Clock::time_point mLastUpdate;

		FrameCount mFrameCount;
		FrameTime mAccFrameTime;

		float mCurrentFPS;
	};
}
