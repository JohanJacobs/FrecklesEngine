
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

#define LOG_CORE_TRACE(...) ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)  ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)  ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::FE::CORE::LOGGER::s_Logger->GetCoreLogger()->error(__VA_ARGS__)

#define LOG_TRACE(...)      ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)       ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      ::FE::CORE::LOGGER::s_Logger->GetClientLogger()->error(__VA_ARGS__)