#pragma once
#include "nodes/ui/ui.hpp"

namespace Tank::Editor
{
	class _Panel : public UI
	{
		// Classes permitted access to constructor
		friend class _SceneView;
		friend class _Hierarchy;
		friend class _Inspector;
		friend class _Console;
	private:
		ImGuiWindowFlags m_flags;
		bool m_autoScroll;
		_Panel(const std::string &name, const ImGuiWindowFlags &flags = ImGuiWindowFlags_None, bool autoScroll = false);
	public:
		virtual ~_Panel() = default;

		virtual void drawUI() override;
		virtual void drawPanel() = 0;
	};
}
