#include "FE/FrecklesEngine.hpp"
#include "TestAppLayer/TestAppLayer.hpp"

int main(int argc, char** argv)
{
    
    auto App = CreateApplication();
    
    App->PushLayer(new TestApp());
    App->Run();
    
    return 0;
}