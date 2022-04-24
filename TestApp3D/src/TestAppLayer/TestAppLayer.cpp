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
    auto windowSize= RenderCommand::GetWindowSize();
    auto aspectRatio = windowSize.x / windowSize.y;
    
    MainFrameBuffer->Bind();
    DrawDemoScene();
    MainFrameBuffer->Unbind();

    DrawMainScene();
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
        
    MainFrameBuffer = FrameBuffer::Create((uint32_t)viewport.x, (uint32_t)viewport.y);
    
    GrayShader = Shader::Create("assets/shaders/gray.shader");

    SetupRearviewVAO();
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
    cameraController.SetAspectRatio(aspectRatio);
}

void TestApp::DrawDemoScene()
{
	using namespace FE;
	using namespace RENDERER;
	using namespace CORE;

	
	RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
	RenderCommand::Clear();

	Render2D::BeginScene(cameraController.GetViewProjection());

	Render2D::RenderQuad({ 3.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.5f,0.8f,0.5f,1.0f });

	Render2D::RenderQuad({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.5f,0.15f,0.5f,1.0f });

	Render2D::RenderQuad({ -3.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.5f,0.5f,0.5f,1.0f }, Angle);

	Render2D::RenderTexture(SmileyTexture, SmileyPos, { 1.0f, 1.0f }, { 1.0f, 0.7f, 0.05f, 1.0f });
	Render2D::RenderTexture(CrateTexture, { 0.0f, -3.0f, 0.0f }, { 1.0f, 1.0f }, { 0.5f, 0.15f, 0.5f, 1.0f });
	Render2D::RenderTexture(CrateTexture, { -3.0f, -3.0f, 0.0f }, { 1.0f, 1.0f }, { 0.5f, 0.5f,  0.5f, 1.0f }, Angle);

	Render2D::EndScene();   
}

void TestApp::DrawMainScene()
{
	using namespace FE;
	using namespace RENDERER;
	using namespace CORE;
    
    DrawDemoScene();

    //draw the framebuffer
    RenderCommand::SetFlag(ContextFlags::DepthTest, false);

    GrayShader->Bind();
    //GrayShader->SetUniform("u_Texture", (int)MainFrameBuffer->GetColorAtachment()->GetRenderID());
    GrayShader->SetUniform("u_Texture", 0);
    MainFrameBuffer->GetColorAtachment()->Bind(0);
    RearviewVAO->Bind();
    RenderCommand::DrawIndexed(RearviewVAO, 6);
    RearviewVAO->Unbind();
    GrayShader->Unbind();

    RenderCommand::SetFlag(ContextFlags::DepthTest, true);
}

void TestApp::SetupRearviewVAO()
{
    using namespace FE;
    using namespace CORE;
    using namespace RENDERER;
    
    glm::vec3 MirrorVertData[4];
    MirrorVertData[0] = { -1.0f, 1.0f, 0.0f }; // left top 
    MirrorVertData[1] = { -1.0f, 0.35f, 0.0f }; // left bottom
    MirrorVertData[2] = { -0.35f, 0.35f, 0.0f }; // right bottom
    MirrorVertData[3] = { -0.35f, 1.0f, 0.0f };  // right top 

	glm::vec2 defaultQuadTextCoords[4] =
	{
		{ 0.0f, 1.0f }, // Top Left
		{ 0.0f, 0.0f }, // Bottom Left
		{ 1.0f, 0.0f }, // Bottom Right
		{ 1.0f, 1.0f } // Top Right
	};

    uint32_t defaultQuadIndices[6] = { 0, 1, 2, 2, 3, 0 };

    glm::vec4 defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	
    struct VertDataStruct
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextCoord;
        int TextureIndex;
        int TilingFactor;
    };

	VertDataStruct QuadVertexData[4];
	for (int i = 0; i < 4; i++)
	{
		QuadVertexData[i].Position = MirrorVertData[i]; // scale to half size
		QuadVertexData[i].Color = defaultColor;
		QuadVertexData[i].TextCoord = defaultQuadTextCoords[i];
		QuadVertexData[i].TextureIndex = 0;
		QuadVertexData[i].TilingFactor = 1;
	}

	uint32_t quad_vb_size = sizeof(VertDataStruct) * 4;

    auto vb = VertexBuffer::Create(quad_vb_size);
    BufferLayout layout{
		{"a_Position",      BufferElementType::Float3},
	    {"a_Color",         BufferElementType::Float4},
	    {"a_TexCoord",      BufferElementType::Float2},
	    {"a_TexIndex",      BufferElementType::Int},
	    {"a_TilingFactor",  BufferElementType::Int},
    };
    vb->SetLayout(layout);
    vb->SetData(QuadVertexData, quad_vb_size);
    auto ib = IndexBuffer::Create(defaultQuadIndices, 6);

    RearviewVAO = VertexArray::Create(vb, ib);
    RearviewVAO->Unbind();
}
