#include "FE/Core/Base.hpp"
#include "FE/Core/Window/Window.hpp"

namespace FE
{
    namespace CORE
    {
        class Application
        {
        public:
            Application();
            
            void Run();
            void Shutdown();
            ~Application();

        private:
            Ref<Window> m_Window;
        };
    }
}