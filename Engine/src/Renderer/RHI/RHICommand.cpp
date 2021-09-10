#include "hpch.h"
#include "RHICommand.h"

#include "Platform/OpenGl/Renderer/OpenGLRenderer.h"

namespace HEngine {

RHI* RHICommand::s_RHI = new OpenGLRenderer;
}
