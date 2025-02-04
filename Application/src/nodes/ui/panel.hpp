#pragma once
#include "nodes/ui/ui.hpp"


namespace Tank
{
	class UI;
	class Panel : public UI
	{
	private:
		ImGuiWindowFlags m_flags;
		bool m_autoScroll;
	public:
		Panel(const std::string &name, const ImGuiWindowFlags &flags = ImGuiWindowFlags_None, bool autoScroll = false);
		virtual ~Panel() = default;

		virtual void drawUI() override;
		virtual void drawPanel() = 0;
	};
}