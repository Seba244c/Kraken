//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_ASSERT_H
#define KR_ASSERT_H

#ifdef KR_ENABLE_ASSERT
    #define KR_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { KR##type##ERROR(msg, __VA_ARGS__); KR_DEBUGBREAK(); } }
    #define KR_INTERNAL_ASSERT_WITH_MSG(type, check, ...) KR_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define KR_INTERNAL_ASSERT_NO_MSG(type, check) KR_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define KR_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define KR_INTERNAL_ASSERT_GET_MACRO(...) EXPAND_MACRO( KR_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KR_INTERNAL_ASSERT_WITH_MSG, KR_INTERNAL_ASSERT_NO_MSG) )

    #define KR_ASSERT(...) EXPAND_MACRO(KR_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
    #define KRC_ASSERT(...) EXPAND_MACRO(KR_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(C_, __VA_ARGS__))
#else
    #define KR_ASSERT(...)
    #define KRC_ASSERT(...)
#endif

#endif //KR_ASSERT_H
