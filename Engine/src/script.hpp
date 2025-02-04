#pragma once
#include <core.hpp>


namespace Tank
{
	class Node;
	class TANK_API IScript
	{
	public:
		IScript() {};
		virtual ~IScript() {};

		virtual void setEnabled(bool enabled) noexcept = 0;
		virtual bool getEnabled() const noexcept = 0;
		virtual void update() = 0;
	};

	template <class T>
	class TANK_API Script : public IScript
	{
	protected:
		T *m_node;
		bool m_enabled = true;
	public:
		Script(T *node);
		virtual ~Script() {};

		void setEnabled(bool enabled) noexcept { m_enabled = enabled; }
		bool getEnabled() const noexcept { return m_enabled; }
		virtual void update() override = 0;
	};
}