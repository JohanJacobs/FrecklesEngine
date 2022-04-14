#pragma once 
#include "FE/FrecklesEngine.hpp"

class TestApp: public FE::CORE::Layer
{
public:
    TestApp(); // default constructor    
    ~TestApp();

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

private:
    Ref<FE::RENDERER::Shader> Shader;
    Ref<FE::RENDERER::VertexArray> VAO;    
};