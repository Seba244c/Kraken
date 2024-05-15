//
// Created by sebsn on 15-05-2024.
//

#pragma once

namespace Kraken {
    class Timestep {
    public:
        explicit Timestep(const float time = 0.0f) : m_Time(time) {}

        [[nodiscard]] float GetSeconds() const { return m_Time; }
        [[nodiscard]] float GetMilliseconds() const { return m_Time * 1000LL; }

        operator float() const { return m_Time; }
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

    namespace Platform {
        class Time {
        public:
            static float GetTime();
            [[nodiscard]] static TimeInstant Now() { return TimeInstant(GetTime()); }
        };
    }

    class Timer {
    public:
        Timer(): m_TimeStart(0), m_TimeEnd(0) {} ;
        explicit Timer(const TimeInstant timeStart) : m_TimeStart(timeStart), m_TimeEnd(0) { }

        static Timer Start() { return Timer(Platform::Time::Now()); }
        void Stop() { m_TimeEnd = Platform::Time::Now(); }

        [[nodiscard]] inline Timestep Get() const {
            if(m_TimeEnd.Get() > 0) return m_TimeEnd - m_TimeStart;
            return Platform::Time::Now() - m_TimeStart;
        };
    private:
        TimeInstant m_TimeStart;
        TimeInstant m_TimeEnd;
    };

    class FPSTimer : public Timer {
    public:
        void Frame() {
            if(Get().GetSeconds() > m_Seconds) {
                m_FPS = m_CountFrames;
                m_CountFrames = 0;

                m_FrameTime = 1.0 / (m_FPS==0 ? 0.01 : m_FPS);
                m_Seconds += 1;
            }

            m_CountFrames++;
        }

        [[nodiscard]] int FPS() const { return m_FPS; }
        [[nodiscard]] float AverageFrameTime() const { return m_FrameTime; }
    private:
        int m_CountFrames = 0;
        int m_Seconds = 0;
        int m_FPS = 0;
        float m_FrameTime = 0;
    };
}
