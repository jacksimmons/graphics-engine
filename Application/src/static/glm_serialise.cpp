#include "glm_serialise.hpp"
#include "transformation.hpp"


namespace Tank
{
	// ============== glm::vec3 ===============
	json vec3::serialise(const glm::vec3 &vec3)
	{
		json serialised = {
			{ "x", vec3.x },
			{ "y", vec3.y },
			{ "z", vec3.z }
		};

		return serialised;
	}


	glm::vec3 vec3::deserialise(const json &serialised)
	{
		return glm::vec3{ serialised["x"], serialised["y"], serialised["z"] };
	}


	// ============== glm::quat ===============
	json quat::serialise(const glm::quat &quat)
	{
		json serialised = {
			{ "w", quat.w },
			{ "x", quat.x },
			{ "y", quat.y },
			{ "z", quat.z }
		};

		return serialised;
	}


	glm::quat quat::deserialise(const json &serialised)
	{
		return glm::quat{ serialised["w"], serialised["x"], serialised["y"], serialised["z"] };
	}
}