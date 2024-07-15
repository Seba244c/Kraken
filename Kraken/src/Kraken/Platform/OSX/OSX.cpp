//
// Created by sebsn on 15-07-2024.
//
#include "krpch.h"
#include "Kraken/Platform/PlatformUtils.h"
#include "Kraken/Platform/Time.h"

#include <CoreServices/CoreServices.h>
#include <ctime>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

namespace Kraken {
    uint64_t TimeStart, TimeNow;
    const float nanoToSecond = 0.0000000001f;

    void PlatformInit::Init() {
        TimeStart = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
        KRC_ASSERT(TimeStart, "OSX: Failed to get uptime clock")
    }
    float Platform::Time::GetTime() {
        TimeNow = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
        return (TimeNow - TimeStart) * nanoToSecond;
    }
}
