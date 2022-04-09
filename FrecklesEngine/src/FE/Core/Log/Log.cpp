#include "Log.hpp"

#include <initializer_list>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace FE
{
    namespace CORE
    {
        Log::Log()
        {
            // display log information in the console starting at information level
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(spdlog::level::info);

            // display log information in the console starting at trace level
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/fe.log", true);
            file_sink->set_level(spdlog::level::trace);

            CoreLogger = std::make_shared<spdlog::logger>("FE", std::initializer_list<spdlog::sink_ptr>{console_sink,file_sink});
            ClientLogger = std::make_shared<spdlog::logger>("APP", std::initializer_list<spdlog::sink_ptr>{console_sink,file_sink});

            //enable the logs
            CoreLogger->set_level(spdlog::level::trace);
            CoreLogger->flush_on(spdlog::level::trace);
            ClientLogger->set_level(spdlog::level::trace);
            
           
        }
        Log::~Log()
        {

        }

    }
}