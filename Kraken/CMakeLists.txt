project(Kraken DESCRIPTION "Kraken, C++ Engine" LANGUAGES CXX)

# Add library
add_library(Kraken STATIC
        src/krpch.h
        src/Kraken.h
        src/Kraken/Core/Base.h
        src/Kraken/Core/Log.cpp
        src/Kraken/Core/Log.h
        src/Kraken/Core/Entrypoint.h
        src/Kraken/Core/Application.cpp
        src/Kraken/Core/Application.h)
target_include_directories(Kraken PUBLIC src)

# Precompiled headers
target_precompile_headers(Kraken PRIVATE src/krpch.h)

# Preprocessor Definitions
if(WIN32)
    add_compile_definitions(KR_PLATFORM_WIN)
elseif(APPLE)
    add_compile_definitions(KR_PLATFORM_OSX)
elseif(UNIX) # Automatically NOT APPLE
    add_compile_definitions(KR_PLATFORM_UNIX)
endif()

# Dependencies
## spdlog
add_subdirectory(vendor/spdlog)
if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

## Link Dependencies
target_link_libraries(Kraken PUBLIC spdlog::spdlog)