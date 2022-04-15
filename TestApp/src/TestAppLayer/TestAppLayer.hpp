#pragma once 
#include "FE/FrecklesEngine.hpp"
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
    
    // camera stuff 
    glm::vec3 cameraPosition{0.0f, 0.0f, 10.0f};
    glm::vec3 cameraRotation{0.0f, 0.0f, 0.0f};
    float cameraZoom{1.0f};
    glm::mat4 cameraTransform{glm::mat4(1.0f)};

};