//
// Created by sebsn on 28-04-2024.
//

#include "KRImGui.h"

#include "Kraken/Core/Application.h"

#include "imgui.h"
#ifdef KR_SUPPORT_OPENGL
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef KR_SUPPORT_GLFW
#include "backends/imgui_impl_glfw.h"
#include "GLFW/GLFW.h"
#endif

namespace Kraken {
    static unsigned int s_ImGuiConsumers  = 0;
    
    void KRImGui::InitializePlatform() {
        KRC_TRACE("KRImGui: InitializePlatform");
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        // Start the platform
#ifdef KR_SUPPORT_GLFW
        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()), false);
#endif

#ifdef KR_SUPPORT_OPENGL
        ImGui_ImplOpenGL3_Init("#version 410");
#endif
    }

    void KRImGui::ShutdownPlatform() {
        KRC_TRACE("KRImGui: ShutdownPlatform");
#ifdef KR_SUPPORT_GLFW
        ImGui_ImplOpenGL3_Shutdown();
#endif
#ifdef KR_SUPPORT_GLFW
        ImGui_ImplGlfw_Shutdown();
#endif
        
        ImGui::DestroyContext();
    }

    void KRImGui::NewFrame() {
#ifdef KR_SUPPORT_GLFW
        ImGui_ImplGlfw_NewFrame();
#endif
#ifdef KR_SUPPORT_GLFW
        ImGui_ImplOpenGL3_NewFrame();
#endif
        ImGui::NewFrame();
    }

    void KRImGui::Render() {
        ImGui::Render();
#ifdef KR_SUPPORT_GLFW
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
    }

    void KRImGui::RegisterConsumer() {
        if(s_ImGuiConsumers++ == 0) InitializePlatform();
    }

    void KRImGui::RemoveConsumer() {
        if(s_ImGuiConsumers++ == 1) ShutdownPlatform();
    }

    void KRImGui::OnEvent(Event &event) { // This onnly needs to be called once pr event, but should be able to handle if it is called multiple times 
        EventDispatcher dispatcher(&event);
        dispatcher.Dispatch<WindowResizeEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnWindowResized));
        dispatcher.Dispatch<ButtonPressedEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnMouseButtonPressed));
        dispatcher.Dispatch<ButtonReleasedEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnMouseButtonReleased));
        dispatcher.Dispatch<MouseScrolledEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnScrolled));
        dispatcher.Dispatch<MouseMovedEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnMouseMoved));
        dispatcher.Dispatch<KeyPressedEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(KR_BIND_EVENT_STATIC_FN(KRImGui::OnKeyReleased));
    }
    
    bool KRImGui::OnMouseButtonPressed(const ButtonPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.MouseButton()] = true;
        
        return io.WantCaptureMouse;
    }

    bool KRImGui::OnMouseButtonReleased(const ButtonReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.MouseButton()] = false;
        
        return io.WantCaptureMouse;
    }

    bool KRImGui::OnMouseMoved(const MouseMovedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.X(), e.Y());
        
        return io.WantCaptureMouse;
    }

    bool KRImGui::OnScrolled(const MouseScrolledEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.YOffset();
        io.MouseWheelH += e.XOffset();
        
        return io.WantCaptureMouse;
    }

    bool KRImGui::OnKeyPressed(const KeyPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.KeyCode()] = true;
        return io.WantCaptureKeyboard;
    }

    bool KRImGui::OnKeyReleased(const KeyReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.KeyCode()] = false;
        return io.WantCaptureKeyboard;
    }

    bool KRImGui::OnKeyTyped(const KeyTypedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();

        if (const int keycode = e.KeyCode(); keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter(static_cast<unsigned short>(keycode));
        
        return io.WantCaptureKeyboard;
    }

    bool KRImGui::OnWindowResized(const WindowResizeEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
        return false;
    }
}
