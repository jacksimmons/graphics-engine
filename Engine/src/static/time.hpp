#pragma once
#include <chrono>
#include <core.hpp>


namespace Tank
{
	class TANK_API Time
	{
		friend class Application; // Applications can modify time values
	private:
		static float s_frameDelta;
		static void setFrameDelta(std::clock_t lastFrameStart, std::clock_t lastFrameEnd)
		{
			s_frameDelta = (lastFrameEnd - lastFrameStart) / (float)CLOCKS_PER_SEC;
		}
	public:
		static float getFrameDelta() { return s_frameDelta; }
	};
}