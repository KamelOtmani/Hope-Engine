#include "hpch.h"
#include "Renderer.h"
#include "ECS/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"

namespace HEngine {
	void Renderer::prepareScene(Scene* scene)
	{
		//HLOG("Preparing scene");
		RHICommand::SetClearColor(Vec4(0.1, 0.1, 0.5, 1));
		RHICommand::Clear();
	}
	void Renderer::submitScene(Scene* scene)
	{

		for (auto& mesh : scene->entityList)
		{
			if (mesh.shouldUpdate())
				mesh.updateMesh();
			if (mesh.mat.shader)
			{
				mesh.mat.shader->Bind();
			}
			mesh.getVertexArray()->Bind();
			RHICommand::DrawIndexed(mesh.getVertexArray());

		}
		//HLOG("Submiting scene");
	}
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