#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "FE/Renderer/Render2D.hpp"
#include <glad/glad.h>

void TestApp::OnUpdate(FE::CORE::Timestep ts)
{
    LOG_TRACE("TestAppLayer::OnUpdate({})", static_cast<float>(ts));
    Angle += 90.0f * ts;
    if (Angle > 360.0f)
        Angle -=360.0f;

    SmileyPos.x += SmileyVelocity*ts;
    if ((SmileyPos.x >= 4.0f) || (SmileyPos.x <= -4.0f))
        SmileyVelocity *= -1.0f;
    
    using namespace FE;
    using namespace RENDERER;
    using namespace CORE;
    
    if (Input::Keypressed(KeyCode::LeftBracket))
    {
        auto size = cameraController.GetSize() - 10.0f * ts;
        cameraController.SetSize(size);
    }
    else if (Input::Keypressed(KeyCode::RightBracket))
    {
        auto size = cameraController.GetSize() + 10.0f * ts;
        cameraController.SetSize(size);
    }

    cameraController.OnUpdate(ts);
        
    // hack to handle window resize 
    auto viewport = RenderCommand::GetWindowSize();
    auto aspectRatio = viewport.x / viewport.y;
    cameraController.SetAspectRatio(aspectRatio);
    RenderCommand::SetViewportSize(0, 0, static_cast<int>(viewport.x), static_cast<int>(viewport.y));
    

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render the initial picture as a texture
	RenderCommand::ClearColor({ 0.15f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();
    // create vertex data 

    glm::vec4 QuadVertexPositions[4];
	QuadVertexPositions[0] = { -1.0f,  1.0f, 0.0f, 1.0f }; // Top Left 
	QuadVertexPositions[1] = { -1.0f, -1.0f, 0.0f, 1.0f }; // Bottom Left 
	QuadVertexPositions[2] = { 1.0f, -1.0f, 0.0f, 1.0f }; // Bottom Right 
	QuadVertexPositions[3] = { 1.0f,  1.0f, 0.0f, 1.0f }; // top Right

    glm::vec2 QuadTextureCoordinates[4];
	QuadTextureCoordinates[0] = { 0.0f, 1.0f }; // Top Left
	QuadTextureCoordinates[1] = { 0.0f, 0.0f }; // Bottom Left
	QuadTextureCoordinates[2] = { 1.0f, 0.0f }; // Bottom Right
	QuadTextureCoordinates[3] = { 1.0f, 1.0f }; // Top Right
    for (int i = 0; i < 4; i++)
    {
        vertData[i].Position = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), { 50.0f,50.0f,1.0f }) * QuadVertexPositions[i];
        vertData[i].TexCoord = QuadTextureCoordinates[i];
        vertData[i].Color = { 1.0f,1.0f,1.0f,1.0f };
        vertData[i].TexIndex= 0;
        vertData[i].TilingFactor = 1;

    }
    VB->SetData(vertData.data(), sizeof(VertexData) * 4);
    Tex1->Bind(0);
    GrayScaleShader->Bind();
    GrayScaleShader->SetUniform("u_ViewProjection", cameraController.GetViewProjection());
    GrayScaleShader->SetUniform("u_Texture", 0);
    RenderCommand::DrawIndexed(VAO, 6);
    //Render2D::RenderTexture( Tex1, { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
    
    
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
    cameraController.SetupCamera(props);

    // framebuffer setup 	
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //create a color attachment 
    Tex1 = Texture2D::Create(static_cast<uint32_t>(viewport.x), static_cast<uint32_t>(viewport.y));
	//glGenTextures(1, &Tex1);
	//glBindTexture(GL_TEXTURE_2D, Tex1);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(viewport.x), static_cast<int>(viewport.y), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex1->GetRenderID(), 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<int>(viewport.x), static_cast<int>(viewport.y)); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
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
            
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GrayScaleShader = Shader::Create("assets/shaders/GrayScale.shader");
    
    uint32_t default_indices[] = { 0, 1, 2, 2, 3, 0 };
    IB = FE::RENDERER::IndexBuffer::Create(default_indices, 6);

	

	VB = VertexBuffer::Create(sizeof(VertexData) * 4);
	BufferLayout layout({
		{"a_Position", BufferElementType::Float3},
		{"a_Color", BufferElementType::Float4},
		{"a_TexCoord", BufferElementType::Float2},
		{"a_TexIndex", BufferElementType::Int},
		{"a_TilingFactor", BufferElementType::Int},
		});
    VAO = VertexArray::Create(VB, IB);


}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
    glDeleteFramebuffers(1, &FBO);
    //glDeleteTextures(1, &Tex1);
    glDeleteRenderbuffers(1, &RBO);   

}