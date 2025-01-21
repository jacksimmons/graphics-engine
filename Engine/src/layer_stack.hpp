#pragma once

#include <vector>
#include "core.hpp"
#include "layer.hpp"


namespace Tank
{
	class TANK_API LayerStack
	{
	private:
		std::vector<Layer *> m_layers;
		std::vector<Layer *>::iterator m_layerInsert;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);
		void PopLayer(Layer *layer);
		void PopOverlay(Layer *overlay);

		std::vector<Layer *>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer *>::iterator end() { return m_layers.end(); }
	};
}