#pragma once
#include <Blind/Core/Layer.h>
#include <Blind/Events/ApplicationEvent.h>
#include <Blind/Events/KeyEvent.h>
#include <Blind/Events/MouseEvent.h>

namespace Blind
{
	enum class ScemeType
	{
		Dark = 0,
		White = 1
	};
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		static void SetColorSceme(const ScemeType& type);

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColors();

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}