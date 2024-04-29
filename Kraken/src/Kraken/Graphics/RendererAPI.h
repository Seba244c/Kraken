//
// Created by sebsn on 29-04-2024.
//

#ifndef KR_RENDERAPI_H
#define KR_RENDERAPI_H
#include "krpch.h"

namespace Kraken {
    class RendererAPI {
    public:
        enum class API {
            None = 0, OpenGL = 1
        };
        static API GetAPI() { return API::OpenGL; }
    //     static Scope<RendererAPI> Create();
    // private:
    //     static API s_API;
    };
}

#endif //KR_RENDERAPI_H
