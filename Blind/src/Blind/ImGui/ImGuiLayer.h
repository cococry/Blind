#pragma once
#include <Blind/Layer.h>
#include <Blind/Events/ApplicationEvent.h>
#include <Blind/Events/KeyEvent.h>
#include <Blind/Events/MouseEvent.h>

namespace Blind
{
	class BLIND_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiDraw() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}