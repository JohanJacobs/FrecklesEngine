#pragma once 
#include "FE/Core/Base.hpp"
#include "FE/Core/Window/Window.hpp"
namespace FE
{
    namespace RENDERER
    {
        class RenderCommand
        {
        public:
            
            static void Init(Ref<CORE::Window>& mainWindow);
            static void Shutdown();

            static void ClearColor(float red,float green,float blue, float alpha);
            static void Clear();
        private:
            struct RenderCommandData
            {
                Ref<RENDERER::Context> GraphicsContext;
            };
            static Ref<RenderCommandData> s_RenderCommandData;
        };
    }
}