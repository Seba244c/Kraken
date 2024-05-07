//
// Created by sebsn on 07-05-2024.
//

#include "RenderCommand.h"

namespace Kraken {
    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}