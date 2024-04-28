//
// Created by sebsn on 27-04-2024.
//

#include "Layerstack.h"

namespace Kraken {
    Layerstack::Layerstack() = default;
    Layerstack::~Layerstack() {
        for (Layer* layer : m_Layers) {
            layer->OnDetach();
            delete layer;
        }
    }

    void Layerstack::PushLayer(Layer *layer) {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void Layerstack::PushOverlay(Layer *overlay) {
        m_Layers.emplace_back(overlay);
    }

    Layer *Layerstack::PopLayer(Layer *layer) {
        if (auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer); it != m_Layers.begin() + m_LayerInsertIndex) {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsertIndex--;
            return layer;
        }

        return nullptr;
    }

    Layer *Layerstack::PopOverlay(Layer *overlay) {
        if (auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay); it != m_Layers.end())
        {
            overlay->OnDetach();
            m_Layers.erase(it);
            return overlay;
        }
        
        return nullptr;
    }
} // Kraken