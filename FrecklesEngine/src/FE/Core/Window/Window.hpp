#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Renderer/Context.hpp"

struct GLFWwindow;
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
            static Ref<Window> Create();
        private:
        private:
            GLFWwindow* WindowHandle;
            Ref<RENDERER::Context> GraphicsContext;
        };
    }
}