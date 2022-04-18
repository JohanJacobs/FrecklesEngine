#pragma once 
#include "FE/FrecklesEngine.hpp"
#include "FE/Renderer/Cameras/OrthographicCameraController.h"
#include <glm/glm.hpp>


class TestApp : public FE::CORE::Layer
{
public:
	TestApp() = default;
	~TestApp() = default;

	void OnUpdate(FE::CORE::Timestep ts) override;
	void OnAttach() override;
	void OnDetach() override;

private:

	//frame buffer 
	uint32_t FBO;
	uint32_t /*ColorAttachment,*/DepthAndStencilBuffer;
	//shaders 
	Ref<FE::RENDERER::Shader> defaultShader,screenShader;
	// buffers 
	Ref<FE::RENDERER::VertexArray> QuadVAO,FBVAO;
	Ref<FE::RENDERER::VertexBuffer> QuadVB, FBVB;
	Ref<FE::RENDERER::IndexBuffer> QuadIB, FBIB;

	// Textures 
	Ref<FE::RENDERER::Texture2D> SmileyTexture, CrateTexture, ColorAttachment;

	// camera stuff 
	FE::RENDERER::OrthographicCameraController cameraController;

	struct VertexData
	{
		glm::vec3 Position; // 3 * float = 12 bytes
		glm::vec4 Color;	// 4 * float = 16 bytes
		glm::vec2 TexCoord; // 2 * float = 8 bytes
		int TexIndex;  		// 1 * uint32 = 4 bytes
		int TilingFactor; 	// 1 * uint32 = 4 bytes
	};

	std::array<VertexData, 4> vertData;

	glm::vec4 defaultQuadVertices[4]
	{
		{ -1.0f,  1.0f, 0.0f, 1.0f }, // top left
		{ -1.0f, -1.0f, 0.0f, 1.0f }, // bottom left
		{  1.0f, -1.0f, 0.0f, 1.0f }, // bottom right
		{  1.0f,  1.0f, 0.0f, 1.0f }  // top right
	};

	glm::vec2 defaultQuadTextCoords[4] =
	{
		{ 0.0f, 1.0f }, // Top Left
		{ 0.0f, 0.0f }, // Bottom Left
		{ 1.0f, 0.0f }, // Bottom Right
		{ 1.0f, 1.0f } // Top Right
	};

    uint32_t defaultQuadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		
    bool framebufferOn = false;
};