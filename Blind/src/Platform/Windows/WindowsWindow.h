#pragma once
#include <Blind/Core/Window.h>
#include <Blind/Renderer/GraphicsContext.h>
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

		inline virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
		
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

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