//
// Created by sebsn on 27-04-2024.
//

#ifndef KR_MOUSEEVENTS_H
#define KR_MOUSEEVENTS_H

#include "Event.h"

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
}

#endif //KR_MOUSEEVENTS_H
