#include <string>
#include <imgui.h>
#include "console.hpp"


namespace Tank
{
	Console::Console(const std::string &name) : Panel(name, ImGuiWindowFlags_None, true){}


	void Console::drawPanel()
	{
		std::string linesAsStr;
		for (auto line : m_lines)
		{
			line();
		}
	}


	void Console::addLine(std::function<void()> line)
	{
		m_lines.push_back(line);

		// Maximum length of 100
		if (m_lines.size() >= 100) m_lines.erase(m_lines.begin());
	}
}