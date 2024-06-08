//
// Created by sebsn on 30-04-2024.
//

#pragma once
#include <krpch.h>

namespace Kraken {
    class  Files {
    public:
        [[nodiscard]] static std::string ReadFile(const std::string& filepath);
        [[nodiscard]] static std::string ReadFile(const std::filesystem::path& filepath);
        [[nodiscard]] static long long LoadFile(char **buffer,const std::filesystem::path& filepath);
        [[nodiscard]] static std::string GetCacheDir();
    };
} // Kraken
