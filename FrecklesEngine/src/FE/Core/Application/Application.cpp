
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace FE
{
    namespace CORE
    {
        Application::Application()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            m_Window = Window::CreateWindow();
        }

        Application::~Application()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        }

        void Application::Run()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            while (!m_Window->ShouldClose())
            {                
                glClearColor(0.10f,0.10f,0.25f, 1.0f);
                
                glClear(GL_COLOR_BUFFER_BIT);

                m_Window->Update();
            }
        }

        void Application::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        }
    }
}