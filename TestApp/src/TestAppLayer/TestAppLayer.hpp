#pragma once 
#include "FE/FrecklesEngine.hpp"
#include "FE/Renderer/Cameras/OrthographicCameraController.h"
#include <glm/glm.hpp>
class TestApp: public FE::CORE::Layer
{
public:
    TestApp()=default;
    ~TestApp()=default;

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

private:
    
    Ref<FE::RENDERER::VertexArray> VAO;
    Ref<FE::RENDERER::Texture2D> CrateTexture,SmileyTexture, Tex1;

    Ref<FE::RENDERER::Shader> GrayScaleShader;
    // camera stuff 
    FE::RENDERER::OrthographicCameraController cameraController;
 
    //basic animation stuff 
    float Angle{0.0f};
    glm::vec3 SmileyPos {0.0f, 3.0f, 0.0f};
    float SmileyVelocity{5.0f};

    //framebuffer
    uint32_t FBO, RBO;
    //uint32_t Tex1;
        
    Ref<FE::RENDERER::VertexBuffer> VB;
    Ref<FE::RENDERER::IndexBuffer> IB; 

	struct VertexData
	{
		glm::vec3 Position; // 3 * float = 12 bytes
		glm::vec4 Color;	// 4 * float = 16 bytes
		glm::vec2 TexCoord; // 2 * float = 8 bytes
		int TexIndex;  		// 1 * uint32 = 4 bytes
		int TilingFactor; 	// 1 * uint32 = 4 bytes
	};
    std::array<VertexData, 4> vertData;
};