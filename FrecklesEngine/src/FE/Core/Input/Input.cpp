#include "FE/Core/FEpch.hpp"
#include "FE/Core/Input/Input.hpp"

#include <glfw/glfw3.h>

namespace FE
{
	namespace CORE
	{
		static void* s_InputNativeWindowHandle;
		void Input::Init(void* nativeWindowHandle)
		{
			s_InputNativeWindowHandle = nativeWindowHandle;
		}

		bool Input::Keypressed(const KeyCode key)
		{
			auto* window = static_cast<GLFWwindow*>(s_InputNativeWindowHandle);
			auto state = glfwGetKey(window, static_cast<int32_t>(key));
			return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
		}

		bool Input::MousebuttonPressed(const MouseCode button)
		{
			auto* window = static_cast<GLFWwindow*>(s_InputNativeWindowHandle);
			auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
			return state == GLFW_PRESS;
		}

		glm::vec2 Input::GetMousePosition()
		{
			auto* window = static_cast<GLFWwindow*>(s_InputNativeWindowHandle);
			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);
			return { static_cast<float>(xPos), static_cast<float>(yPos) };
		}

		uint32_t Input::GetMouseX()
		{
			return static_cast<uint32_t>(GetMousePosition().x);
		}

		uint32_t Input::GetMouseY()
		{
			return static_cast<uint32_t>(GetMousePosition().y);
		}

	}
}