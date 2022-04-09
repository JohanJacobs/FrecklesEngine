#pragma once 
#include "FE/Core/Base.hpp"

namespace FE
{
    namespace RENDERER
    {
        class Context
        {
            public:
                Context();
                ~Context();
                void Init(void* windowHandle);
                void Shutdown();

                void SwapBuffers() const;

                void ClearColor(float red,float green,float blue, float alpha) const;
                void Clear() const;
                static Ref<Context> Create();
            private:
                void* WindowHandle;
        };
    }
}