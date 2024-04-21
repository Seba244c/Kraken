//
// Created by sebsn on 21-04-2024.
//

#ifndef KR_ENTRYPOINT_H
#define KR_ENTRYPOINT_H
#include "Application.h"

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main(int argc, char** argv) {
    Kraken::Log::Init();
    KRC_INFO("The Kraken awakes!");
    KRC_INFO("Kraken Version: {}", KR_VERSION);

    // Create App
    const auto app = Kraken::CreateApplication({argc, argv});
    KRC_TRACE("Application Info\n\tName: {}, Author: {}, Version: {}",
        app->GetApplicationInfo().Name,
        app->GetApplicationInfo().Author,
        app->GetApplicationInfo().Version);

    // Run application
    app->Run();
    
    // Delete app
    delete app;
    KRC_INFO("Goodbye");
    return 0;
}

#endif //KR_ENTRYPOINT_H
