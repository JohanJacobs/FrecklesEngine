#pragma once 
#include "FE/Core/Base.hpp"
#include "FE/Core/Time/Timestep.hpp"
namespace FE
{
    namespace CORE
    {
        class Layer
        {
            public:
                Layer() =default;
                virtual ~Layer() = default;
                                 
                virtual void OnUpdate(Timestep ts){};
                virtual void OnRenderGUI(){};

                virtual void OnAttach(){};
                virtual void OnDetach(){};
        };
    }

}
