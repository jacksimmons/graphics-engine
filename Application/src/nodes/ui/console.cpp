#include <string>
#include "console.hpp"


namespace Tank
{
	Console::Console(const std::string &name) : Panel(name){}


	void Console::drawPanel()
	{
		std::string linesAsStr;
		for (auto line : m_lines)
		{
			ImGui::Text(line.c_str());
		}
	}


	void Console::addLine(const std::string &line)
	{
		m_lines.push_back(line);

		// Maximum length of 100
		if (m_lines.size() >= 100) m_lines.erase(m_lines.begin());
	}
}