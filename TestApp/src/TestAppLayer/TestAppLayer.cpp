#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "FE/Renderer/Render2D.hpp"
#include "imgui.h"

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

    ViewPortFrameBuffer->Bind();
    DrawMainScene();
    ViewPortFrameBuffer->Unbind();
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
    ViewPortFrameBuffer = FrameBuffer::Create((uint32_t)viewport.x, (uint32_t)viewport.y);
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

void TestApp::OnRenderGUI()
{
    bool main_window_open = true;
    bool opt_fullscreen = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None ;

    //main window as the background
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // remove window padding
    ImGui::Begin("TestApp", &main_window_open, window_flags);
    ImGui::PopStyleVar();

	// DockSpace setup
	ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

    // Menu 
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                LOG_CORE_ERROR("Seriously cant quit application yet?");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }


    // Render our scene to the viewport
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    uint64_t textureID = ViewPortFrameBuffer->GetColorAtachment()->GetRenderID();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::End();
    

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
    RenderCommand::SetFlag(ContextRenderFlags::DepthTest, false);

    GrayShader->Bind();    
    GrayShader->SetUniform("u_Texture", 0);
    MainFrameBuffer->GetColorAtachment()->Bind(0);
    RearviewVAO->Bind();
    RenderCommand::DrawIndexed(RearviewVAO, 6);
    RearviewVAO->Unbind();
    GrayShader->Unbind();        
    RenderCommand::SetFlag(ContextRenderFlags::DepthTest, true);
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
