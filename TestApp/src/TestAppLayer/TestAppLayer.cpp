#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "FE/Renderer/Render2D.hpp"

void TestApp::OnUpdate(FE::CORE::Timestep ts)
{
    LOG_TRACE("TestAppLayer::OnUpdate({})", static_cast<float>(ts));
    Angle += 15.0f * ts;
    if (Angle > 360.0f)
        Angle -=360.0f;

    SmileyPos.x += SmileyVelocity*ts;
    if ((SmileyPos.x >= 4.0f) || (SmileyPos.x <= -4.0f))
        SmileyVelocity *= -1.0f;
    
    using namespace FE;
    using namespace RENDERER;
    using namespace CORE;
    
    cameraController.OnUpdate(ts);
    
    
    // hack to handle window resize 
    auto viewport = RenderCommand::GetWindowSize();
    auto aspectRatio = viewport.x / viewport.y;
    cameraController.SetAspectRatio(aspectRatio);
    RenderCommand::SetViewportSize(0, 0, static_cast<int>(viewport.x), static_cast<int>(viewport.y));
    
    RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
    RenderCommand::Clear();
    
    Render2D::BeginScene(cameraController.GetViewProjection());

    Render2D::RenderQuad({ 3.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.5f,0.8f,0.5f,1.0f });

    Render2D::RenderQuad({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.5f,0.15f,0.5f,1.0f });

    Render2D::RenderQuad({ -3.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.5f,0.5f,0.5f,1.0f },Angle);

    Render2D::RenderTexture(SmileyTexture, SmileyPos, { 1.0f, 1.0f }, { 1.0f, 0.7f, 0.05f, 1.0f });
    Render2D::RenderTexture(CrateTexture, {  0.0f, -3.0f, 0.0f }, { 1.0f, 1.0f }, { 0.5f, 0.15f, 0.5f, 1.0f });
    Render2D::RenderTexture(CrateTexture, { -3.0f, -3.0f, 0.0f }, { 1.0f, 1.0f }, { 0.5f, 0.5f,  0.5f, 1.0f }, Angle);

    Render2D::EndScene();
}

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;
    CrateTexture = Texture2D::Create("assets/textures/container.jpg");    
    SmileyTexture = Texture2D::Create("assets/textures/smiley.png");

    // setup the camera 
    auto viewport = RenderCommand::GetWindowSize();
    OrthographicProperties props;
    props.AspectRatio = viewport.x / viewport.y;
    props.TranslationSpeed = 1.0f;
    props.Size = 10.0f;

    cameraController.SetupCamera(props);
}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}