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
    Ref<FE::RENDERER::Texture2D> CrateTexture,SmileyTexture;

    // camera stuff 
    FE::RENDERER::OrthographicCameraController cameraController;
 
    //basic animation stuff 
    float Angle{0.0f};
    glm::vec3 SmileyPos {0.0f, 3.0f, 0.0f};
    float SmileyVelocity{0.5f};

};