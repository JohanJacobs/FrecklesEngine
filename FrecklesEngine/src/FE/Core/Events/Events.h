#pragma once
#include <string>

namespace FE
{
	namespace EVENTS
	{
		enum class EventCategory
		{
			Application,
			Input
		};

		enum class EventType
		{
			WindowResizeEvent, WindowCloseEvent
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
