//
// Created by sebsn on 28-04-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Events/Event.h"

namespace Kraken {
    class GraphicsContext;

    struct WindowSpecs {
        const ApplicationInfo appInfo = {};
        bool noResize = false;
        bool initializeFullscreen = false;
        bool initializeHidden = false;
        uint32_t Width = 800;
        uint32_t Height = 600;
        std::string title;
    };
    
    class Window {
    public:
        using EventCallbackFn = std::function<void(Event*)>;
        
        virtual ~Window() = default;

        // Actions
        virtual void PollEvents() = 0;
        virtual void Show() = 0;
        virtual void Fullscreen(bool fullscreen) = 0;

        // Information
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        [[nodiscard]] virtual uint32_t GetFBWidth() const = 0;
        [[nodiscard]] virtual uint32_t GetFBHeight() const = 0;
        [[nodiscard]] virtual bool IsFullscreen() const = 0;
        [[nodiscard]] virtual void* GetNativeWindow() const = 0;
        [[nodiscard]] virtual const Scope<GraphicsContext>& GetGraphicsContext() = 0;

        static Scope<Window> Create(const WindowSpecs& specs = WindowSpecs());
    };
}
