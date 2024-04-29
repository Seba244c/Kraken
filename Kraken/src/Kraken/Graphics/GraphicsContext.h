//
// Created by sebsn on 29-04-2024.
//

#ifndef KR_GRAPHICSCONTEXT_H
#define KR_GRAPHICSCONTEXT_H

namespace Kraken {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        static Scope<GraphicsContext> Create(void* window);
    };
} // Kraken

#endif //KR_GRAPHICSCONTEXT_H
