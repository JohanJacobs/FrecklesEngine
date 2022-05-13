#include "ImGuiLayer.hpp"
#include "FE/Core/Log/Log.hpp"
#include "FE/Core/Window/Window.hpp"

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"


namespace FE
{
	namespace CORE
	{

		ImGuiLayer::ImGuiLayer(Ref<Window>& window)
			:MainWindow{window}
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);
		}

		void ImGuiLayer::OnUpdate(Timestep ts)
		{
			
		}

		void ImGuiLayer::OnRenderGUI()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);
			if (ShowDemoWindow)
				ImGui::ShowDemoWindow(&ShowDemoWindow);
		}

		void ImGuiLayer::OnAttach()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			GLFWwindow* window = static_cast<GLFWwindow*>(MainWindow->GetWindowHandle());
			ImGui_ImplGlfw_InitForOpenGL(window, true);

			const char* glsl_version = "#version 460";
			ImGui_ImplOpenGL3_Init(glsl_version);

			ShowDemoWindow = true;
		}

		void ImGuiLayer::OnDetach()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			// Cleanup
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void ImGuiLayer::BeginGui()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void ImGuiLayer::EndGui()
		{
			LOG_CORE_TRACE(LOG_FUNCTION_NAME);

			ImGuiIO& io = ImGui::GetIO();
			auto windowSize = MainWindow->GetWindowSize();
			
			io.DisplaySize = ImVec2(windowSize.x, windowSize.y);
			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}
	}
}