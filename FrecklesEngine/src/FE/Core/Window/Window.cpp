#include "FE/Core/FEpch.hpp"
#include "FE/Core/Window/Window.hpp"
#include "FE/Core/Input/Input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace FE
{
    namespace CORE
    {   
        void glfw_error_callback(int error, const char* description)
        {
            LOG_CORE_ERROR("[GLFW] Error Code {0}:{1}",error,description );
        }

        Window::Window()
        {
        }   
        
        Window::~Window()
        {
            Shutdown();
        } 

        void Window::Init()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            // Initialized glfw system
            auto result = glfwInit();
            if (!result)
            {
                LOG_CORE_ERROR("GLFW init failed!");
                exit(1);
            }
            glfwSetErrorCallback(glfw_error_callback);

            // create glfw window
            
            GraphicsContext = RENDERER::Context::Create();

            WindowHandle = glfwCreateWindow(1024, 768, "FrecklesEngine", NULL, NULL);
            if (!WindowHandle)
            {
                LOG_CORE_ERROR("Failed to create window!");
                Shutdown();
                exit(1);
            }

            //setup subsystems
            GraphicsContext->Init(WindowHandle);
            Input::Init(WindowHandle);
        }

        void Window::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            GraphicsContext->Shutdown();

            if (WindowHandle)
                glfwDestroyWindow(WindowHandle);

            glfwTerminate();
        }

        void Window::Update(Timestep ts)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            GraphicsContext->SwapBuffers();
            glfwPollEvents();
        }

        bool Window::ShouldClose() const
        {
            return glfwWindowShouldClose(WindowHandle);
        }

        Ref<Window> Window::Create()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            return CreateRef<Window>();
        }
    }
}