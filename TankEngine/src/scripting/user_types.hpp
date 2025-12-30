#pragma once
#include "lua_codegen.hpp"


namespace sol { class state; }
namespace Tank::UserTypes
{
	inline std::vector<LuaClass> s_luaClasses;
	
	std::optional<LuaClass *>classFromName(const std::string &);
	void Vec3(sol::state &);
	void Quat(sol::state &);

	void codegen();
}