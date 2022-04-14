#pragma once 
#include "FE/FrecklesEngine.hpp"

class TestApp: public FE::CORE::Layer
{
public:
    TestApp(); // plain constructor    
    TestApp(const TestApp& other); // copy constructor;
    TestApp(TestApp&& other)  noexcept; // move constructor;
    //TestApp operator = (const TestApp& other);  //copy assignment operator;
    //TestApp operator = (TestApp&& other); // Move assignment operator;

    ~TestApp();

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

private:
    Ref<FE::RENDERER::Shader> Shader;
    Ref<FE::RENDERER::VertexArray> VAO;
};