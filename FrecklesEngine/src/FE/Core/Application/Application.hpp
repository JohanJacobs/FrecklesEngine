#include "FE/Core/Base.hpp"
#include "FE/Core/Window/Window.hpp"
#include "FE/Core/Layer/LayerStack.hpp"

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
            Ref<Window> MainWindow;
            LayerStack Layers;
        };
    }
}