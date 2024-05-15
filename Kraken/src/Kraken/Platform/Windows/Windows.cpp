//
// Created by sebsn on 15-05-2024.
//
#include "krpch.h"
#include "Kraken/Platform/PlatformUtils.h"
#include "Kraken/Platform/Time.h"

namespace Kraken {
    LARGE_INTEGER TimeStart, TimeNow;
    double Frequency;

    void PlatformInit::Init() {
        LARGE_INTEGER queryFrequency;
        const auto a = QueryPerformanceFrequency(&queryFrequency);
        KRC_ASSERT(a, "QueryPerformanceFrequency failed");
        Frequency = 1.0 / static_cast<double>(queryFrequency.QuadPart);

        QueryPerformanceCounter(&TimeStart);
    }

    float Platform::Time::GetTime() {
        QueryPerformanceCounter(&TimeNow);
        const auto elapesed = (TimeNow.QuadPart - TimeStart.QuadPart) * Frequency;

        return elapesed;
    }
}