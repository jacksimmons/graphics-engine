#include <glm/glm.hpp>
#include "nodes/model.hpp"


namespace Tank
{
	class Light : public Node
	{
	public:
		Light(std::string name, glm::vec3 pos);
	};
}