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
        src/Kraken/Graphics/RendererAPI.cpp
        src/Kraken/Platform/OpenGL/OpenGLRendererAPI.cpp
        src/Kraken/Platform/OpenGL/OpenGLRendererAPI.h
        src/Kraken/Graphics/Buffer.h
        src/Kraken/Platform/OpenGL/OpenGLBuffer.h
        src/Kraken/Platform/OpenGL/OpenGLBuffer.cpp
        src/Kraken/Graphics/VertexArray.h
        src/Kraken/Platform/OpenGL/OpenGLVertexArray.cpp
        src/Kraken/Platform/OpenGL/OpenGLVertexArray.h
        src/Kraken/Graphics/RenderCommand.h
        src/Kraken/Graphics/Renderer.cpp
        src/Kraken/Graphics/RenderCommand.cpp
        src/krpch.cpp
        src/Kraken/Graphics/Framebuffer.h
        src/Kraken/Platform/OpenGL/OpenGLFramebuffer.cpp
        src/Kraken/Platform/OpenGL/OpenGLFramebuffer.h
        src/Kraken/Graphics/Camera.h
)
target_include_directories(Kraken PUBLIC src)
target_precompile_headers(Kraken PRIVATE src/krpch.h) # Precompiled headers
target_compile_definitions(Kraken PUBLIC KR_VERSION="1.0.0")

# Preprocessor build type definition
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Kraken PUBLIC KR_DEBUG)
else ()
    target_compile_definitions(Kraken PUBLIC KR_RELEASE)
endif ()

# Universal Dependencies
set(KR_LIBS spdlog::spdlog imgui glm::glm)

add_subdirectory(vendor/spdlog) ## spdlog
add_subdirectory(vendor/imgui) # dear ImGui
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE) # GLM
add_subdirectory(vendor/glm)

### Platform Support
if(WIN32)
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_WIN)

    set(support_glfw ON BOOL)
    set(support_opengl ON BOOL)
elseif(APPLE)
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_OSX)

    set(support_glfw OFF BOOL)
    set(support_opengl OFF BOOL)
elseif(UNIX) # Automatically NOT APPLE
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_UNIX)

    set(support_glfw OFF BOOL)
    set(support_opengl OFF BOOL)
else ()
    target_compile_definitions(Kraken PUBLIC KR_PLATFORM_UNKNOWN)

    set(support_glfw OFF BOOL)
    set(support_opengl OFF BOOL)
endif()

# Add platform specific libraries
if(support_opengl)
    add_subdirectory(vendor/GLAD)

    target_compile_definitions(Kraken PUBLIC KR_SUPPORT_OPENGL)
    set(KR_LIBS ${KR_LIBS} glad)
endif()
if(support_glfw)
    target_compile_definitions(Kraken PUBLIC GLFW_INCLUDE_NONE) # Force GLFW not to include GL, for everybodys sake
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(vendor/glfw)

    target_compile_definitions(Kraken PUBLIC KR_SUPPORT_GLFW)
    set(KR_LIBS ${KR_LIBS} glfw)
endif()

## Link Dependencies
target_link_libraries(Kraken PUBLIC ${KR_LIBS})