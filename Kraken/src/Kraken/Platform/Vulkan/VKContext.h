//
// Created by Sebastian Snoer on 15/07/2024.
//

#pragma once

#include <krpch.h>
#include "Kraken/Graphics/GraphicsContext.h"

namespace Kraken {
    class VKContext final : public GraphicsContext {
    public:
        explicit VKContext(const ApplicationInfo& appInfo, GLFWwindow* windowHandle);

        void Init() override;
    private:
        GLFWwindow* m_WindowHandle;
        const ApplicationInfo m_AppInfo;
    };
} // Kraken
