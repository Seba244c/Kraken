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
        src/Kraken/IO/MouseCodes.h
        src/Kraken/IO/Input.cpp
        src/Kraken/IO/Input.h
        src/Kraken/IO/Window.cpp
        src/Kraken/IO/Window.h
        src/Kraken/Graphics/GraphicsContext.cpp
        src/Kraken/Graphics/GraphicsContext.h
        src/Kraken/Graphics/Renderer.h
        "src/Kraken/Graphics/GraphicsAPI.h"
        src/Kraken/Platform/OpenGL/OpenGLContext.cpp
        src/Kraken/Platform/OpenGL/OpenGLContext.h
        src/Kraken/Math/Color.h
        src/Kraken/Graphics/Shader.cpp
        src/Kraken/Graphics/Shader.h
        src/Kraken/Platform/OpenGL/OpenGLShader.cpp
        src/Kraken/Platform/OpenGL/OpenGLShader.h
        src/Kraken/Utils/Files.cpp
        src/Kraken/Utils/Files.h
        "src/Kraken/Graphics/GraphicsAPI.cpp"
        "src/Kraken/Platform/OpenGL/OpenGLGraphicsAPI.cpp"
        "src/Kraken/Platform/OpenGL/OpenGLGraphicsAPI.h"
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
        src/Kraken/Graphics/Camera.cpp
        src/Kraken/Platform/PlatformUtils.h
        src/Kraken/Platform/Windows/Windows.cpp
        src/Kraken/Platform/Time.h
        src/Kraken/Debug/Instrumentor.h
        "src/Kraken/Assets/Asset.h"
        src/Kraken/Graphics/UniformBuffer.h
        src/Kraken/Platform/OpenGL/OpenGLUniformBuffer.cpp
        src/Kraken/Platform/OpenGL/OpenGLUniformBuffer.h
        "src/Kraken/Platform/OpenGL/OpenGLTexture.h"
        "src/Kraken/Assets/Asset.cpp"
        "src/Kraken/Graphics/Texture.h"
 "src/Kraken/Platform/OpenGL/OpenGLTexture.cpp" "src/Kraken/Utils/OrthographicCameraController.h" "src/Kraken/Utils/OrthographicCameraController.cpp" "src/Kraken/Renderer/Renderer2D.h" "src/Kraken/Renderer/Renderer2D.cpp"   "src/Kraken/Renderer/Sprite.h" "src/Kraken/Renderer/Sprite.cpp" "src/Kraken/Renderer/Font.h" "src/Kraken/Renderer/Font.cpp")

target_include_directories(Kraken PUBLIC src)
target_precompile_headers(Kraken PRIVATE src/krpch.h) # Precompiled headers
target_compile_definitions(Kraken PUBLIC KR_VERSION="1.0.0")
set_property(TARGET Kraken PROPERTY CMAKE_CXX_STANDARD 20)

# Preprocessor build type definition
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Kraken PUBLIC KR_DEBUG)
    target_compile_definitions(Kraken PUBLIC KR_PROFILE)
else ()
    target_compile_definitions(Kraken PUBLIC KR_RELEASE)
endif ()

# Universal Dependencies
find_package(spdlog)
find_package(glm)
find_package(unofficial-shaderc)
find_package(spirv_cross_core)
find_package(spirv_cross_glsl)
set(KR_LIBS spdlog::spdlog glm::glm unofficial::shaderc::shaderc spirv-cross-core spirv-cross-glsl stb_image msdf-atlas-gen)

### Platform Support
if(WIN32)
    message("KRAKEN PLATFORM: Windows")
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
    target_compile_definitions(Kraken PUBLIC KR_SUPPORT_OPENGL)
    find_package(glad CONFIG REQUIRED)
    set(KR_LIBS ${KR_LIBS} glad::glad)
endif()
if(support_glfw)
    target_compile_definitions(Kraken PUBLIC KR_SUPPORT_GLFW)
    find_package(glfw3)
    set(KR_LIBS ${KR_LIBS} glfw)
endif()

## Link Dependencies
target_link_libraries(Kraken PUBLIC ${KR_LIBS})