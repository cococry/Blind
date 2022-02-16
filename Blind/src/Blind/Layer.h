#pragma once

#include <Blind/Core.h>
#include <Blind/Events/Event.h>

namespace Blind
{
	class BLIND_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}