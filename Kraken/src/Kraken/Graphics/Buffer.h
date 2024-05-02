//
// Created by sebsn on 02-05-2024.
//

#ifndef KR_BUFFER_H
#define KR_BUFFER_H

#include "krpch.h"

namespace Kraken {
    class VertexBuffer {
    public:
		virtual ~VertexBuffer() = default;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<VertexBuffer> Create(const float* vertices, uint32_t size);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        [[nodiscard]] virtual uint32_t GetCount() const = 0;
        
		static Ref<IndexBuffer> Create(const uint32_t *indices, uint32_t count);
    };
}

#endif //KR_BUFFER_H
