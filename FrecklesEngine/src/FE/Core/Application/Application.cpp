
#include "FE/Core/FEpch.hpp"
#include "FE/Core/Application/Application.hpp"
#include "FE/Core/Renderer/RenderCommand.hpp"


// temp
#include "FE/Core/Renderer/Buffers.hpp"
#include "FE/Core/Renderer/VertexArray.hpp"
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
            using namespace RENDERER;
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            //uint32_t vao;
            auto VAO = VertexArray::Create();
            //glCreateVertexArrays(1,&vao);

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

            // link to vao
            // glVertexArrayVertexBuffer(vao, 0, vb->GetRenderID(), 0,sizeof(float)*3); 
            // auto l  = vb->GetLayout();
            // {
            //     int elementIndex = 0;
            //     int bindingIndex = 0;

            //     for (auto& element : l)
            //     {
            //         glVertexArrayAttribBinding(vao, elementIndex, bindingIndex);
            //         glVertexArrayAttribFormat(vao, elementIndex, element.Count ,BufferElementToOpenGLType(element.Type), element.Normalized,element.Offset);
            //         glEnableVertexArrayAttrib(vao, elementIndex);
            //     }

            // }
            //glVertexArrayAttribBinding(vao, 0, 0);
            //glVertexArrayAttribFormat(vao, 0, 2,GL_FLOAT, GL_FALSE,0);
            //glEnableVertexArrayAttrib(vao,0);

            //glVertexArrayElementBuffer(vao, ib->GetRenderID());
            //glBindVertexArray(vao);

            VAO->SetVertexBuffer(vb);
            VAO->SetIndexBuffer(ib);
            // shader 
            std::string vertShaderCode = R"(
                #version 460 core
                layout(location=0) in vec3 a_Position;
                void main()
                {
                    gl_Position = vec4(a_Position, 1.0);
                }
            )";
            unsigned int vertShader;
            vertShader = glCreateShader(GL_VERTEX_SHADER);
            const char* vSrc = vertShaderCode.c_str();
            glShaderSource(vertShader, 1, &vSrc, NULL);
            glCompileShader(vertShader);
            {
                int  success;
                char infoLog[512];
                glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

                if(!success)
                {
                    glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
                    LOG_CORE_ERROR("Vertex Shader compilation failed: {}", infoLog );
                }
            }

            std::string fragmentShader = R"(
                #version 460 core
                out vec4 color;
                void main()
                {
                    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);   
                }
            )";
            const char* fSrc = fragmentShader.c_str();
            unsigned int fragShader;
            fragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragShader, 1, &fSrc, NULL);
            glCompileShader(fragShader);

            {
                int  success;
                char infoLog[512];
                glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

                if(!success)
                {
                    glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
                    LOG_CORE_ERROR("Fragment Shader compilation failed: {}", infoLog );
                }
            }

            unsigned int shaderProgram; 
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram,vertShader);
            glAttachShader(shaderProgram,fragShader);
            glLinkProgram(shaderProgram);
            {
                int success;
                char infoLog[512];
                glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
                if(!success) {
                    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                    LOG_CORE_ERROR("Shader linking failed: {}", infoLog );
                }
            }
            glUseProgram(shaderProgram);
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            glUseProgram(0);

            using namespace RENDERER;
            while (!MainWindow->ShouldClose())
            {                
                RenderCommand::ClearColor(0.1f,0.1f,0.15f,1.0f);                
                RenderCommand::Clear();
                
                //glDrawElements(GL_TRIANGLES,ib->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
                VAO->Bind();
                glUseProgram(shaderProgram);
                auto idxcount = VAO->GetIndexCount();
                glDrawElements(GL_TRIANGLES,VAO->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
                glUseProgram(0);
                VAO->Unbind();

                //glDrawArrays(GL_TRIANGLES, 0, 3);
                MainWindow->Update();
            }

            // delete             
            //glDeleteVertexArrays(1,&vao);

        }

        void Application::Shutdown()
        {            
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            RENDERER::RenderCommand::Shutdown();
        }
    }
}