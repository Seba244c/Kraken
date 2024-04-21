//
// Created by sebsn on 21-04-2024.
//

#include "Kraken.h"

class Sandbox : public Kraken::Application {
public:
    explicit Sandbox(const Kraken::ApplicationInfo &applicationInfo)
        : Application(applicationInfo) {
    }
};

Kraken::Application *Kraken::CreateApplication(AppCommandlineArguments cmdlineArguments) {
    ApplicationInfo info;
    info.Name = "Kraken Sandbox";
    info.Author = "ssnoer";
    info.Version = "1.0.0";
    
    return new Sandbox(info);
}
