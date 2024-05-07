//
// Created by sebsn on 28-04-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Events/Event.h"
#include "Kraken/Events/KeyEvents.h"
#include "Kraken/Events/MouseEvents.h"
#include "Kraken/Events/ApplicationEvents.h"

namespace Kraken {
    class KRImGui {
    public:
        static void InitializePlatform();
        static void ShutdownPlatform();
        static void NewFrame();
        static void Render();

        static void RegisterConsumer();
        static void RemoveConsumer();
        static void OnEvent(Event &event);
    private:
        static bool OnMouseButtonPressed(const ButtonPressedEvent& e);
        static bool OnMouseButtonReleased(const ButtonReleasedEvent& e);
        static bool OnMouseMoved(const MouseMovedEvent& e);
        static bool OnScrolled(const MouseScrolledEvent& e);
        static bool OnKeyPressed(const KeyPressedEvent& e);
        static bool OnKeyReleased(const KeyReleasedEvent& e);
        static bool OnKeyTyped(const KeyTypedEvent& e);
        static bool OnWindowResized(const WindowResizeEvent& e);
    };
}
