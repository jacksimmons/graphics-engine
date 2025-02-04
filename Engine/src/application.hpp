#pragma once
#include <memory>
#include "core.hpp"
#include "static/time.hpp"


namespace Tank
{
	class TANK_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void run() = 0;

	protected:
		static void setFrameDelta(std::clock_t lastFrameStart, std::clock_t lastFrameEnd)
		{
			Time::setFrameDelta(lastFrameStart, lastFrameEnd);
		}
	};

	std::unique_ptr<Tank::Application> createApplication();
}