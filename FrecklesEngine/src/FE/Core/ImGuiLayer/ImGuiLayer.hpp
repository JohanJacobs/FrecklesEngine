#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Core/Window/Window.hpp"
#include "FE/Core/Layer/Layer.hpp"

namespace FE
{
	namespace CORE
	{
		class ImGuiLayer : public Layer
		{

		public:
			ImGuiLayer(Ref<Window>& window);
			void OnUpdate(Timestep ts) override;
			void OnRenderGUI() override;
			void OnAttach() override;
			void OnDetach() override;
			
			void BeginGui();
			void EndGui();
		private:
			Ref<Window> MainWindow;
			bool ShowDemoWindow{true};
		};
	}
}
