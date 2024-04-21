//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_BASE_H
#define KR_BASE_H

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

#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

#include "Assert.h"

#include "Log.h"

#endif //KR_BASE_H
