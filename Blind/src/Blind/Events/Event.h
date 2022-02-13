/* Blind Event System Main Header */

/* This header includes all necessary classes and enums to create any kind of event in other event files of the system.
   The base Event class defines all important methods for an event. The event system is a raw class system that is implemented 
   and called back in the windowing library that is in use.
   
   IMPORTANT: The event system from Blind is currently a blocking event system which means that every event that occurs needs to be
   handled immediately as its being instantiated. The EventDispatcher takes care of this.*/

#pragma once
#include <Core.h>
// STD Lib includes
#include <string>
#include <functional>

namespace Blind
{
	// This enum defines every possible type of event that can occur in the current state of the engine
	enum class EventType
	{
		None = 0,
		KeyPressed, KeyReleased,
		WindowClose, WindowResize, WindowMoved, WindowFocus, WindowLostFocus,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		AppTick, AppUpdate, AppRender
	};
	// The enum is used to categories events with a bitfield. BIT macro is defined in the Blind Core Header
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};
// Utility macros that are called in each event class
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type;}\
								virtual EventType GetType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category; }
	class Event
	{
		/* \Event class is a base class for every event type in Blind. Every type of event inherits from it. The
			class implements virtual event utility methods that are going to be overwritten by the inheriting class
			It friends the EventDispatcher class that dispatches events with a certain callback function. */
		friend class EventDispatcher;
	public:

		virtual const char* GetName() const = 0;
		virtual EventType GetType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

		bool m_Handled = false;
	};

	class EventDispatcher
	{
		/* \This EventDispatcher class takes an Event as argument at instantiation which should then be dispatched with
		    the public Dispatch method that takes in a callback function that is being called when an event that has the 
			type of the passed in event occurs. The return value of the callback function is then being assigned to the 
			currently dispatching event. (m_Event) */

		template <typename T>
		using EventFunction = std::function<bool(T)>;
	public:
		// Constructor taking in a Event reference
		EventDispatcher(Event& e)
			: m_Event(e) {}

		/* \Dispatch method calls the passed in callback if the passed in event
		    type as the callback argument is equal to the currently dispatching event (m_Event Constructor parameter) 
			returns true if the currently dispatching event type is the same as the callback argument event type and
			false if not. */
		template <typename T>
		bool Dispatch(EventFunction<T> func )
		{
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		// Currently being dispatched event
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) 
	{
		return os << e.ToString();
	}
}