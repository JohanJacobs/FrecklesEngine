#pragma once 
#include "FE/FrecklesEngine.hpp"
#include <glm/glm.hpp>

class TestApp: public FE::CORE::Layer
{
public:
    TestApp()=default;
    ~TestApp()=default;

    void OnUpdate(FE::CORE::Timestep ts) override;
    void OnAttach() override;
    void OnDetach() override;

    void OnWindowResizeEvent(FE::EVENTS::WindowResizeEvent& event);
    void OnMouseScrollEvent(FE::EVENTS::MouseScrollEvent& event);
private:

private:
    struct CameraData
    {
        float TranslationSpeed{ 10.0f };
        float FOV, NearClip, FarClip, AspectRatio;
        glm::mat4 Projection;
        glm::mat4 View; // aka, camera transform


        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;

        glm::vec2 OldMouseValues;
        float MouseSensitivity;
        float Yaw, Pitch;        

    } Camera;
    glm::vec3 Positions[5] =
    {
        { 0.0f, 0.0f, 0.0f},
        { 5.0f, 0.0f, 0.0f},
        {-5.0f, 0.0f, 0.0f},
        { 0.0f, 5.0f, 0.0f},
        { 0.0f,-5.0f, 0.0f}
    };
    struct CubeData
    {
        float RotationDegree ;
        Ref<FE::RENDERER::VertexArray> VAO;
        Ref<FE::RENDERER::IndexBuffer> IB;
        Ref<FE::RENDERER::VertexBuffer> VB;

        Ref<FE::RENDERER::Texture2D> Texture,Texture2;
        Ref<FE::RENDERER::Shader> TextureShader;
    }Cube;
};