#pragma once
#include <Blind/Window.h>
#include <GLFW/glfw3.h>

namespace Blind
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.width; }
		inline uint32_t GetHeight() const override { return m_Data.height; }

		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }
		void SetVsync(bool enabled) override;
		bool IsVsync() const;
	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
		
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;

			EventCallbackFunction EventCallback;
			bool vsync;
		};
		WindowData m_Data;
	};
}