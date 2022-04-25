#include "TestAppLayer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

struct VertexData
{
	glm::vec3 Position;
	glm::vec2 TextureCoords;
};

void TestApp::OnAttach()
{
    LOG_TRACE("TestAppLayer::OnAttach()");
    using namespace FE;
    using namespace RENDERER;
	using namespace CORE;

    //register for events
    EventBus::AddListener<EVENTS::WindowResizeEvent&>("TestApp", BIND_EVENT_FN(OnWindowResizeEvent));
	EventBus::AddListener<EVENTS::MouseScrollEvent&>("TestApp", BIND_EVENT_FN(OnMouseScrollEvent));

    // create the camera 
    Camera.Position = glm::vec3(0.0, 0.0f, 10.0f); // opengl right hand +Z is to me, -z is the background
    Camera.View = glm::translate(glm::mat4(1), Camera.Position);
    Camera.FOV = 45.0f; // degrees 
    auto windowSize = RenderCommand::GetWindowSize();
    Camera.AspectRatio = windowSize.x / windowSize.y;
	Camera.NearClip = 0.1f;
	Camera.FarClip = 100.0f;
    Camera.Projection = glm::perspective(Camera.FOV,Camera.AspectRatio,Camera.NearClip,Camera.FarClip);
	// camera movement stuff 
	Camera.Front = glm::vec3{ 0.0f, 0.0f, -1.0f }; // unit vector defining the forward direction 
	Camera.Up = glm::vec3{ 0.0f, 1.0f, 0.0f }; // unit vector defining the up direction;
	Camera.View = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	Camera.MouseSensitivity = 0.1f;
	Camera.OldMouseValues = Input::GetMousePosition();
	Camera.Pitch = 0.0f; // start with no pitch
	Camera.Yaw = -90.0f; // cause openGL -Z 	

    // cube stuff 	
	Cube.VB = VertexBuffer::Create(36 * 5 * sizeof(float));
	BufferLayout layout{
		{"a_Position", BufferElementType::Float3},
		{"a_TexCoord", BufferElementType::Float2}
	};
	Cube.VB->SetLayout(layout);
//	Cube.VB->SetData(vertices, 36 * 5 * sizeof(float));
	
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
	RenderCommand::SetFlag(ContextRenderFlags::DepthTest, true);

	// this should be in context;
	RenderCommand::SetFlag(ContextWindowFlags::MouseCapture, true);
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
	
	// update camera movement
	if (Input::Keypressed(KeyCode::W))
	{
		Camera.Position += Camera.Front * (Camera.TranslationSpeed * ts);
	}
	else if (Input::Keypressed(KeyCode::S))
	{
		Camera.Position -= Camera.Front * (Camera.TranslationSpeed * ts);
	}
	else if (Input::Keypressed(KeyCode::A))
	{
		Camera.Position -= glm::normalize(glm::cross(Camera.Front, Camera.Up)) * (Camera.TranslationSpeed * ts); // get the Direction (right) vector
	}
	else if (Input::Keypressed(KeyCode::D))
	{
		Camera.Position += glm::normalize(glm::cross(Camera.Front, Camera.Up)) * (Camera.TranslationSpeed * ts);
	}
		
	// camera look around 
	auto mousePos = Input::GetMousePosition();

	//difference from old mouse position
	glm::vec2 PosOffset{ mousePos.x - Camera.OldMouseValues.x,Camera.OldMouseValues.y - mousePos.y };

	glm::vec2 deltaMovement = PosOffset * Camera.MouseSensitivity;
	Camera.OldMouseValues = mousePos;

	Camera.Yaw += deltaMovement.x;
	Camera.Pitch += deltaMovement.y;

	if (Camera.Pitch > 89.0f)
		Camera.Pitch = 89.0f;
	if (Camera.Pitch < -89.0f)
		Camera.Pitch = -89.0f;

	// calculate the direction the camera is facing 
	glm::vec3 direction;
	direction.x = cos(glm::radians(Camera.Yaw)) * cos(glm::radians(Camera.Pitch));
	direction.y = sin(glm::radians(Camera.Pitch));
	direction.z = sin(glm::radians(Camera.Yaw)) * cos(glm::radians(Camera.Pitch));
	Camera.Front = glm::normalize(direction);

	Camera.View = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	
	Camera.Projection = glm::perspective(glm::radians(Camera.FOV), Camera.AspectRatio, Camera.NearClip, Camera.FarClip);

	auto viewProjection = Camera.Projection * Camera.View;
		
	//auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(Cube.RotationDegree), { 1.0f,0.0f,0.0f });
	auto modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), {0.0f,0.0f,1.0f});
	
	Cube.Texture->Bind(0);// bind slot 0
	Cube.Texture2->Bind(1);// bind slot 0
	Cube.TextureShader->Bind();
	Cube.TextureShader->SetUniform("u_Texture", 0); // look at slot 0 for texture
	Cube.TextureShader->SetUniform("u_Texture2", 1); // look at slot 0 for texture
	Cube.TextureShader->SetUniform("u_ViewProjection", viewProjection);
	Cube.TextureShader->SetUniform("u_Model", modelRotation);
	for (int i = 0; i < 5; i++)
	{
		VertexData verts[36];
		for (int j = 0; j < 36; j++)
		{
			uint32_t offset = j * 5;
			
			verts[j].Position.x = vertices[offset + 0] ;
			verts[j].Position.y = vertices[offset + 1];
			verts[j].Position.z = vertices[offset + 2];
			verts[j].Position = glm::rotate(verts[j].Position, glm::radians(Cube.RotationDegree), {1.0f,1.0f,0.0f });
			verts[j].Position += Positions[i];

			verts[j].TextureCoords.x = vertices[offset + 3];
			verts[j].TextureCoords.y = vertices[offset + 4];
		}
		// draw this cube 		
		Cube.VB->SetData(verts, 36 * sizeof(VertexData));
		
		Cube.VAO->Bind();
		RenderCommand::DrawIndexed(Cube.VAO, Cube.VAO->GetIndexCount());
		Cube.VAO->Unbind();
	}
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

void TestApp::OnMouseScrollEvent(FE::EVENTS::MouseScrollEvent& event)
{
	auto offset = event.GetYOffset();
	Camera.FOV -= event.GetYOffset();

	if (Camera.FOV < 1.0f)
		Camera.FOV = 1.0f;
	if (Camera.FOV > 45.0f)
		Camera.FOV = 45.0f;
}
