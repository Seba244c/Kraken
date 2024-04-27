//
// Created by sebsn on 27-04-2024.
//

#ifndef KR_EVENT_H
#define KR_EVENT_H

#include "krpch.h"

namespace Kraken {
    enum class EventType {
        None = 0,
        KeyPressed, KeyReleased
    };

    enum EventCatagory {
        None = 0,
        CatagoryInput         = BIT(1),
        CatagoryKeyboard      = BIT(2),
        CatagoryMouse         = BIT(3),
        CatagoryMouseButton   = BIT(4)
    };

// Used by event subclasses
#define EVENT_CLASS_CATEGORY(category) virtual int Category() const override { return category; }
#define EVENT_CLASS_TYPE(type) static Kraken::EventType GetStaticType() { return EventType::type; }\
                                [[nodiscard]] Kraken::EventType EventType() const override { return GetStaticType(); }\
                                [[nodiscard]] const char* Name() const override { return #type; }
    
    class Event {
        friend class EventDispatcher;
    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual EventType EventType() const = 0;
        [[nodiscard]] virtual const char* Name() const = 0;
        [[nodiscard]] virtual int Category() const = 0;
        [[nodiscard]] virtual std::string ToString() const { return Name(); };

        [[nodiscard]] inline bool IsInCategory(EventCatagory category) const { return Category() & category; }
    protected:
        bool m_Handled = false;
    };

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& event)
            : m_Event(event) {}
		
        // F will be deduced by the compiler
        template<typename T, typename F>
        bool Dispatch(const F& func) {
            if (m_Event.EventType() == T::GetStaticType()) {
                m_Event.m_Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.ToString();
    }

}

#endif //KR_EVENT_H
