#include "hpch.h"
#include "Renderer.h"

namespace HEngine {
	void Renderer::BeginScene()
	{
		RHICommand::SetClearColor(Vec4(0.1, 0.1, 0.1, 1));
		RHICommand::Clear();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RHICommand::DrawIndexed(vertexArray);
	}
}