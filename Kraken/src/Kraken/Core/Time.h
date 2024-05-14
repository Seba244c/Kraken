//
// Created by sebsn on 14-05-2024.
//

#pragma once

namespace Kraken {
    class Timestep {
    public:
        explicit Timestep(const float time = 0.0f) : m_Time(time) {}

        [[nodiscard]] float GetSeconds() const { return m_Time; }
        [[nodiscard]] float GetMilliseconds() const { return m_Time * 1000.0f; }

        operator const float() const { return m_Time; }
    private:
        float m_Time;
    };

    class TimeInstant {
    public:
        explicit TimeInstant(float moment);

        Timestep operator-(TimeInstant const& past) const {
            return Timestep(m_Moment - past.m_Moment);
        }

        [[nodiscard]] float Get() const { return m_Moment; }
    private:
        float m_Moment;
    };

    inline TimeInstant::TimeInstant(const float moment): m_Moment(moment) {}
}
