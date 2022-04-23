#pragma once 
#include "FE/Core/Base.hpp"
#include "FE/Core/Window/Window.hpp"
#include "FE/Renderer/VertexArray.hpp"

#include <glm/glm.hpp>

namespace FE
{
    namespace RENDERER
    {
        class RenderCommand
        {
        public:
            
            static void Init(Ref<CORE::Window>& mainWindow);
            static void Shutdown();

            static void DrawIndexed(Ref<VertexArray>& vao, uint32_t indices=0);
            
            static void ClearColor(const glm::vec4& color);
            static void Clear();

            static glm::vec2 GetWindowSize();
            static void SetViewportSize(int x, int y, int width, int height);

            static void SetFlag(ContextFlags flag, bool state);

        private:            
            struct RenderCommandData
            {
                Ref<RENDERER::Context> GraphicsContext;
            };
            static Ref<RenderCommandData> s_RenderCommandData;
        };
    }
}