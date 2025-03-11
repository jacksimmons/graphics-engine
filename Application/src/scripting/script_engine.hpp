#pragma once
#include <filesystem>


namespace Tank
{
	class ScriptEngine
	{
	public:
		static void init();
		static void shutdown();
	private:
		static void initMono();
		static void shutdownMono();
	};
}