#pragma once

#include "core.hpp"


namespace Tank
{
	class TANK_API Time
	{
	private:
		static float s_frameDelta;
	public:
		// Sets time since last frame, in seconds.
		static void setFrameDelta(std::clock_t lastFrameStart, std::clock_t lastFrameEnd)
		{
			s_frameDelta = (lastFrameEnd - lastFrameStart) / (float)CLOCKS_PER_SEC;
		}
		// Returns time since last frame, in seconds.
		static float getFrameDelta()
		{
			return s_frameDelta;
		}
	};
}