#include <sol/sol.hpp>
#include "user_types.hpp"


namespace Tank
{
	void UserTypes::Quat(sol::state &lua)
	{
		// Quat
		auto utVec3 = lua.new_usertype<glm::quat>(
			SOL_CLASS("Quat"),
			sol::constructors<glm::quat()>()
		);
		SOL_CONSTRUCTOR("Quat", {});

		utVec3[SOL_FIELD("Quat", "x", "number")] = &glm::quat::x;
		utVec3[SOL_FIELD("Quat", "y", "number")] = &glm::quat::y;
		utVec3[SOL_FIELD("Quat", "z", "number")] = &glm::quat::z;
		utVec3[SOL_FIELD("Quat", "w", "number")] = &glm::quat::w;

		utVec3[SOL_STATIC_METHOD("Quat", "euler_angles", "Quat", {{ "angles", "Vec3" }})] = [](const glm::vec3 &vec)
		{
			glm::quat rot = glm::vec3(vec.y, vec.x, vec.z);
			return rot;
		};
	}
}