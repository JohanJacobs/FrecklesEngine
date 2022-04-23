#pragma once
#include "Events.h"

namespace FE
{
	namespace EVENTS
	{
		class ApplicationEvent : public Event
		{
		public:

			EventCategory GetCategory() const override
			{
				return EventCategory::Application;
			}
		private:
		};

		class WindowResizeEvent : public ApplicationEvent
		{
		public:
			WindowResizeEvent(uint32_t width, uint32_t height)
				:Width{ width }, Height{ height } {}
			EventType GetEventType() const override
			{
				return EventType::WindowResizeEvent;
			}

			std::string ToString() const override
			{
				return "Window Resize Event";
			}

			uint32_t GetWidth() const { return Width; }
			uint32_t GetHeight() const { return Height; }

		private:
			uint32_t Width, Height;
		};

		class WindowCloseEvent : public ApplicationEvent
		{
		public:

			EventType GetEventType() const override
			{
				return EventType::WindowCloseEvent;
			}


			std::string ToString() const override
			{
				return "Window close Event";
			}

		private:
		};
	}
}
