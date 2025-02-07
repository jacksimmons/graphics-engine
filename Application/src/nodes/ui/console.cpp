#include <string>
#include <imgui.h>
#include "console.hpp"
#include "widget.hpp"


namespace Tank::Editor
{
	_Console::_Console(const std::string &name) : _Panel(name, ImGuiWindowFlags_None, true)
	{
		m_commands.push_back({
			.name = "help",
			.callback =
			[this]() { 
				addLine([]() {
					ImGui::Text(
						"[help]\n"
						"--- Global Keybinds ---\n"
						"ESC - Quit\n"
						"--- SceneView Keybinds ---\n"
						"F1 - Toggle glPolygonMode\n"
						"(x)AD (y)WS (z)QE - Camera pan, axis in brackets\n"
						"(x)JL (y)IK (z)UO - Camera rotation, axis in brackets\n"
					);
				});
			}
		});
	}


	void _Console::drawPanel()
	{
		std::string linesAsStr;
		for (auto line : m_lines)
		{
			line();
		}

		// Command input
		Widget::textInput("Enter a command...", "help", [this](const std::string &modified)
		{
			if (!ImGui::IsItemDeactivatedAfterEdit()) return;

			// For each valid console command...
			for (const auto &command : m_commands)
			{
				// If user input equals this command
				if (command.name == modified)
				{
					command.callback();
				}
			}
		});
	}


	void _Console::addLine(std::function<void()> line)
	{
		m_lines.push_back(line);

		// Maximum length of 100
		if (m_lines.size() >= 100) m_lines.erase(m_lines.begin());
	}
}