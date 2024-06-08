//
// Created by sebsn on 07-05-2024.
//

#include "RenderCommand.h"

namespace Kraken {
    Scope<GraphicsAPI> RenderCommand::s_GraphicsAPI = GraphicsAPI::Create();
}