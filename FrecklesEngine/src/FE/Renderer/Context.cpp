#include "FE/Core/FEpch.hpp"
#include "FE/Renderer/Context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

            LOG_WARN("Message ({0}): {1}",id,message);
        }


		Ref<Context> Context::Create()
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            return CreateRef<Context>();
        }

        Context::Context()
        :WindowHandle{nullptr}
        {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        Context::~Context()
        {            
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
          
            LOG_CORE_TRACE("OpenGL Info:");
            LOG_CORE_TRACE("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            LOG_CORE_TRACE("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            LOG_CORE_TRACE("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));


            // blending / depth testing information 
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);

            // set viewport to match window size
            auto viewport = GetWindowSize();
            SetViewportSize(0, 0, int(viewport.x), int(viewport.y));
        }
        
        void Context::Shutdown()
        {
        }

        /*
            Shaders and VAO should be bound prior to calling DrawIndexed
        */
        void Context::DrawIndexed(uint32_t indices)
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
        }

        void Context::SwapBuffers() const
        {
            LOG_CORE_TRACE(LOG_FUNCTION_NAME);

            glfwSwapBuffers(static_cast<GLFWwindow*>(WindowHandle));
        }

        void Context::ClearColor(const glm::vec4& color) const
        {
            glClearColor(color.r,color.g,color.b,color.a);
        }

        void Context::Clear() const
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }        

        void Context::SetDepthTest(bool value)
        {
            if (value)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
        }

		glm::vec2 Context::GetWindowSize() const
		{
            int width{0}, height{ 0 };
            glfwGetWindowSize(static_cast<GLFWwindow*>(WindowHandle), &width, &height);
            return { width,height };
		}

		void Context::SetViewportSize(int x, int y, int width, int height) const
		{
            glViewport(x, y, width, height);
		}

	}
}