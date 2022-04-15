#include "Log.hpp"

#include <initializer_list>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> FE::CORE::Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> FE::CORE::Log::s_ClientLogger;

namespace FE
{
    namespace CORE
	{        
        void Log::Init()
        {
            // display log information in the console starting at information level
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::info);

            // display log information in the console starting at trace level
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/FE.log", true);
            file_sink->set_level(spdlog::level::trace);

            s_CoreLogger = std::make_shared<spdlog::logger>("FE", std::initializer_list<spdlog::sink_ptr>{console_sink,file_sink});
            s_ClientLogger = std::make_shared<spdlog::logger>("APP", std::initializer_list<spdlog::sink_ptr>{console_sink,file_sink});

            //enable the logs 
            s_CoreLogger->set_level(spdlog::level::trace);
            s_CoreLogger->flush_on(spdlog::level::trace);
            
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->flush_on(spdlog::level::trace);			
           
        }               
    }
}

