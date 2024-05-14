//
// Created by sebsn on 14-05-2024.
//

#pragma once
#include "Kraken/Core/Time.h"

namespace Kraken {
    class Time {
    public:
        static float GetTime();
        [[nodiscard]] static TimeInstant Now() { return TimeInstant(GetTime()); }
    };
}
