#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>

float vertices[] = { // (x,y,z) (tex.x,tex.y) 36 vertices of 5 elements (sizeof(float))
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 1
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 2
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 3
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 4 
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 5
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 6

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;

    //register for events
    EventBus::AddListener<EVENTS::WindowResizeEvent&>("TestApp", BIND_EVENT_FN(OnWindowResizeEvent));

    // create the camera 
    Camera.Position = glm::vec3(0.0, 0.0f, 10.0f); // opengl right hand +Z is to me, -z is the background
    Camera.View = glm::translate(glm::mat4(1), Camera.Position);
    Camera.FOV = 45.0f; // degrees 
    auto windowSize = RenderCommand::GetWindowSize();
    Camera.AspectRatio = windowSize.x / windowSize.y;
	Camera.NearClip = 0.01f;
	Camera.FarClip = 100.0f;
    Camera.Projection = glm::perspective(Camera.FOV,Camera.AspectRatio,Camera.NearClip,Camera.FarClip);

    // cube stuff 
	
	
	Cube.VB = VertexBuffer::Create(36 * 5 * sizeof(float));
	BufferLayout layout{
		{"a_Position", BufferElementType::Float3},
		{"a_TexCoord", BufferElementType::Float2}
	};
	Cube.VB->SetLayout(layout);
	Cube.VB->SetData(vertices, 36 * 5 * sizeof(float));
	
	uint32_t indices[36];
	for (auto i = 0; i < 36; i++)
	{
		indices[i] = i;
	}

	Cube.IB = IndexBuffer:: Create(indices, 36);
	Cube.VAO = VertexArray::Create(Cube.VB, Cube.IB);
	Cube.VAO->Unbind();
	Cube.IB->Unbind();
	Cube.VB->Unbind();

	std::string vertexSrc = R"(
		#version 460 core	
		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec2 a_TexCoord;
		
		// temp
		uniform mat4 u_Model;

		uniform mat4 u_ViewProjection;
		out vec2 v_TexCoords;
		void main()
		{
			v_TexCoords = a_TexCoord;
			gl_Position = u_ViewProjection * u_Model * vec4(a_Position,1.0f) ;
		}
	)";

	std::string fragmentSrc = R"(
		#version 460 core
		out vec4 color;
		in vec2 v_TexCoords;
		uniform sampler2D u_Texture;
		uniform sampler2D u_Texture2;
		void main()
		{
			// linearly interpolate between both textures (80% container, 20% awesomeface)
			vec4 tex1 = vec4(texture(u_Texture,v_TexCoords).rgb,1.0);
			vec4 tex2 = texture(u_Texture2,v_TexCoords);
			color = mix(tex1, tex2,0.2);
			color.a = 1.0;
		}
	)";
	Cube.TextureShader = Shader::Create(vertexSrc, fragmentSrc);
	Cube.TextureShader->Unbind();

	Cube.Texture = Texture2D::Create("assets/textures/container.jpg");
	Cube.Texture->Unbind();
	Cube.Texture2 = Texture2D::Create("assets/textures/AwesomeFace.png");
	Cube.Texture2->Unbind();
		
	Cube.RotationDegree = 0.0f;
	RenderCommand::SetFlag(ContextFlags::DepthTest, true);
}

void TestApp::OnUpdate(FE::CORE::Timestep ts)
{
    LOG_TRACE("TestAppLayer::OnUpdate({})", static_cast<float>(ts));
	Cube.RotationDegree += 45.0f * ts;

    using namespace FE;
    using namespace RENDERER;
    using namespace CORE;
	RenderCommand::ClearColor({0.1f,0.1f,0.15f,1.0f});
	RenderCommand::Clear();
	
	// update camera
	Camera.View = glm::translate(glm::mat4(1.0f), Camera.Position);
	Camera.View = glm::inverse(Camera.View);
	auto viewProjection = Camera.Projection* Camera.View;

	
	auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Cube.RotationDegree), { 1.0f,0.5f,0.2f });
	Cube.Texture->Bind(0);// bind slot 0
	Cube.Texture2->Bind(1);// bind slot 0
	Cube.TextureShader->Bind();
	Cube.TextureShader->SetUniform("u_Texture", 0); // look at slot 0 for texture
	Cube.TextureShader->SetUniform("u_Texture2", 1); // look at slot 0 for texture
	Cube.TextureShader->SetUniform("u_ViewProjection", viewProjection);
	Cube.TextureShader->SetUniform("u_Model", modelRotation);
	Cube.VAO->Bind();
	RenderCommand::DrawIndexed(Cube.VAO, Cube.VAO->GetIndexCount());
	Cube.VAO->Unbind();
	Cube.TextureShader->Unbind();	
}

void TestApp::OnDetach()
{
    LOG_TRACE("TestAppLayer::OnDetach()");
}

void TestApp::OnWindowResizeEvent(FE::EVENTS::WindowResizeEvent& event)
{
    Camera.AspectRatio = event.GetWidth() / static_cast<float>(event.GetHeight());
	Camera.Projection = glm::perspective(Camera.FOV, Camera.AspectRatio, Camera.NearClip, Camera.FarClip);
}