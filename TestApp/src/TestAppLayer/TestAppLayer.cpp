#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "FE/Renderer/Render2D.hpp"

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;

    //setup a VAO for the quad to render 
    uint32_t quad_vb_size = sizeof(VertexData) * 4;
    glm::vec4 defaultColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    QuadVB = VertexBuffer::Create(quad_vb_size);
	BufferLayout layout({
	{"a_Position", BufferElementType::Float3},
	{"a_Color", BufferElementType::Float4},
	{"a_TexCoord", BufferElementType::Float2},
	{"a_TexIndex", BufferElementType::Int},
	{"a_TilingFactor", BufferElementType::Int},
		});
    QuadVB->SetLayout(layout);
    VertexData QuadVertexData[4];
    for (int i = 0; i < 4; i++)
    {
        QuadVertexData[i].Position = defaultQuadVertices[i] *0.5f; // scale to half size
        QuadVertexData[i].Color = defaultColor;
        QuadVertexData[i].TexCoord = defaultQuadTextCoords[i];
        QuadVertexData[i].TexIndex = 0;
        QuadVertexData[i].TilingFactor = 1;
    }

    QuadVB->SetData(QuadVertexData, quad_vb_size);
    QuadIB = IndexBuffer::Create(defaultQuadIndices, 6);
    QuadVAO = VertexArray::Create(QuadVB, QuadIB);
    QuadVAO->Unbind();
    SmileyTexture = Texture2D::Create("assets/textures/smiley.png");
    SmileyTexture->Unbind();
    CrateTexture = Texture2D::Create("assets/textures/container.jpg");
    CrateTexture->Unbind();
    // load shaders
    defaultShader = Shader::Create("assets/shaders/simpleShader.shader");
	int textureSlots[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
    defaultShader->Bind();
	defaultShader->SetUniform("u_Textures", textureSlots, 32);
    defaultShader->Unbind();

    screenShader = Shader::Create("assets/shaders/screenShader.shader");
    GrayScaleShader = Shader::Create("assets/shaders/GrayScaleShader.shader");
    BlurShader = Shader::Create("assets/shaders/blurShader.shader");
    // QUAD FOR FRAMEBUFFER
    FBVB = VertexBuffer::Create(quad_vb_size);
    FBVB->SetLayout(layout);

	for (int i = 0; i < 4; i++)
	{	
        GrayscaleVertData[i].Position = defaultQuadVertices[i]; // scale to half size
        GrayscaleVertData[i].Color = defaultColor;
        GrayscaleVertData[i].TexCoord = defaultQuadTextCoords[i];
        GrayscaleVertData[i].TexIndex = 0;
        GrayscaleVertData[i].TilingFactor = 1;
	}
    GrayscaleVertData[0].Position = { -1.0f, 1.0f, 0.0f }; // left top 
    GrayscaleVertData[1].Position = { -1.0f, 0.35f, 0.0f }; // left bottom
    GrayscaleVertData[2].Position = { -0.35f, 0.35f, 0.0f }; // right bottom
    GrayscaleVertData[3].Position = { -0.35f, 1.0f, 0.0f};  // right top 

	// Blurring Effect
	for (int i = 0; i < 4; i++)
	{
	
		BlurVertData[i].Color = defaultColor;
		BlurVertData[i].TexCoord = defaultQuadTextCoords[i];
		BlurVertData[i].TexIndex = 0;
		BlurVertData[i].TilingFactor = 1;
	}
    BlurVertData[0].Position = { 0.35f, 1.0f , 0.0f }; // left top 
    BlurVertData[1].Position = { 0.35f, 0.35f, 0.0f }; // left bottom
    BlurVertData[2].Position = { 1.0f , 0.35f, 0.0f }; // right bottom
    BlurVertData[3].Position = { 1.0f , 1.0f , 0.0f };  // right top 

    //FBVB->SetData(FBVertexData, quad_vb_size);
	FBIB = IndexBuffer::Create(defaultQuadIndices, 6);
    FBVAO = VertexArray::Create(FBVB, FBIB);
    FBVAO->Unbind();

    FB = FrameBuffer::Create(uint32_t(RenderCommand::GetWindowSize().x), uint32_t(RenderCommand::GetWindowSize().y));

    //setup camera
    OrthographicProperties props;
    props.AspectRatio = RenderCommand::GetWindowSize().x  / RenderCommand::GetWindowSize().y;
    props.Size = 10;
    props.Position = { 0.0f,0.0f,10.0f };
    props.NearClip = 0.0f;
    props.FarClip = 100.0f;
    props.TranslationSpeed = 5.0f;
    cameraController.SetupCamera(props);
}

void TestApp::OnUpdate(FE::CORE::Timestep ts)
{
    using namespace FE;
    using namespace RENDERER;
    using namespace CORE;

	Angle += 15.0f * ts;
	if (Angle > 360.0f)
		Angle -= 360.0f;

	SmileyPos.x += SmileyVelocity * ts;
	if ((SmileyPos.x >= 4.0f) || (SmileyPos.x <= -4.0f))
		SmileyVelocity *= -1.0f;

    if (Input::Keypressed(KeyCode::RightBracket))
    {
        auto size = cameraController.GetSize();
        size += 5.0f*ts;
        cameraController.SetSize(size);
        LOG_INFO("Camera size {}", size);

    }

	if (Input::Keypressed(KeyCode::LeftBracket))
	{
		auto size = cameraController.GetSize();
		size -= 5.0f * ts;
		cameraController.SetSize(size);
        LOG_INFO("Camera size {}", size);
        		
	}

    if (Input::Keypressed(KeyCode::T) && framebufferOn == true)
    { 
        framebufferOn = false;
        LOG_INFO("Framebuffer off");
    }
    if (Input::Keypressed(KeyCode::Y) && framebufferOn == false)
    {
        framebufferOn = true;
        LOG_INFO("Framebuffer on");
    }

    cameraController.OnUpdate(ts);

    if (framebufferOn)
        FB->Bind();
    
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

    if (framebufferOn)
        FB->Unbind();


    if (framebufferOn)
    {
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

        //glDisable(GL_DEPTH_TEST);        
        RenderCommand::SetFlags(RenderFlags::DepthTest,false);
		// GrayScaleEffect 
		GrayScaleShader->Bind();
        GrayScaleShader->SetUniform("u_Texture", 0);        
        FB->GetColorAtachment()->Bind(0);
        FBVB->SetData(GrayscaleVertData, sizeof(VertexData) * 4);
		FBVAO->Bind();
		RenderCommand::DrawIndexed(QuadVAO, 6);
		FBVAO->Unbind();
        GrayScaleShader->Unbind();

        // blur effect 
		BlurShader->Bind();
        BlurShader->SetUniform("u_Texture", 0);
        float offset = 1 / 300;// float(RenderCommand::GetWindowSize().x);
        BlurShader->SetUniform("u_Offset", offset);
                        
		FB->GetColorAtachment()->Bind(0);
        FBVB->SetData(BlurVertData, sizeof(VertexData)*4);
		FBVAO->Bind();
		RenderCommand::DrawIndexed(QuadVAO, 6);
		FBVAO->Unbind();
        BlurShader->Unbind();

        FB->GetColorAtachment()->Unbind();
        RenderCommand::SetFlags(RenderFlags::DepthTest,true);
        //glEnable(GL_DEPTH_TEST);
    }    
}


void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}
