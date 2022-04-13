#pragma once 

#include "FE/Core/Base.hpp"
#include "FE/Core/Layer/Layer.hpp"
#include <vector>

namespace FE
{
    namespace CORE
    {
        class LayerStack
        {
            public:
                LayerStack() = default;
                ~LayerStack()
                {
                    for (Layer* l : Layers)
                        {
                            l->OnDetach();
                            delete l;
                        }
                }

                void PushLayer(Layer* layer)
                {
                    Layers.emplace_back(layer);
                    layer->OnAttach();
                }
                
                std::vector<Layer*>::iterator begin() { return Layers.begin();}
                std::vector<Layer*>::iterator end() { return Layers.end();}
            private:
            std::vector<Layer*> Layers;
        };
    }
}