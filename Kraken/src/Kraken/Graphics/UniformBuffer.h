//
// Created by sebsn on 25-05-2024.
//

#pragma once
#include <krpch.h>

namespace Kraken {
    class UniformBuffer {
    public:
        virtual ~UniformBuffer() {}
        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
    };
}