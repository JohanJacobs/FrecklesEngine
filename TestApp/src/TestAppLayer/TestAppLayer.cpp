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


    Render2D::EndScene();
}

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;

    //VAO = VertexArray::Create();

    //// vertex buffer
    //float vertData[] = {
    //    -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top left     / RGBA / texture coordinates
    //    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left  / RGBA / texture coordinates
    //     1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom right / RGBA / texture coordinates
    //     1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f  // top right    / RGBA / texture coordinates
    //};
    //int vbSize = sizeof(vertData);
    //auto vb = VertexBuffer::Create(vbSize);
    //vb->SetData(vertData,vbSize);

    //BufferLayout bufferLayout{
    //    {"a_Position",BufferElementType::Float3},
    //    {"a_Color",BufferElementType::Float4},
    //    {"a_TexCoord",BufferElementType::Float2}
    //};
    //vb->SetLayout(bufferLayout);

    //// index buffer 
    //uint32_t indexData[] = {
    //    0,1,2,2,3,0
    //};
    //int ibCount = 6;
    //auto ib = IndexBuffer::Create(indexData, ibCount);

    //VAO->SetVertexBuffer(vb);
    //VAO->SetIndexBuffer(ib);

    // shaders
    //VertexColorShader = Shader::Create("assets/shaders/CameraVertexColorShaderTexture.shader");
    
    // CrateTexture = Texture2D::Create("assets/textures/container.jpg");
    
    
}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}