#pragma once
#include <sol/sol.hpp>


namespace Tank
{
	class ScriptProp
	{
	public:
		static sol::table transformToTable(sol::state *state, Transform *transform);
		static void tableToTransform(const sol::table &table, Transform *transform);
	};
}