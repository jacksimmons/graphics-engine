#pragma once
#include <deque>
#include "panel.hpp"


namespace Tank
{
	class Console : public Panel
	{
	private:
		std::vector<std::string> m_lines;
	public:
		Console(const std::string &name);
		virtual void drawPanel() override;

		void addLine(const std::string &line);
	};
}