//
// Created by sebsn on 27-04-2024.
//

#pragma once
#include <krpch.h>

#include "Layer.h"

namespace Kraken {
    class Layerstack {
    public:
        Layerstack();
        ~Layerstack();
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        Layer *PopLayer(Layer *layer);

        Layer *PopOverlay(Layer *overlay);

        [[nodiscard]] bool IsEmpty() const { return m_Layers.empty(); };
        
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

        [[nodiscard]] std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
        [[nodiscard]] std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        [[nodiscard]] std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
    private:
        std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
    };
} // Kraken
