
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"

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
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            MainWindow = Window::Create();
            MainWindow->Init();
            RENDERER::RenderCommand::Init(MainWindow);
        }

        Application::~Application()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            Shutdown();
        }

        void Application::Run()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            using namespace RENDERER;
            auto VAO = VertexArray::Create();

            // vertex buffer
            float vertData[] = {
                 0.0f, 1.0f, 0.0f, // mid top 
                -1.0f,-1.0f, 0.0f,// bottom left 
                 1.0f,-1.0f, 0.0f // bottom right 
            };
            int vbSize = sizeof(vertData);
            auto vb = VertexBuffer::Create(vbSize);
            vb->SetData(vertData,vbSize);
            
            BufferLayout bufferLayout{
                {"a_Position",BufferElementType::Float3}
            };
            vb->SetLayout(bufferLayout);

            // index buffer 
            uint32_t indexData[] = {
                0,1,2
            };
            int ibCount = 3;
            auto ib = IndexBuffer::Create(indexData, ibCount);

            VAO->SetVertexBuffer(vb);
            VAO->SetIndexBuffer(ib);

            auto shader = Shader::Create("assets/shaders/basic.shader");
            while (!MainWindow->ShouldClose())
            {
                for (auto* l : Layers)
                {
                    l->OnUpdate();
                }
                                
                RenderCommand::ClearColor(0.1f,0.1f,0.15f,1.0f);
                RenderCommand::Clear();

                VAO->Bind();
                shader->Bind();
                RenderCommand::DrawIndexed(VAO);
                shader->Unbind();
                VAO->Unbind();
                
                MainWindow->Update();
            }
        }

        void Application::PushLayer(Layer* layer)
        {
            Layers.PushLayer(layer);
        }
        void Application::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            RENDERER::RenderCommand::Shutdown();
        }
    }
}