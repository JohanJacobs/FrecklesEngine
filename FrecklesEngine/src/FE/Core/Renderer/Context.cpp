#include "FE/Core/FEpch.hpp"
#include "Context.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace FE
{    
    namespace RENDERER
    {
        void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
        {
            // ignore non-significant error/warning codes
            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

            LOG_WARN("Debug message ({0}): {1}",id,message);

            switch (source)
            {
                case GL_DEBUG_SOURCE_API:             LOG_CORE_WARN("Source: API"); break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG_CORE_WARN("Source: Window System"); break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG_CORE_WARN("Source: Shader Compiler"); break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG_CORE_WARN("Source: Third Party"); break;
                case GL_DEBUG_SOURCE_APPLICATION:     LOG_CORE_WARN("Source: Application"); break;
                case GL_DEBUG_SOURCE_OTHER:           LOG_CORE_WARN("Source: Other"); break;
            } 

            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:               LOG_CORE_WARN("Type: Error"); break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG_CORE_WARN("Type: Deprecated Behaviour"); break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG_CORE_WARN("Type: Undefined Behaviour"); break; 
                case GL_DEBUG_TYPE_PORTABILITY:         LOG_CORE_WARN("Type: Portability"); break;
                case GL_DEBUG_TYPE_PERFORMANCE:         LOG_CORE_WARN("Type: Performance"); break;
                case GL_DEBUG_TYPE_MARKER:              LOG_CORE_WARN("Type: Marker"); break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          LOG_CORE_WARN("Type: Push Group"); break;
                case GL_DEBUG_TYPE_POP_GROUP:           LOG_CORE_WARN("Type: Pop Group"); break;
                case GL_DEBUG_TYPE_OTHER:               LOG_CORE_WARN("Type: Other"); break;
            } 
            
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_WARN("Severity: high"); break;
                case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_WARN("Severity: medium"); break;
                case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN("Severity: low"); break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_WARN("Severity: notification"); break;
            } 
        }

        Ref<Context> Context::Create()
        {
            return CreateRef<Context>();
        }

        Context::Context()
        :WindowHandle{nullptr}
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        Context::~Context()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            
        }

        void Context::Init(void* windowHandle)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
            if (!windowHandle)
            {
                LOG_CORE_ERROR("Null handle for window!");                
            }

            WindowHandle = windowHandle;

            glfwMakeContextCurrent(static_cast<GLFWwindow*>(WindowHandle));

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                LOG_ERROR("gladLoadGLLoader error!");
                exit(1);
            } 
            
            // register error callback
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            
            // print the opengl version of the window.
            LOG_INFO("OpenGL Info:");
            LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
            LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
            LOG_INFO("  Version: {0}", glGetString(GL_VERSION));
        }
        void Context::Shutdown()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);
        }

        void Context::SwapBuffers() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glfwSwapBuffers(static_cast<GLFWwindow*>(WindowHandle));
        }

        void Context::ClearColor(float red,float green,float blue, float alpha) const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glClearColor(red, green, blue, alpha);
        }

        void Context::Clear() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }        
    }
}