#include "hpch.h"
#include "Renderer.h"
#include "ECS/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"

namespace HEngine {
	void Renderer::Initialise()
	{
	}
	void Renderer::prepareScene(Scene* scene)
	{
		//HLOG("Preparing scene");
		RHICommand::SetClearColor(Vec4(0.1, 0.1, 0.5, 1));
		RHICommand::Clear();
	}
	void Renderer::submitScene(Scene* scene)
	{
		auto cam = scene->CameraList[0];
		if (cam)
		{
			for (auto& mesh : scene->entityList)
			{
				if (mesh.shouldUpdate())
					mesh.updateMesh();
				if (mesh.mat.shader)
				{
					mesh.mat.shader->Bind();
					mesh.mat.shader->SetMat4("u_Transform", mesh.transform.ModelMatrix());
					//HLOG("Camera position = {0},{1},{2}", cam->transform.Position.x, cam->transform.Position.y, cam->transform.Position.z);
					mesh.mat.shader->SetMat4("u_ViewProjection", cam->ViewProjectionMatrix());
				}
				mesh.getVertexArray()->Bind();
				RHICommand::DrawIndexed(mesh.getVertexArray());

			}
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