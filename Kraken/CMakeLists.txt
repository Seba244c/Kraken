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
        src/Kraken/Platform/GLFW/GLFW.cpp
        src/Kraken/Platform/GLFW/GLFW.h
        src/Kraken/Events/Event.h
        src/Kraken/Events/KeyEvents.h
        src/Kraken/IO/KeyCodes.h
        src/Kraken/Events/ApplicationEvents.h
        src/Kraken/Core/Layerstack.cpp
        src/Kraken/Core/Layerstack.h
        src/Kraken/Core/Layer.cpp
        src/Kraken/Core/Layer.h
        src/Kraken/Events/MouseEvents.h
        src/Kraken/Debug/ImGuiLayer.cpp
        src/Kraken/Debug/ImGuiLayer.h
        src/Kraken/Platform/KRImGui.h
        src/Kraken/Platform/KRImGui.cpp
        src/Kraken/IO/MouseCodes.h
        src/Kraken/IO/Input.cpp
        src/Kraken/IO/Input.h
        src/Kraken/IO/Window.cpp
        src/Kraken/IO/Window.h
        src/Kraken/Graphics/GraphicsContext.cpp
        src/Kraken/Graphics/GraphicsContext.h
        src/Kraken/Graphics/Renderer.h
        src/Kraken/Graphics/RendererAPI.h
        src/Kraken/Platform/OpenGL/OpenGLContext.cpp
        src/Kraken/Platform/OpenGL/OpenGLContext.h
        src/Kraken/Math/Color.h
        src/Kraken/Graphics/Shader.cpp
        src/Kraken/Graphics/Shader.h
        src/Kraken/Platform/OpenGL/OpenGLShader.cpp
        src/Kraken/Platform/OpenGL/OpenGLShader.h
        src/Kraken/Utils/Files.cpp
        src/Kraken/Utils/Files.h
)
target_include_directories(Kraken PUBLIC src)

# Precompiled headers
target_precompile_headers(Kraken PRIVATE src/krpch.h)

# Preprocessor Definitions
target_compile_definitions(Kraken PUBLIC KR_VERSION="1.0.0")
target_compile_definitions(Kraken PUBLIC GLFW_INCLUDE_NONE) # Force GLFW not to include GL, for everybodys sake
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
# OpenGL
add_subdirectory(vendor/GLAD)
# dear ImGui
add_subdirectory(vendor/imgui)
# GLM
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glm)

## Link Dependencies
target_link_libraries(Kraken PUBLIC spdlog::spdlog glm::glm glfw glad imgui)