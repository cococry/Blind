#include "blindpch.h"
#include "LayerStack.h"

namespace Blind
{
	LayerStack::LayerStack()
	{
		m_LayerInstertPos = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInstertPos = m_Layers.emplace(m_LayerInstertPos, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		// Overlays are always pushed behind layers
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer); // returning a Layer pointer
		if (iterator != m_Layers.end())
		{
			m_Layers.erase(iterator);
			m_LayerInstertPos--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (iterator != m_Layers.end())
			m_Layers.erase(iterator);
	}

}
