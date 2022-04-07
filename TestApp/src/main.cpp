#include <iostream>
#include "FE/FrecklesEngine.hpp"

int main()
{
    std::cout << "Hello FE\n";
    std::cout << "FE welcome message :" << FE::WelcomeMSG() << '\n'; 

    LOG_CORE_TRACE("Hello CORE logger: trace");
    LOG_CORE_INFO("Hello CORE logger: info");
    LOG_CORE_WARN("Hello CORE logger: warn");
    LOG_CORE_ERROR("Hello CORE logger: error");

    LOG_TRACE("Hello client logger: trace");
    LOG_INFO("Hello client logger: info");
    LOG_WARN("Hello client logger: warn");
    LOG_ERROR("Hello client logger: error");

    LOG_CORE_INFO(LOG_FUNCTION_NAME);
    return 0;
}