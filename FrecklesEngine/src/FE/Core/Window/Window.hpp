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

            static Ref<Window> CreateWindow();
        private:
            void Init();
            void Shutdown();
        private:
            GLFWwindow* WindowHandle;
            Ref<FE::Renderer::Context> GraphicsContext;
        };
    }
}