
#include "spdlog/spdlog.h"

#include <memory>

namespace FE
{
    namespace CORE
    {
        class Log
        {
            public:
                Log();
                ~Log();
                std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger;}
                std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger;}
            private:
                std::shared_ptr<spdlog::logger> CoreLogger;
                std::shared_ptr<spdlog::logger> ClientLogger;       
        };     

        namespace LOGGER
        {
            static std::unique_ptr<Log> s_Logger = std::make_unique<Log>();
        }
    }
}

#define LOG_CORE_TRACE(x) ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->trace(x)
#define LOG_CORE_INFO(x)  ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->info(x)
#define LOG_CORE_WARN(x)  ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->warn(x)
#define LOG_CORE_ERROR(x) ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->error(x)

#define LOG_TRACE(x) ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->trace(x)
#define LOG_INFO(x)  ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->info(x)
#define LOG_WARN(x)  ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->warn(x)
#define LOG_ERROR(x) ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->error(x)