#include "TestAppLayer/TestAppLayer.hpp"

int main()
{
    auto App = FE::CreateApplication();
    
    App->PushLayer(new TestApp());
    App->Run();
    
    return 0;
}