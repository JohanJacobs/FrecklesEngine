#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "FE/Renderer/Render2D.hpp"
#include <glad/glad.h>


void CheckFramebufferStatus(uint32_t fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	switch (status)
    {
    case GL_FRAMEBUFFER_UNDEFINED:LOG_ERROR("GL_FRAMEBUFFER_UNDEFINED"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
    case GL_FRAMEBUFFER_UNSUPPORTED:LOG_ERROR("GL_FRAMEBUFFER_UNSUPPORTED"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); break;
    case GL_FRAMEBUFFER_COMPLETE: LOG_INFO("framebuffer complete");
    };
}
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
    
    // QUAD FOR FRAMEBUFFER
    FBVB = VertexBuffer::Create(quad_vb_size);
    FBVB->SetLayout(layout);
    VertexData FBVertexData[4];
	for (int i = 0; i < 4; i++)
	{	
        FBVertexData[i].Position = defaultQuadVertices[i]; // scale to half size
        FBVertexData[i].Color = defaultColor;
        FBVertexData[i].TexCoord = defaultQuadTextCoords[i];
        FBVertexData[i].TexIndex = 0;
        FBVertexData[i].TilingFactor = 1;
	}
    FBVertexData[0].Position = { -1.0f, 1.0f, 0.0f }; // left top 
    FBVertexData[1].Position = { -1.0f, 0.35f, 0.0f }; // left bottom
    FBVertexData[2].Position = { -0.35f, 0.35f, 0.0f }; // right bottom
    FBVertexData[3].Position = { -0.35f, 1.0f, 0.0f};  // right top 

    FBVB->SetData(FBVertexData, quad_vb_size);
	FBIB = IndexBuffer::Create(defaultQuadIndices, 6);
    FBVAO = VertexArray::Create(FBVB, FBIB);
    FBVAO->Unbind();

    //create frame buffer 
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //create color attachment (texture) 
    ColorAttachment = Texture2D::Create(GLsizei(RenderCommand::GetWindowSize().x), GLsizei(RenderCommand::GetWindowSize().y));
    ColorAttachment->Unbind();
    //glGenTextures(1, &ColorAttachment);
    //glBindTexture(GL_TEXTURE_2D, ColorAttachment);//bind 
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLsizei(RenderCommand::GetWindowSize().x), GLsizei(RenderCommand::GetWindowSize().y), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr); // text storage
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glBindTexture(GL_TEXTURE_2D, 0); // unbind 

    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment->GetRenderID(), 0);

    // depth and stencil buffer as render buffer 
    glGenRenderbuffers(1, &DepthAndStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, DepthAndStencilBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GLsizei(RenderCommand::GetWindowSize().x), GLsizei(RenderCommand::GetWindowSize().y));
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthAndStencilBuffer);
   
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    CheckFramebufferStatus(FBO);

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

    if (Input::Keypressed(KeyCode::RightBracket))
    {
        auto size = cameraController.GetSize();
        size += 5.0f*ts;
        cameraController.SetSize(size);
        LOG_INFO("Camera size {}", size);

        /*auto pos = cameraController.GetPosition();
        pos.z -= 5.0f * ts;
        cameraController.SetPosition(pos);
        LOG_INFO("CameraPos ({},{},{})",pos.x, pos.y, pos.z);*/
    }

	if (Input::Keypressed(KeyCode::LeftBracket))
	{
		auto size = cameraController.GetSize();
		size -= 5.0f * ts;
		cameraController.SetSize(size);
        LOG_INFO("Camera size {}", size);

		/*auto pos = cameraController.GetPosition();
		pos.z += 5.0f * ts;
		cameraController.SetPosition(pos);
        LOG_INFO("CameraPos ({},{},{})", pos.x, pos.y, pos.z);*/
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
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
	RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
	RenderCommand::Clear();

    Render2D::BeginScene(cameraController.GetViewProjection());
   
    Render2D::RenderTexture(CrateTexture, { -1.0f,0.0f,0.0f }, { 1.0f,1.0f }, {1.0f,1.0f,1.0f,1.0f });
    Render2D::RenderTexture(SmileyTexture, { 1.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
    Render2D::EndScene();

    //RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
    //RenderCommand::Clear();
    
    //SmileyTexture->Bind(0);
    //defaultShader->Bind();    
    //QuadVAO->Bind();
    //RenderCommand::DrawIndexed(QuadVAO, 6);
    //QuadVAO->Unbind();
    //defaultShader->Unbind();
    //SmileyTexture->Unbind();

    if (framebufferOn)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (framebufferOn)
    {
		RenderCommand::ClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
		RenderCommand::Clear();

		Render2D::BeginScene(cameraController.GetViewProjection());

		Render2D::RenderTexture(CrateTexture, { -1.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		Render2D::RenderTexture(SmileyTexture, { 1.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
		Render2D::EndScene();

		//RenderCommand::ClearColor({ 0.15f, 0.1f, 0.1f, 1.0f });
		//RenderCommand::Clear();
        glDisable(GL_DEPTH_TEST);        
		// render the captured screen back 
		screenShader->Bind();
		screenShader->SetUniform("u_Texture", 0);        
        ColorAttachment->Bind(0);
		FBVAO->Bind();
		RenderCommand::DrawIndexed(QuadVAO, 6);
		FBVAO->Unbind();
		screenShader->Unbind();
        ColorAttachment->Unbind();
        glEnable(GL_DEPTH_TEST);
    }    
}


void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}

//switch (status)
//{
//case GL_FRAMEBUFFER_UNDEFINED:LOG_ERROR("GL_FRAMEBUFFER_UNDEFINED"); break;
//case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"); break;
//case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"); break;
//case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"); break;
//case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"); break;
//case GL_FRAMEBUFFER_UNSUPPORTED:LOG_ERROR("GL_FRAMEBUFFER_UNSUPPORTED"); break;
//case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"); break;
//case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"); break;
//case GL_FRAMEBUFFER_COMPLETE: LOG_INFO("framebuffer complete");
//};