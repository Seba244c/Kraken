//
// Created by sebsn on 28-04-2024.
//

#include "Input.h"
#include "KeyCodes.h"
#include <bitset>

namespace Kraken {
    static std::bitset<KR_KEY_LAST+1> s_KeyDown;
    static std::bitset<KR_MOUSE_LAST+1> s_MouseDown;
    static glm::vec2 s_Mouse;
    
    bool Input::IsKeyPressed(const Key::KeyCode key) {
        return s_KeyDown[key];
    }

    bool Input::IsMouseButtonPressed(const Mouse::MouseCode button) {
        return s_MouseDown[button];
    }

    glm::vec2 Input::GetMousePosition() {
        return s_Mouse;
    }

    float Input::GetMouseX() {
        return s_Mouse.x;
    }

    float Input::GetMouseY() {
        return s_Mouse.y;
    }

    void Input::Event(Kraken::Event &e) {
        if(!e.IsInCategory(CatagoryInput)) return;
        EventDispatcher dispatcher(&e);
        dispatcher.Dispatch<KeyPressedEvent>(KR_BIND_EVENT_STATIC_FN(Input::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(KR_BIND_EVENT_STATIC_FN(Input::OnKeyReleased));
        dispatcher.Dispatch<MouseMovedEvent>(KR_BIND_EVENT_STATIC_FN(Input::OnMouseMoved));
        dispatcher.Dispatch<ButtonPressedEvent>(KR_BIND_EVENT_STATIC_FN(Input::OnMouseButtonPressed));
        dispatcher.Dispatch<ButtonReleasedEvent>(KR_BIND_EVENT_STATIC_FN(Input::OnMouseButtonReleased));
    }

    bool Input::OnMouseButtonPressed(const ButtonPressedEvent &e) {
        s_MouseDown[e.MouseButton()] = true;
        return false;
    }

    bool Input::OnMouseButtonReleased(const ButtonReleasedEvent &e) {
        s_MouseDown[e.MouseButton()] = false;
        return false;
    }

    bool Input::OnMouseMoved(const MouseMovedEvent &e) {
        s_Mouse.x = e.X();
        s_Mouse.y = e.Y();
        return false;
    }

    bool Input::OnKeyPressed(const KeyPressedEvent &e) {
        s_KeyDown[e.KeyCode()] = true;
        return false;
    }

    bool Input::OnKeyReleased(const KeyReleasedEvent &e) {
        s_KeyDown[e.KeyCode()] = false;
        return false;
    }
} // Kraken