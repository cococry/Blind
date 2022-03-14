/* Blind Event System Key Event Header */

/* This header implements all of the necessary key events in Blind.
   It heavily depends on the main Event System header and every implemented event
	inherits from the Event class from the main header. */
#pragma once
#include "Event.h"

namespace Blind
{
	class KeyEvent : public Event
	{
	public:
		inline uint32_t GetKeyCode() { return m_Keycode; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(uint32_t keycode)
			: m_Keycode(keycode) {}

		int m_Keycode;
	};
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(uint32_t keycode, uint32_t repeatcount)
			: m_RepeatCount(repeatcount), KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: (Key-code) " << m_Keycode << ": Repeat Count: " << m_RepeatCount;
			return ss.str();
		}
		uint32_t GetRepeatCount() { return m_RepeatCount; }
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint32_t m_RepeatCount;
	};
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(uint32_t keycode)
			: KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: (Key-code) " << m_Keycode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(uint32_t keycode)
			: KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: (Key-code) " << m_Keycode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}