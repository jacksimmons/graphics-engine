#pragma once
#include <vector>
#include <memory>
#include "imgui.h"
#include "node.hpp"


namespace Tank
{
	class UIElement : public Node
	{
	public:
		UIElement(std::string name);
		virtual void draw() const;
	};
}