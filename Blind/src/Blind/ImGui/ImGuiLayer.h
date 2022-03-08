#pragma once
#include <Blind/Core/Layer.h>
#include <Blind/Events/ApplicationEvent.h>
#include <Blind/Events/KeyEvent.h>
#include <Blind/Events/MouseEvent.h>

namespace Blind
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}