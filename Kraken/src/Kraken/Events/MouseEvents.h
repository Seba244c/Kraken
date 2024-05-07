//
// Created by sebsn on 27-04-2024.
//

#pragma once
#include <krpch.h>

#include "Event.h"
#include "Kraken/IO/MouseCodes.h"

namespace Kraken {
    class MouseMovedEvent final : public Event {
    public:
        MouseMovedEvent(const float x, const float y, const float dx, const float dy) : m_MouseX(x), m_MouseY(y), m_DeltaX(dx), m_DeltaY(dy) {}

        [[nodiscard]] float X() const { return m_MouseX; }
        [[nodiscard]] float Y() const { return m_MouseY; }
        [[nodiscard]] float DeltaX() const { return m_DeltaX; }
        [[nodiscard]] float DeltaY() const { return m_DeltaY; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << " +(" << m_DeltaX << ")" << ", " << m_MouseY << " +(" << m_DeltaY << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved);
        EVENT_CLASS_CATEGORY(CatagoryMouse | CatagoryInput);
    private:
        float m_MouseX, m_MouseY, m_DeltaX, m_DeltaY;
    };

    class MouseScrolledEvent final : public Event {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset) : m_OffsetX(xOffset), m_OffsetY(yOffset) {}

        [[nodiscard]] float XOffset() const { return m_OffsetX; }
        [[nodiscard]] float YOffset() const { return m_OffsetY; }
        
        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << XOffset() << ", " << YOffset();
            return ss.str();
        }
        
        EVENT_CLASS_TYPE(MouseSrolled);
        EVENT_CLASS_CATEGORY(CatagoryMouse | CatagoryInput);
    private:
        const float m_OffsetX, m_OffsetY;
    };

    class MouseButtonEvent : public Event {
    public:
        [[nodiscard]] Mouse::MouseCode MouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(CatagoryMouse | CatagoryInput | CatagoryMouseButton);
    protected:
        explicit MouseButtonEvent(const Mouse::MouseCode button) : m_Button(button) {}
        
        Mouse::MouseCode m_Button;
    };

    class ButtonPressedEvent final : public MouseButtonEvent {
    public:
        explicit ButtonPressedEvent(const Mouse::MouseCode button)
            : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
			ss << "ButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(ButtonPressed)
    };

    class ButtonReleasedEvent final : public MouseButtonEvent {
    public:
        explicit ButtonReleasedEvent(const Mouse::MouseCode button)
            : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "ButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(ButtonReleased)
    };
}
