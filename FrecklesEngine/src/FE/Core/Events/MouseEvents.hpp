#pragma once
#include "FE/Core/Events/Events.hpp"
#include  "FE/Core/Input/MouseCodes.hpp"
namespace FE
{
	namespace EVENTS
	{
		class MouseEvent : public Event
		{
		public:

			EventCategory GetCategory() const override
			{
				return EventCategory::Mouse;
			}

		private:
		};

		class MouseMoveEvent : public MouseEvent
		{

		public:
			MouseMoveEvent(uint32_t x, uint32_t y)
				:XPos{ x }, YPos{ y }{}

			EventType GetEventType() const override
			{
				return EventType::MouseMoveEvent;
			}

			std::string ToString() const override
			{
				return "MouseMove";
			}

			uint32_t GetXPos() const { return XPos; }
			uint32_t GetYPos() const { return YPos; }
		private:
			uint32_t XPos, YPos;
		};

		class MouseScrollEvent : public MouseEvent
		{

		public:
			MouseScrollEvent(float xOffset, float yOffset)
				:XOffset{ xOffset }, YOffset{ yOffset }{}

			EventType GetEventType() const override
			{
				return EventType::MouseMoveEvent;
			}

			std::string ToString() const override
			{
				return "Mouse Scroll Event";
			}

			float GetXOffset() const { return XOffset; }
			float GetYOffset() const { return YOffset; }
		private:
			float XOffset, YOffset;
		};


		class MouseButtonEvent: public MouseEvent
		{
		public:
			MouseButtonEvent(MouseCode button, bool pressedState)
				:Button{ button }, PressedState{ pressedState } {}
			
			MouseCode GetButton() const { return Button; }			
			
			bool Pressed() const { return PressedState; }
						

			EventType GetEventType() const override
			{
				return EventType::MouseButton;
			}

			std::string ToString() const override
			{
				if (PressedState)
					return "Mouse Button Pressed";
				else 
					return "Mouse Button Released";
			}

		protected:
			MouseCode Button;
			bool PressedState;
		};

	}
}
