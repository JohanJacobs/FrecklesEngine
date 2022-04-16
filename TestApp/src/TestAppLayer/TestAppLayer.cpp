#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "FE/Renderer/Render2D.hpp"

// image is not square like the window.

TestApp::TestApp() 
{
    LOG_TRACE("TestAppLayer::TestApp()");
}

TestApp::~TestApp()
{
    LOG_TRACE("TestAppLayer::~TestApp()");
}

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

    // moving the camera 
    if (Input::Keypressed(KeyCode::W)) // up
    {
        cameraPosition.y += 1.0f * ts;
    }
    else if (Input::Keypressed(KeyCode::S)) // down
    {
        cameraPosition.y -= 1.0f * ts;
    }

    if (Input::Keypressed(KeyCode::A)) // left
    {
        cameraPosition.x -= 1.0f * ts;
	}
    else if (Input::Keypressed(KeyCode::D)) // right
    {
        cameraPosition.x += 1.0f * ts;
    }

	if (Input::Keypressed(KeyCode::LeftBracket)) // left
	{
        cameraPosition.z += 1.0f * ts;
	}
	else if (Input::Keypressed(KeyCode::RightBracket)) // right
	{
        cameraPosition.z -= 1.0f * ts;
        cameraPosition.z = std::max(0.01f, cameraPosition.z);
	}
    
    auto halfCameraDist= cameraPosition.z /2.0f;
    
    auto viewport = RenderCommand::GetWindowSize();
    auto aspectRatio = viewport.x / viewport.y;
    RenderCommand::SetViewportSize(0, 0, static_cast<int>(viewport.x), static_cast<int>(viewport.y));

    cameraTransform = glm::translate(glm::mat4(1.0f), cameraPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(cameraRotation.z), glm::vec3(0, 0, 1));
    auto view = glm::inverse(cameraTransform);
    auto proj = glm::ortho(-aspectRatio * halfCameraDist, aspectRatio * halfCameraDist, -halfCameraDist, halfCameraDist, 0.0f, 1000.0f);
    auto viewProj = proj * view;
    
    RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
    RenderCommand::Clear();
    
    Render2D::BeginScene(viewProj);

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
}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}