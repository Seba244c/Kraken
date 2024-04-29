//
// Created by sebsn on 28-04-2024.
//

#ifndef KR_INPUT_H
#define KR_INPUT_H
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Kraken/Events/Event.h"
#include "Kraken/Events/MouseEvents.h"
#include "Kraken/Events/KeyEvents.h"

namespace Kraken {
    class Input {
    public:
        static bool IsKeyPressed(Key::KeyCode key);

        static bool IsMouseButtonPressed(Mouse::MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();

        static void Event(Event& e);
    protected:
        static bool OnMouseButtonPressed(const ButtonPressedEvent& e);
        static bool OnMouseButtonReleased(const ButtonReleasedEvent& e);
        static bool OnMouseMoved(const MouseMovedEvent& e);
        static bool OnKeyPressed(const KeyPressedEvent& e);
        static bool OnKeyReleased(const KeyReleasedEvent& e);
    };
} // Kraken

#endif //KR_INPUT_H
