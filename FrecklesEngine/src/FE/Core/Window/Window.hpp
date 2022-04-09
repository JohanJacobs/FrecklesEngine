#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Core/Renderer/Context.hpp"

class GLFWwindow;
namespace FE
{
    namespace CORE
    {
        class Window
        {
        public:
            Window();
            ~Window();

            void Update();

            bool ShouldClose() const;

            void Init();
            void Shutdown();
            Ref<RENDERER::Context>& GetGraphicsContext() { return GraphicsContext;}
            static Ref<Window> CreateWindow();
        private:
        private:
            GLFWwindow* WindowHandle;
            Ref<RENDERER::Context> GraphicsContext;
        };
    }
}