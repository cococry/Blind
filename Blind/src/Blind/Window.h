#pragma once

#include <blindpch.h>

#include <Blind/Core.h>
#include <Blind/Events/Event.h>

namespace Blind
{
	struct WindowProperties
	{
		std::string title;
		uint32_t width, height;

		WindowProperties(const std::string& title = "Blind Engine", uint32_t width = 1920, uint32_t height = 1080)
			: title(title), width(width), height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& calllback) = 0;
		virtual void SetVsync(bool enabled) = 0;
		
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& props = WindowProperties());
	};
}