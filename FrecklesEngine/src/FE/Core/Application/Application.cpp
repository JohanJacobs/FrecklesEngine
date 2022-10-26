#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"
#include "FE/Core/Time/Timestep.hpp"

#include "FE/Core/Events/Events.hpp"

// temp
#include "FE/Renderer/RenderCommand.hpp"

namespace FE
{
    namespace CORE
    {
        Application::Application()
        {
            MainWindow = Window::Create();
            MainWindow->Init();


            RENDERER::RenderCommand::Init(MainWindow);

            // register for events 
            EventBus::AddListener<EVENTS::WindowCloseEvent&>("Application", BIND_EVENT_FN(OnWindowCloseEvent));

            Running = true;

            // GuiLayer
            GuiLayer = CreateRef<ImGuiLayer>(MainWindow);
            GuiLayer->OnAttach();
        }

        Application::~Application()
        {
            Shutdown();
        }


		void Application::Run()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            while (Running)
            {
				Timestep ts = MainTimer.GetSeconds();
				MainTimer.Reset();

                // render main update
                for (auto* l : Layers)
                {
                    l->OnUpdate(ts);
                }

                // render GUI
                GuiLayer->BeginGui();
				for (auto* l : Layers)
				{
                    l->OnRenderGUI();
				}
                GuiLayer->OnRenderGUI();
                GuiLayer->EndGui();

                MainWindow->Update(ts);
            }
        }

        void Application::PushLayer(Layer* layer)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            Layers.PushLayer(layer);
        }

        void Application::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            GuiLayer->OnDetach();

            EventBus::RemoveListener<EVENTS::WindowCloseEvent&>("Application");

            RENDERER::RenderCommand::Shutdown();
        }

		void Application::OnWindowCloseEvent([[maybe_unused]]EVENTS::WindowCloseEvent& event)
		{
            Running = false;
		}
    }
}