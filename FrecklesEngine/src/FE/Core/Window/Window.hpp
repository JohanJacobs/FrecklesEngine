#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Renderer/Context.hpp"
#include "FE/Core/Time/Timestep.hpp"

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

            void Update(Timestep ts);

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