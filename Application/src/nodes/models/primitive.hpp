#pragma once


namespace Tank
{
	class Model;
	class Primitive : public Model
	{
	public:
		Primitive(std::string name, std::string vsName, std::string fsName);
	private:
		static constexpr float s_vertices[] = {
			// positions          // normals           // texture coords
			-0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		};
	};
}