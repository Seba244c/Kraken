//
// Created by sebsn on 21-04-2024.
//

#ifdef __cplusplus
#ifndef KR_KRPCH_H
#define KR_KRPCH_H

#ifdef HZ_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        // See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
        #define NOMINMAX
    #endif
#endif

#include <format>
#include <string>
#include <string_view>

#include <set>
#include <vector>
#include <map>
#include <queue>

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <array>

#include <glm/glm.hpp>

#ifdef KR_PLATFORM_WIN
    #include <windows.h>
#endif

#include <memory>

// I WANT THIS NOT TO BE FUCKING RECOMPILED!!
#ifdef KR_SUPPORT_OPENGL
#include <glad/glad.h>
#endif
#ifdef KR_SUPPORT_VULKAN
#include "vulkan/vulkan.h"
#endif
#ifdef KR_SUPPORT_GLFW
#include "GLFW/glfw3.h"
#endif

#include "Kraken/Core/Base.h"
#include "Kraken/Debug/Instrumentor.h"

#endif //KR_KRPCH_H
#endif