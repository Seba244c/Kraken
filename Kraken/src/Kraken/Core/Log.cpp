//
// Created by sebsn on 21-04-2024.
//

#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Kraken {
    std::shared_ptr<spdlog::logger> Log::s_Logger;
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;

    void Log::Init() {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Cube.log", true));
    
        logSinks[0]->set_pattern("%^[%T] [thread %t] (%n)%$ %v");
        logSinks[1]->set_pattern("%l: [%T] [thread %t] (%n) %v");

        s_Logger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        s_Logger->set_level(spdlog::level::trace);
        s_Logger->flush_on(spdlog::level::trace);
        
        s_CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);
    }
}