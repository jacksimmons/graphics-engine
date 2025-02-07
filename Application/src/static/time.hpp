#pragma once
#include <chrono>


namespace Tank
{
	namespace Editor { class EditorApp; }

	class Time
	{
		// Applications can modify time values
		friend class Editor::EditorApp;
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