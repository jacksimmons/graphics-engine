#include <string>
#include <nodes/model.hpp>
#include "primitive.hpp"


namespace Tank
{
	Primitive::Primitive(const std::string &name, const std::string &vsName, const std::string &fsName)
		: Model(name, vsName, fsName, (float *)s_vertices, sizeof(s_vertices))
	{
	}
}