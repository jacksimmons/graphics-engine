// CODEGEN FROM .\codegen\usertypes\main.py
#include "generate_user_types.hpp"
#include "user_types.hpp""

namespace Tank
{
    static void generateQuat(sol::state &lua)
    {
        // Define Usertype
		auto ut = lua.new_usertype<glm::quat>(
		    "Quat",
		    sol::constructors<glm::quat(),glm::quat(float, float, float, float)>()
		);
		
		
		// LuaLS Codegen
		LuaClass lc;
		SOL_CONSTRUCTOR("Quat",{});
		SOL_CONSTRUCTOR("Quat",{{"param1", "number"},{"param2", "number"},{"param3", "number"},{"param4", "number"}});
		
		UserTypes::s_luaClasses.push_back(lc);
    }

    static void generateVec3(sol::state &lua)
    {
        // Define Usertype
		auto ut = lua.new_usertype<glm::vec3>(
		    "Vec3",
		    sol::constructors<glm::vec3(),glm::vec3(float, float, float)>()
		);
		ut["x"] = &glm::vec3::x;
		ut["y"] = &glm::vec3::y;
		ut["z"] = &glm::vec3::z;
		
		// LuaLS Codegen
		LuaClass lc;
		SOL_CONSTRUCTOR("Vec3",{});
		SOL_CONSTRUCTOR("Vec3",{{"param1", "number"},{"param2", "number"},{"param3", "number"}});
		lc.fields.push_back({"x", "number"});
		lc.fields.push_back({"y", "number"});
		lc.fields.push_back({"z", "number"});
		UserTypes::s_luaClasses.push_back(lc);
    }


    void generateAll(sol::state &lua)
    {
        generateQuat(lua);
		generateVec3(lua);
    }
}