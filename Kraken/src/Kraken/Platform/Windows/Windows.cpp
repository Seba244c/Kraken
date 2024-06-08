//
// Created by sebsn on 15-05-2024.
//
#include <krpch.h>
#include "Kraken/Platform/PlatformUtils.h"
#include "Kraken/Platform/Time.h"

namespace Kraken {
    LARGE_INTEGER TimeStart, TimeNow;
    LARGE_INTEGER Frequency;
    float FrequencyRate;

    void PlatformInit::Init() {
        const auto a = QueryPerformanceFrequency(&Frequency);
        KRC_ASSERT(a, "QueryPerformanceFrequency failed");
        FrequencyRate = 1 / static_cast<float>(Frequency.QuadPart);

        QueryPerformanceCounter(&TimeStart);
    }

    float Platform::Time::GetTime() {
        QueryPerformanceCounter(&TimeNow);
        return (TimeNow.QuadPart - TimeStart.QuadPart) * FrequencyRate;
    }
}