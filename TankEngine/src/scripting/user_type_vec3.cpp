#include <log.hpp>
#include <sol/sol.hpp>
#include "user_types.hpp"


namespace Tank
{
	void UserTypes::Vec3(sol::state &lua)
	{
		// Meta functions
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::addition,
			"Vec3",
			{ { "other", "Vec3" } }
		)] = [](const glm::vec3 &a, const glm::vec3 &b) { return a + b; };
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::subtraction,
			"Vec3",
			{ { "other", "Vec3" } }
		)] = [](const glm::vec3 &a, const glm::vec3 &b) { return a - b; };
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::multiplication,
			"Vec3",
			{ { "scalar", "number" } }
		)] = [](const glm::vec3 &a, const float b) { return a * b; };
		utVec3[SOL_META_METHOD(
			"Vec3",
			sol::meta_function::division,
			"Vec3",
			{ { "scalar", "number" } }
		)] = [](const glm::vec3 &a, const float b) { return a / b; };
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::unary_minus,
			"Vec3",
			{}
		);
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::equal_to,
			"boolean",
			{ { "other", "Vec3" } }
		);
		SOL_META_METHOD(
			"Vec3",
			sol::meta_function::to_string,
			"string",
			{}
		);
	}
}