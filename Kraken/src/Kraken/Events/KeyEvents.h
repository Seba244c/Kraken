//
// Created by sebsn on 27-04-2024.
//

#ifndef KR_KEYEVENTS_H
#define KR_KEYEVENTS_H

#include "Event.h"
#include "Kraken/IO/KeyCodes.h"

namespace Kraken {
    class KeyEvent : public Event {
    public:
        [[nodiscard]] Key::KeyCode KeyCode() const { return m_KeyCode; }
        EVENT_CLASS_CATEGORY(CatagoryKeyboard | CatagoryInput)
    protected:
        explicit KeyEvent(const Key::KeyCode keycode)
            : m_KeyCode(keycode) {}

        Key::KeyCode m_KeyCode;
    };

    class KeyPressedEvent final : public KeyEvent {
    public:
        explicit KeyPressedEvent(const Key::KeyCode keycode, const bool isRepeat = false)
            : KeyEvent(keycode), m_IsRepeat(isRepeat) {}

        [[nodiscard]] bool IsRepeat() const { return m_IsRepeat; }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        bool m_IsRepeat;
    };

    class KeyReleasedEvent final : public KeyEvent {
    public:
        explicit KeyReleasedEvent(const Key::KeyCode keycode)
            : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(const Key::KeyCode keycode) : KeyEvent(KeyCode()) {};
        
        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }
        EVENT_CLASS_TYPE(KeyTyped)
    };
}

#endif //KR_KEYEVENTS_H
