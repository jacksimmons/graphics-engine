#pragma once
#include <functional>
#include "panel.hpp"


namespace Tank
{
	class TANK_API Console : public Panel
	{
	private:
		std::vector<std::function<void()>> m_lines;
	public:
		Console(const std::string &name);
		virtual void drawPanel() override;

		void addLine(std::function<void()> line);
	};
}