//
// Created by sebsn on 28-04-2024.
//

#pragma once
#include <krpch.h>

#include "Kraken/Core/Layer.h"
#include "Kraken/Events/ApplicationEvents.h"

namespace Kraken {
    class ImGuiLayer final : public Layer {
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
