//
// Created by sebsn on 27-04-2024.
//

#ifndef KR_LAYER_H
#define KR_LAYER_H
#include "Kraken/Events/Event.h"

namespace Kraken {
    class Layer {
    public:
		explicit Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& event) {}

        [[nodiscard]] const std::string& Name() const { return m_Name; }
    protected:
        std::string m_Name;
    };
} // Kraken

#endif //KR_LAYER_H
