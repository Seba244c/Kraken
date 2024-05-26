//
// Created by sebsn on 21-04-2024.
//

#pragma once

#include <filesystem>
#include <spdlog/spdlog.h>

namespace Kraken {
    class Log {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetLogger() { return s_Logger; }
        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
}


#define KR_TRACE(...) ::Kraken::Log::GetLogger()->trace(__VA_ARGS__)
#define KR_INFO(...) ::Kraken::Log::GetLogger()->info(__VA_ARGS__)
#define KR_WARN(...) ::Kraken::Log::GetLogger()->warn(__VA_ARGS__)
#define KR_ERROR(...) ::Kraken::Log::GetLogger()->error(__VA_ARGS__)
#define KR_CRITICAL(...) ::Kraken::Log::GetLogger()->critical(__VA_ARGS__)


#define KRC_TRACE(...) ::Kraken::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KRC_INFO(...) ::Kraken::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KRC_WARN(...) ::Kraken::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KRC_ERROR(...) ::Kraken::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KRC_CRITICAL(...) ::Kraken::Log::GetCoreLogger()->critical(__VA_ARGS__)

template <> struct fmt::formatter<std::filesystem::path> : formatter<std::string> {
    auto format(std::filesystem::path p, format_context& ctx) {
        return formatter<std::string>::format(p.string(), ctx);
    }
};