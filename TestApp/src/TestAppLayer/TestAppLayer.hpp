#pragma once 
#include "FE/FrecklesEngine.hpp"

class TestApp: public FE::CORE::Layer
{
public:
    TestApp();
    TestApp(const TestApp& other) ;
    TestApp(TestApp&& other)  noexcept;

    ~TestApp();

    void OnUpdate() override;
    void OnAttach() override;
    void OnDetach() override;

private:
    Ref<FE::RENDERER::Shader> Shader;
    Ref<FE::RENDERER::VertexArray> VAO;
};