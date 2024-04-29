//
// Created by sebsn on 29-04-2024.
//

#ifndef KR_RENDERER_H
#define KR_RENDERER_H

#include "RendererAPI.h"

namespace Kraken {
    class Renderer {
    public:
        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };    
}

#endif //KR_RENDERER_H
