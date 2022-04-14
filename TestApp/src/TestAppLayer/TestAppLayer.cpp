#include "TestAppLayer.hpp"

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
    
    RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
    RenderCommand::Clear();

    VAO->Bind();
    Shader->Bind();
    RenderCommand::DrawIndexed(VAO);
    Shader->Unbind();
    VAO->Unbind();
}

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;

    VAO = VertexArray::Create();

    // vertex buffer
    float vertData[] = {
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top left / RGBA
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,// bottom left / RGBA
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,// bottom right / RGBA
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // top right / RGBA
    };
    int vbSize = sizeof(vertData);
    auto vb = VertexBuffer::Create(vbSize);
    vb->SetData(vertData,vbSize);
    
    BufferLayout bufferLayout{
        {"a_Position",BufferElementType::Float3},
        {"a_Color",BufferElementType::Float4}
    };
    vb->SetLayout(bufferLayout);

    // index buffer 
    uint32_t indexData[] = {
        0,1,2,2,3,0
    };
    int ibCount = 6;
    auto ib = IndexBuffer::Create(indexData, ibCount);

    VAO->SetVertexBuffer(vb);
    VAO->SetIndexBuffer(ib);

    // shaders
    Shader = Shader::Create("assets/shaders/VertexColorShader.shader");
}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}