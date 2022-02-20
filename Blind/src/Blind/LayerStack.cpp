#include "blindpch.h"
#include "LayerStack.h"

namespace Blind
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInstertIndex, layer);
		m_LayerInstertIndex++;
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
			m_LayerInstertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (iterator != m_Layers.end())
			m_Layers.erase(iterator);
	}

}
