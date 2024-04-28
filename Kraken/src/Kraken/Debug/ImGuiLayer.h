//
// Created by sebsn on 28-04-2024.
//

#ifndef KR_IMGUILAYER_H
#define KR_IMGUILAYER_H

#include "Kraken/Core/Layer.h"
#include "Kraken/Events/ApplicationEvents.h"
#include "Kraken/Events/MouseEvents.h"
#include "Kraken/Events/KeyEvents.h"

namespace Kraken {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event &event) override;
        
		void SetDarkThemeColors();
    };
}

#endif //KR_IMGUILAYER_H
