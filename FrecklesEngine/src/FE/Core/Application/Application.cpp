
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"
#include "FE/Core/Time/Timestep.hpp"

// temp
#include "FE/Renderer/RenderCommand.hpp"
#include "FE/Renderer/Buffers.hpp"
#include "FE/Renderer/VertexArray.hpp"
#include "FE/Renderer/Shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
  
namespace FE
{
    namespace CORE
    {
        Application::Application()
        {
            MainWindow = Window::Create();
            MainWindow->Init();
            RENDERER::RenderCommand::Init(MainWindow);
        }

        Application::~Application()
        {
            Shutdown();
        }

        void Application::Run()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            Timestep ts = MainTimer.GetSeconds();
            MainTimer.Reset();

            while (!MainWindow->ShouldClose())
            {
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
            
            RENDERER::RenderCommand::Shutdown();
        }
    }
}