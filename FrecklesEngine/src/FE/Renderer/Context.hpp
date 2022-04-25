#pragma once 
#include "FE/Core/Base.hpp"
#include <glm/glm.hpp>
namespace FE
{
    namespace RENDERER
    {
        enum class ContextRenderFlags
        {
            DepthTest
        };

        enum class ContextWindowFlags
        {
            MouseCapture
        };
        class Context
        {
            public:
                Context();
                ~Context();
                void Init(void* windowHandle);
                void Shutdown();

                void DrawIndexed(uint32_t indices);
                
                void SwapBuffers() const;
                void ClearColor(const glm::vec4& color) const;
                void Clear() const;

                glm::vec2 GetWindowSize() const;
                void SetViewportSize(int x, int y, int width, int height) const;

                void EnableFlag(ContextRenderFlags flag);
                void DisableFlag(ContextRenderFlags flag);

				void EnableFlag(ContextWindowFlags flag);
				void DisableFlag(ContextWindowFlags flag);
                
                static Ref<Context> Create();
            private:
                void* WindowHandle;
        };
    }
}