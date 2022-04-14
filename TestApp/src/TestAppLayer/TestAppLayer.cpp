#include "TestAppLayer.hpp"

TestApp::TestApp() 
{
    LOG_TRACE("TestAppLayer::TestApp()");
}

TestApp::TestApp(const TestApp& other) 
{
    LOG_TRACE("TestAppLayer::TestApp(const TestApp& other)");
}

TestApp::TestApp(TestApp&& other)  noexcept
{
    LOG_TRACE("TestAppLayer:: TestApp(TestApp&& other) noexcept");
    Shader = std::move(other.Shader);
    VAO = std::move(other.VAO);
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

    RenderCommand::ClearColor(0.1f,0.1f,0.15f,1.0f);
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
         0.0f, 1.0f, 0.0f, // mid top 
        -1.0f,-1.0f, 0.0f,// bottom left 
         1.0f,-1.0f, 0.0f // bottom right 
    };
    int vbSize = sizeof(vertData);
    auto vb = VertexBuffer::Create(vbSize);
    vb->SetData(vertData,vbSize);
    
    BufferLayout bufferLayout{
        {"a_Position",BufferElementType::Float3}
    };
    vb->SetLayout(bufferLayout);

    // index buffer 
    uint32_t indexData[] = {
        0,1,2
    };
    int ibCount = 3;
    auto ib = IndexBuffer::Create(indexData, ibCount);

    VAO->SetVertexBuffer(vb);
    VAO->SetIndexBuffer(ib);

    // shaders
    Shader = Shader::Create("assets/shaders/basic.shader");
}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}