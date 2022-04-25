#pragma once 
#include "FE/Core/Base.hpp"
#include "FE/Core/Window/Window.hpp"
#include "FE/Core/Layer/LayerStack.hpp"
#include "FE/Core/Time/Timer.hpp"

#include "FE/Core/Events/Events.hpp"
#include "FE/Core/Events/ApplicationEvents.hpp"
namespace FE
{
    namespace CORE
    {
        class Application
        {
        public:
            Application();
            
            void Run();

            void PushLayer(Layer* layer);
            
            void Shutdown();
                        
            ~Application();
        private:
            void OnWindowCloseEvent(EVENTS::WindowCloseEvent& event);

        private:
            Ref<Window> MainWindow;
            LayerStack Layers;
            Timer MainTimer;

            bool Running{false};
        };
    }
}