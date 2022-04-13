#pragma once 
#include "FE/Core/Base.hpp"
namespace FE
{
    namespace CORE
    {
        class Layer
        {
            public:
                Layer() =default;
                virtual ~Layer() = default;
                                 
                virtual void OnUpdate(){};
                virtual void OnRenderGUI(){};

                virtual void OnAttach(){};
                virtual void OnDetach(){};
        };
    }

}
