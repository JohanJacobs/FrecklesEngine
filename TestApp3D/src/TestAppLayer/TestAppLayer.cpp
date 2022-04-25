#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

void TestApp::OnUpdate(FE::CORE::Timestep ts)
{
    LOG_TRACE("TestAppLayer::OnUpdate({})", static_cast<float>(ts));

    using namespace FE;
    using namespace RENDERER;
    using namespace CORE;
}

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;

    //register for events
    EventBus::AddListener<EVENTS::WindowResizeEvent&>("TestApp", BIND_EVENT_FN(OnWindowResizeEvent));


}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}

void TestApp::OnWindowResizeEvent(FE::EVENTS::WindowResizeEvent& event)
{
    float aspectRatio = event.GetWidth() / static_cast<float>(event.GetHeight());    
}