
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"
#include "FE/Core/Renderer/RenderCommand.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace FE
{
    namespace CORE
    {
        Application::Application()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            MainWindow = Window::CreateWindow();
            MainWindow->Init();
            RENDERER::RenderCommand::Init(MainWindow);
        }

        Application::~Application()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        }

        void Application::Run()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            using namespace RENDERER;
            while (!MainWindow->ShouldClose())
            {                
                RenderCommand::ClearColor(0.1f,0.1f,0.15f,1.0f);
                
                RenderCommand::Clear();

                MainWindow->Update();
            }
        }

        void Application::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            RENDERER::RenderCommand::Shutdown();
            MainWindow->Shutdown();
        }
    }
}