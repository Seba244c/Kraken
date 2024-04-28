//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_BASE_H
#define KR_BASE_H

#include <memory>

#ifdef KR_DEBUG
    #if defined(KR_PLATFORM_WIN)
        #define KR_DEBUGBREAK() __debugbreak()
    #elif defined(KR_PLATFORM_UNIX)
        #include <csignal>
        #define KR_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif

    #define KR_ENABLE_ASSERT
#else
    #define KR_DEBUGBREAK()
#endif

#define BIT(x) (1 << x)
#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x
#define KR_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Kraken {
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

#include "Assert.h"
#include "Log.h"

#endif //KR_BASE_H
