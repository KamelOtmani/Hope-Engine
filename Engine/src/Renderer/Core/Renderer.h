#pragma once

#include "Renderer/RHI/RHICommand.h"

namespace HEngine {

	class Renderer
	{
	public:
	static void BeginScene();
	static void EndScene();

	static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

	inline static RHI::API GetAPI() { return RHI::GetAPI(); }
	};


}