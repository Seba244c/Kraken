//
// Created by sebsn on 29-04-2024.
//

#pragma once
#include <krpch.h>

namespace Kraken {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void WaitForFrame() = 0;
        virtual void NewFrame() = 0;
        virtual void ShowFrame() = 0;
        virtual void EnsureIdle() = 0;

        static Scope<GraphicsContext> Create(const ApplicationInfo& appInfo, void* window);
    };
} // Kraken
