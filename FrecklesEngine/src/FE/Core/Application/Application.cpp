
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"
#include "FE/Core/Time/Timestep.hpp"
#include "FE/Core/Events/Eventbus/EventBus.hpp"

// temp
#include "FE/Renderer/RenderCommand.hpp"
//#include "FE/Renderer/Buffers.hpp"
//#include "FE/Renderer/VertexArray.hpp"
//#include "FE/Renderer/Shader.hpp"
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
  
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
            EventBus::AddListener<EVENTS::WindowCloseEvent&>("Application", BINDFN(OnWindowCloseEvent));

            Running = true;
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

                for (auto* l : Layers)
                {
                    l->OnUpdate(ts);
                }
                
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

            EventBus::RemoveListener<EVENTS::WindowCloseEvent&>("Application");

            RENDERER::RenderCommand::Shutdown();
        }

		void Application::OnWindowCloseEvent(EVENTS::WindowCloseEvent& event)
		{
            Running = false;
		}
    }
}