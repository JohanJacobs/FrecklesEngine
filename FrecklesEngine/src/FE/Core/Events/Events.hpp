#pragma once
#include <string>
#include "FE/Core/Events/Eventbus/EventBus.hpp"

namespace FE
{
	namespace EVENTS
	{
		enum class EventCategory
		{
			Application,
			Mouse
		};

		enum class EventType
		{
			WindowResizeEvent, WindowCloseEvent, MouseMoveEvent, MouseScrollEvent, MouseButton
		};

		class Event
		{
		public:
			virtual ~Event() {}
			virtual EventCategory GetCategory() const = 0;
			virtual EventType GetEventType() const = 0;
			virtual std::string ToString() const = 0;
		};
	}
}
