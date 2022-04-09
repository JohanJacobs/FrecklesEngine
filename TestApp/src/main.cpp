
#include "FE/FrecklesEngine.hpp"

int main()
{
    auto App = FE::CreateApplication();
    
    App->Run();

    App->Shutdown();
    return 0;
}