#include "layer_stack.hpp"


namespace Tank
{
	LayerStack::LayerStack()
	{
		m_layerInsert = m_layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer *layer : m_layers)
			delete layer;
	}

	/// <summary>
	/// Pushes to the front of the vector, effectively making index 0 the top of the stack.
	/// The current 0th element is moved to position 1, and 1 to 2, and length - 1 to length.
	/// </summary>
	void LayerStack::PushLayer(Layer *layer)
	{
		m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	}

	/// <summary>
	/// Pops the layer at its index (which must be found), if it is present in the vector.
	/// If it was present, then layer insert must be moved one back
	/// </summary>
	void LayerStack::PopLayer(Layer *layer)
	{
		// Find first element matching `layer`
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);

		// If it == end() then element was not found.
		if (it != m_layers.end())
		{
			// Remove at position the layer was found at.
			m_layers.erase(it);
			// 
			m_layerInsert--;
		}
	}
}