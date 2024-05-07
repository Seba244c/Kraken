//
// Created by sebsn on 30-04-2024.
//

#pragma once
#include <krpch.h>

namespace Kraken {
    class Files {
    public:
        static std::string ReadFile(const std::string& filepath);
    };
} // Kraken
