#include <type_traits>
#include "script.hpp"
#include "log.hpp"
#include "static/time.hpp"
#include "nodes/node.hpp"


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
}