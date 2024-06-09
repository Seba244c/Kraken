//
// Created by sebsn on 21-04-2024.
//

#pragma once
#include <krpch.h>
#include "Application.h"

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
// ReSharper disable once CppParameterMayBeConst
int main(int argc, char** argv) {
    Kraken::Log::Init();
    KRC_INFO("The Kraken awakes!");
    KRC_INFO("Kraken Version: {}", KR_VERSION);

    KR_PROFILE_BEGIN_SESSION("Startup", "KrakenProfile-Startup.json");
    const auto app = Kraken::CreateApplication({argc, argv});
    KRC_TRACE("Application Info\n\tName: {}, Author: {}, Version: {}",
        app->GetApplicationInfo().Name,
        app->GetApplicationInfo().Author,
        app->GetApplicationInfo().Version);
    KR_PROFILE_END_SESSION();
    
    KR_PROFILE_BEGIN_SESSION("Runtime", "KrakenProfile-Runtime.json");
    app->Run();
    KR_PROFILE_END_SESSION();
    
	KR_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
    delete app;
    KRC_INFO("Goodbye");
	KR_PROFILE_END_SESSION();
    
    return 0;
}
