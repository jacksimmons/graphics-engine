#include <type_traits>
#include "script.hpp"
#include "log.hpp"
#include "nodes/node.hpp"
#include "nodes/models/cube.hpp"


namespace Tank
{
	template <class T>
	Script<T>::Script(T *node)
	{
		Node *n;
		if ((n = dynamic_cast<T *>(node)) == nullptr)
		{
			TE_CORE_CRITICAL("Script has invalid template class. Must inherit from Node.");
		}

		m_node = node;
	}


	NewScript::NewScript(Cube *cube) : Script(cube) {}


	void NewScript::update(float frameDelta)
	{
		auto transform = m_node->getTransform();
		transform->setTranslation(glm::vec3{ frameDelta, 0, 0 });
	}
}