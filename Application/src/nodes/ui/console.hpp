#pragma once
#include <functional>
#include <vector>
#include <tuple>
#include "panel.hpp"


namespace Tank::Editor
{
	class _Console : public _Panel
	{
		friend class EditorApp;
	private:
		struct ConsoleCommand
		{
			std::string name;
			std::function<void()> callback;
		};

		std::vector<std::function<void()>> m_lines;
		std::vector<ConsoleCommand> m_commands;

		_Console(const std::string &name);
	public:
		virtual void drawPanel() override;

		void addLine(std::function<void()> line);
		void addColouredTextLine(ImVec4 colour, std::string text);
	};
}