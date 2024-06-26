//
// Created by sebsn on 27-04-2024.
//

#pragma once
#include <krpch.h>

#include "Event.h"

namespace Kraken {
    class WindowResizeEvent final : public Event {
    public:
        WindowResizeEvent(const unsigned int width, const unsigned int height)
            : m_Width(width), m_Height(height) {}

        [[nodiscard]] unsigned int GetWidth() const { return m_Width; }
        [[nodiscard]] unsigned int GetHeight() const { return m_Height; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(CatagoryApplication)
    private:
        unsigned int m_Width, m_Height;
    };

    class WindowCloseEvent final : public Event {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(CatagoryApplication)
    };
}