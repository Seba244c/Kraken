//
// Created by sebsn on 21-04-2024.
//

#include "Application.h"

namespace Kraken {
    Application::Application(const ApplicationInfo &applicationInfo) : m_ApplicationInfo(applicationInfo) {
        KRC_ASSERT(!s_Instance, "Instance shouldn't already exist");
        s_Instance = this;
    }

    void Application::Run() {
        
    }
} // Kraken