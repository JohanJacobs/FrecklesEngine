
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"
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

                m_Window->Update();
            }
        }

        void Application::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        }
    }
}