#pragma once
#include "FE/Core/Base.hpp"
#include "FE/Renderer/Context.hpp"
#include "FE/Core/Time/Timestep.hpp"

#include "FE/Core/Events/ApplicationEvents.hpp"
#include "FE/Core/Events/Eventbus/EventBus.hpp"

struct GLFWwindow;
namespace FE
{
    namespace CORE
    {
        class Window
        {
        public:
            Window()=default;
            ~Window();

            void Update(Timestep ts);

            bool ShouldClose() const;

            void Init();
            void Shutdown();
            Ref<RENDERER::Context>& GetGraphicsContext() { return GraphicsContext;}
            static Ref<Window> Create();
        
        private: 
            void SetupCallbackFunctions();
            void OnWindowResizeEvent(EVENTS::WindowResizeEvent& event);
        private:
            GLFWwindow* WindowHandle;
            Ref<RENDERER::Context> GraphicsContext;
            struct WindowData
            {
                GLFWwindow* WindowHandle;
            } WindowUserData;
        };
    }
}