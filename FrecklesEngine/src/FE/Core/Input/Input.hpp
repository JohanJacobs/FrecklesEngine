#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Core/Input/KeyCodes.hpp"
#include "FE/Core/Input/MouseCodes.hpp"

#include <glm/glm.hpp>

namespace FE
{
	namespace CORE
	{
		class Input
		{
		public:
			static void Init(void* nativeWindowHandle);
			static bool Keypressed(const KeyCode key);			
			static bool MousebuttonPressed(const MouseCode button);
			static glm::vec2 GetMousePosition();
			static uint32_t GetMouseX();
			static uint32_t GetMouseY();

		private:			
		};
	}
}
