#include "FE/Core/Base.hpp"

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
        };
    }
}