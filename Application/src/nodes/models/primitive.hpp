#pragma once
#include "nodes/model.hpp"


namespace Tank
{
	class Model;
	class Primitive : public Model
	{
	public:
		Primitive(const std::string &name, const std::string &vsName="shader.vert", const std::string &fsName="shader.frag");
	private:
		static constexpr float s_vertices[] = {
			// positions          // normals           // texture coords
			-0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		};
	};
}