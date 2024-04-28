//
// Created by sebsn on 21-04-2024.
//

#include "Kraken.h"

class SandboxLayer : public Kraken::Layer {
public:
    SandboxLayer() : Layer("Sandbox") {};

    void OnUpdate() override {
        if(Kraken::Input::IsKeyPressed(Kraken::Key::SPACE)) KR_INFO("SPAACE");
    }

    void OnEvent(Kraken::Event &event) override {
        
    }
};

class Sandbox : public Kraken::Application {
public:
    explicit Sandbox(const Kraken::ApplicationInfo &applicationInfo)
        : Application(applicationInfo) {
        PushLayer(new SandboxLayer());
    }
};

Kraken::Application *Kraken::CreateApplication(AppCommandlineArguments cmdlineArguments) {
    ApplicationInfo info;
    info.Name = "Kraken Sandbox";
    info.Author = "ssnoer";
    info.Version = "1.0.0";
    
    return new Sandbox(info);
}
