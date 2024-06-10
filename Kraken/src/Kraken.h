#pragma once
// This is used by Kraken applications to automatically include everything needed
//-Application--------------------------------
#include "Kraken/Core/Base.h"
#include "Kraken/Core/Application.h"
#include "Kraken/Core/Layer.h"
#include "Kraken/IO/Input.h"
#include "Kraken/Assets/Asset.h"

//--Graphics----------------------------------
#include "Kraken/Graphics/Renderer.h"
#include "Kraken/Graphics/RenderCommand.h"

#include "Kraken/Graphics/Camera.h"
#include "Kraken/Graphics/Shader.h"
#include "Kraken/Graphics/Texture.h"
#include "Kraken/Graphics/VertexArray.h"

#include "Kraken/Renderer/Renderer2D.h"
#include "Kraken/Renderer/Sprite.h"
//--------------------------------------------


//-Application Utils--------------------------
#include "Kraken/Utils/OrthographicCameraController.h"
#include "Kraken/Platform/PlatformUtils.h"
//--------------------------------------------

//-Entrypoint---------------------------------
#ifdef KRAKEN_APP
#include "Kraken/Core/Entrypoint.h"
#undef KRAKEN_APP
#endif
//--------------------------------------------
