#include "TestAppLayer.hpp"

TestApp::TestApp() 
{
    LOG_INFO("TestAppLayer::TestApp()");
}

TestApp::TestApp(const TestApp& other) 
{
    LOG_INFO("TestAppLayer::TestApp(const TestApp& other)");
}

TestApp::TestApp(TestApp&& other)  noexcept
{
    LOG_INFO("TestAppLayer:: TestApp(TestApp&& other)  noexcept");
}

TestApp::~TestApp()
{
    LOG_INFO("TestAppLayer::~TestApp()");
}

void TestApp::OnUpdate()
{
    LOG_INFO("TestAppLayer::OnUpdate()");
    
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
    LOG_INFO("TestAppLayer::OnAttach()");
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
    LOG_INFO("TestAppLayer::OnDetach()");
}