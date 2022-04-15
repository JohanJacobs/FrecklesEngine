#pragma once 
#include "spdlog/spdlog.h"

#include <spdlog/fmt/ostr.h>
#include <memory>

namespace FE
{
    namespace CORE
    {
		class Log
		{
		public:
			static void Init();

			inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
			inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
		};
    }
}

#define LOG_CORE_TRACE(...) ::FE::CORE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)  ::FE::CORE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)  ::FE::CORE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...) ::FE::CORE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_TRACE(...)      ::FE::CORE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)       ::FE::CORE::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)       ::FE::CORE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)      ::FE::CORE::Log::GetClientLogger()->error(__VA_ARGS__)