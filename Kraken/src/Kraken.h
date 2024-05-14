#ifndef KRAKEN_APP
#define KRAKEN_APP
// This is used by Kraken applications to automatically include everything needed

//-Application--------------------------------
#include "Kraken/Core/Base.h"
#include "Kraken/Core/Application.h"
#include "Kraken/Core/Layer.h"
#include "Kraken/Core/Time.h"
#include "Kraken/IO/Input.h"

//--Graphics----------------------------------
#include "Kraken/Graphics/Renderer.h"
#include "Kraken/Graphics/RenderCommand.h"

#include "Kraken/Graphics/Camera.h"
#include "Kraken/Graphics/Shader.h"
#include "Kraken/Graphics/VertexArray.h"
//--------------------------------------------

//-Entrypoint---------------------------------
#include "Kraken/Core/Entrypoint.h"
//--------------------------------------------

#endif //KRAKEN_APP
