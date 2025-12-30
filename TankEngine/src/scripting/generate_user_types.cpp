// CODEGEN FROM .\codegen\usertypes\main.py
#include "generate_user_types.hpp"
#include "user_types.hpp""
#include <nodes/node.hpp>#include <transform.hpp>
namespace Tank
{
    static void generateNode(sol::state &lua)
    {
        // Define Usertype
		auto ut = lua.new_usertype<Node>(
		    "Node"
		);
		ut["name"] = sol::property(&Node::getName, &Node::setName);
		ut["transform"] = sol::property(&Node::getTransform);
		ut["key_input"] = sol::property(&Node::getKeyInput);
		
		// LuaLS Codegen
		LuaClass lc;
		lc.fields.push_back({"name", "string"});
		lc.fields.push_back({"transform", "Transform"});
		lc.fields.push_back({"key_input", "KeyInput"});
		UserTypes::s_luaClasses.push_back(lc);
    }

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

    static void generateTransform(sol::state &lua)
    {
        // Define Usertype
		auto ut = lua.new_usertype<Transform>(
		    "Transform"
		);
		ut["translation"] = sol::property(&Transform::getLocalTranslation, &Transform::setLocalTranslation);
		ut["rotation"] = sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation);
		ut["scale"] = sol::property(&Transform::getLocalScale, &Transform::setLocalScale);
		
		// LuaLS Codegen
		LuaClass lc;
		lc.fields.push_back({"translation", "Vec3"});
		lc.fields.push_back({"rotation", "Quat"});
		lc.fields.push_back({"scale", "Vec3"});
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
		ut[sol::meta_method::addition] = [](const glm::vec3 &a, const glm::vec3 &b){return a + b;};
		ut[sol::meta_method::subtraction] = [](const glm::vec3 &a, const glm::vec3 &b){return a - b;};
		ut[sol::meta_method::multiplication] = [](const glm::vec3 &a, const float &b){return a * b;};
		ut[sol::meta_method::division] = [](const glm::vec3 &a, const float &b){return a / b;};
		ut[sol::meta_method::unary_minus] = [](const glm::vec3 &a){return -a;};
		ut[sol::meta_method::equal_to] = [](const glm::vec3 &a, const glm::vec3 &b){return a == b;};
		
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
        generateNode(lua);
		generateQuat(lua);
		generateTransform(lua);
		generateVec3(lua);
    }
}