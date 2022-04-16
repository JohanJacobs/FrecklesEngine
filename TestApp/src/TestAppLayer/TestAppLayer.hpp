#pragma once 
#include "FE/FrecklesEngine.hpp"
#include "FE/Renderer/Cameras/OrthographicCameraController.h"
#include <glm/glm.hpp>
class TestApp: public FE::CORE::Layer
{
public:
    TestApp(); // default constructor    
    ~TestApp();

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

private:
    
    Ref<FE::RENDERER::VertexArray> VAO;
    Ref<FE::RENDERER::Texture2D> CrateTexture,SmileyTexture;
    // camera stuff 
    FE::RENDERER::OrthographicCameraController cameraController;
    //glm::vec3 cameraPosition{0.0f, 0.0f, 10.0f};
    //glm::vec3 cameraRotation{0.0f, 0.0f, 0.0f};
    //float cameraZoom{1.0f};
    //glm::mat4 cameraTransform{glm::mat4(1.0f)};

    //basic animation stuff 
    float Angle;
    glm::vec3 SmileyPos {0.0f, 3.0f, 0.0f};
    float SmileyVelocity{0.5f};

};