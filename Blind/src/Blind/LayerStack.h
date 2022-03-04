#pragma once

#include <Blind/Core.h>
#include <Blind/Layer.h>

#include <vector>

namespace Blind
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		// ranged base for loop implementation
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		uint32_t m_LayerInstertIndex = 0;
	};
}