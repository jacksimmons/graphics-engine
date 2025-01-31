#pragma once
#include "nodes/ui.hpp"


namespace Tank
{
	class UI;
	class Panel : public UI
	{
	public:
		Panel(const std::string &name);
		virtual ~Panel() = default;

		virtual void drawUI() override;
		virtual void drawPanel() = 0;
	};
}