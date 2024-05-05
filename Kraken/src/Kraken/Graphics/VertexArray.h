//
// Created by sebsn on 05-05-2024.
//

#ifndef KR_VERTEXARRAY_H
#define KR_VERTEXARRAY_H

#include "Kraken/Graphics/Buffer.h"

namespace Kraken {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        [[nodiscard]] virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
    };
} // Kraken

#endif //KR_VERTEXARRAY_H
