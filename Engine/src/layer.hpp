#pragma once

#include <string>
#include "core.hpp"


namespace Tank
{
	class TANK_API Layer
	{
	protected:
		std::string m_debugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event &event) {}

		inline const std::string& GetName() const { return m_debugName; }
	};
}