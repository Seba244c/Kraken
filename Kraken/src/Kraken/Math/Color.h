//
// Created by sebsn on 29-04-2024.
//

#ifndef KR_COLOR_H
#define KR_COLOR_H

namespace Kraken {
    struct Color {
        const float r, g, b, a;

        constexpr Color(const float r, const float g, const float b, const float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    };

    class Colors {
    public:
        static constexpr auto Transparant = Color(0, 0, 0, 0);
        static constexpr auto Black = Color(0, 0, 0);
        static constexpr auto White = Color(1, 1, 1);
    
        static constexpr auto Gray = Color(0.5f, 0.5f, 0.5f);
        static constexpr auto DimGray = Color(0.25f, 0.25f, 0.25f);
        static constexpr auto DarkGray = Color(0.1f, 0.1f, 0.1f);
    
        static constexpr auto Red = Color(1, 0, 0);
        static constexpr auto Green = Color(0, 1, 0);
        static constexpr auto Blue = Color(0, 0, 1);

        static constexpr auto Wine = Color(0.5f, 0, 0);
    };
}

#endif //KR_COLOR_H
