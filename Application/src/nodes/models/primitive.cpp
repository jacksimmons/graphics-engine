#include <string>
#include <nodes/model.hpp>
#include "primitive.hpp"


namespace Tank
{
	Primitive::Primitive(std::string name, std::string vsName, std::string fsName)
		: Model(name, vsName, fsName, (float *)s_vertices, sizeof(s_vertices))
	{
	}
}