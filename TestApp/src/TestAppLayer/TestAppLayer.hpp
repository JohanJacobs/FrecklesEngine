#pragma once 
#include "FE/FrecklesEngine.hpp"
#include "FE/Renderer/Cameras/OrthographicCameraController.hpp"
#include <glm/glm.hpp>
#include "FE/Scene/Scene.hpp"

class TestApp: public FE::CORE::Layer
{
public:
    TestApp()=default;
    ~TestApp()=default;

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

    void OnWindowResizeEvent(FE::EVENTS::WindowResizeEvent& event);

    void OnRenderGUI() override;

private:
    void DrawDemoScene();
    void DrawMainScene();

    void SetupRearviewVAO();
private:

    Ref<FE::RENDERER::VertexArray> RearviewVAO;    

    Ref<FE::RENDERER::Texture2D> CrateTexture,SmileyTexture;
    Ref<FE::RENDERER::FrameBuffer> MainFrameBuffer,ViewPortFrameBuffer;
    Ref<FE::RENDERER::Shader> GrayShader;
    // camera stuff 
    FE::RENDERER::OrthographicCameraController cameraController;
 
    //basic animation stuff 
    float Angle{0.0f};
    glm::vec3 SmileyPos {0.0f, 3.0f, 0.0f};
    float SmileyVelocity{0.5f};

    FE::Scene ActiveScene;
    FE::Entity BlueRect;
};