//
// Created by sebsn on 28-04-2024.
//

#ifndef KR_KRIMGUI_H
#define KR_KRIMGUI_H

namespace Kraken {
    class KRImGui {
    public:
        static void InitializePlatform();
        static void ShutdownPlatform();
        static void NewFrame();
        static void Render();

        static void RegisterConsumer();
        static void RemoveConsumer();
    };
}

#endif //KR_KRIMGUI_H
