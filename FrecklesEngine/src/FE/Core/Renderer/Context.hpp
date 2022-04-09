#include "FE/Core/Base.hpp"

namespace FE
{
    namespace Renderer
    {
        class Context
        {
            public:
                Context();
                ~Context();
                void Init(void* windowHandle);
                void SwapBuffers()const;
                static Ref<Context> Create();
            private:
                void* WindowHandle;
        };
    }
}