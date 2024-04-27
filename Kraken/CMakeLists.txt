project(Kraken DESCRIPTION "Kraken, C++ Engine" VERSION 1.0.0 LANGUAGES CXX)

# Add library
add_library(Kraken STATIC
        src/krpch.h
        src/Kraken.h
        src/Kraken/Core/Base.h
        src/Kraken/Core/Log.cpp
        src/Kraken/Core/Log.h
        src/Kraken/Core/Entrypoint.h
        src/Kraken/Core/Application.cpp
        src/Kraken/Core/Application.h
        src/Kraken/Core/Assert.h
        src/Kraken/IO/GLFW.cpp
        src/Kraken/IO/GLFW.h
        src/Kraken/Events/Event.h
        src/Kraken/Events/KeyEvents.h
        src/Kraken/IO/KeyCodes.h)
target_include_directories(Kraken PUBLIC src)

# Precompiled headers
target_precompile_headers(Kraken PRIVATE src/krpch.h)

# Preprocessor Definitions
target_compile_definitions(Kraken PUBLIC KR_VERSION="1.0.0")
if(WIN32)
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_WIN)
elseif(APPLE)
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_OSX)
elseif(UNIX) # Automatically NOT APPLE
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_UNIX)
else ()
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_UNKNOWN)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Kraken PUBLIC KR_DEBUG)
else ()
    target_compile_definitions(Kraken PUBLIC KR_RELEASE)
endif ()

# Dependencies
## spdlog
add_subdirectory(vendor/spdlog)
## GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

## Link Dependencies
target_link_libraries(Kraken PUBLIC spdlog::spdlog glfw)