#pragma once 
#include "FE/Core/Base.hpp"
#include <glm/glm.hpp>
namespace FE
{
    namespace RENDERER
    {
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

                // flags 
                void SetDepthTest(bool value);

                glm::vec2 GetWindowSize() const;
                void SetViewportSize(int x, int y, int width, int height) const;
                static Ref<Context> Create();
            private:
                void* WindowHandle;
        };
    }
}