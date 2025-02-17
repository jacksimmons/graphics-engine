#include "transform.hpp"
#include "script_prop.hpp"


namespace Tank
{
	sol::table ScriptProp::transformToTable(sol::state *state, Transform *transform)
	{
		const auto &trans = transform->getLocalTranslation();
		const auto &rot = glm::eulerAngles(transform->getLocalRotation());
		const auto &scale = transform->getLocalScale();

		return state->create_table_with(
			"translation", state->create_table_with("x", trans.x, "y", trans.y, "z", trans.z),
			"rotation", state->create_table_with("x", rot.x, "y", rot.y, "z", rot.z),
			"scale", state->create_table_with("x", scale.x, "y", scale.y, "z", scale.z)
		);
	}


	void ScriptProp::tableToTransform(const sol::table &table, Transform *transform)
	{
		transform->setLocalTranslation({ table["translation"]["x"], table["translation"]["y"], table["translation"]["z"] });
		transform->setLocalRotation(quat::fromAngleAxis({ table["rotation"]["x"], table["rotation"]["y"], table["rotation"]["z"] }));
		transform->setLocalScale({ table["scale"]["x"], table["scale"]["y"], table["scale"]["z"] });
	}
}