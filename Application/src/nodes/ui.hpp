#pragma once
#include <vector>
#include <memory>
#include "imgui.h"
#include "node.hpp"


namespace Tank
{
	class UI : public Node
	{
	public:
		UI(std::string name);
		void draw() override;
		virtual void drawUI() = 0;
	};
}