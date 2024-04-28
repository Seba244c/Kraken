//
// Created by sebsn on 28-04-2024.
//

#include "KRImGui.h"

#include "imgui.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Kraken/Core/Application.h"

namespace Kraken {
    static unsigned int s_ImGuiConsumers  = 0;
    
    void KRImGui::InitializePlatform() {
        KRC_TRACE("KRImGui: InitializePlatform");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        // Start the platform
        const Window& window = Application::GetInstance().GetWindow();
        ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void KRImGui::ShutdownPlatform() {
        KRC_TRACE("KRImGui: ShutdownPlatform");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        
        ImGui::DestroyContext();
    }

    void KRImGui::NewFrame() {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void KRImGui::Render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void KRImGui::RegisterConsumer() {
        if(s_ImGuiConsumers++ == 0) InitializePlatform();
    }

    void KRImGui::RemoveConsumer() {
        if(s_ImGuiConsumers++ == 1) ShutdownPlatform();
    }
}
