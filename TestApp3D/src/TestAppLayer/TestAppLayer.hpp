#pragma once 
#include "FE/FrecklesEngine.hpp"
#include <glm/glm.hpp>

class TestApp: public FE::CORE::Layer
{
public:
    TestApp()=default;
    ~TestApp()=default;

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

    void OnWindowResizeEvent(FE::EVENTS::WindowResizeEvent& event);
private:

private:

};