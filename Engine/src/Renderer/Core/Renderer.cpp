#include "hpch.h"
#include "Renderer.h"
#include "ECS/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include <ECS\Components.h>

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
		Mat4 ViewProjectionMatrix = Mat4(1.0f);
		auto view = scene->m_Registry.view<CameraComponent,TransformComponent>();
		for (auto entity : view)
		{
			auto [cam, transform] = view.get<CameraComponent,TransformComponent>(entity);
			if (cam.bPrimary)
			{
				ViewProjectionMatrix = cam.Projection() * glm::inverse(transform.Matrix());
				break;
			}
		}
		auto group = scene->m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
		for (auto entity : group)
		{
			auto [xform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
			if (mesh.shader != nullptr)
			{
				mesh.shader->Bind();
				mesh.shader->SetMat4("u_Transform", xform.Matrix());
				mesh.shader->SetMat4("u_ViewProjection",ViewProjectionMatrix);
			}
			mesh.vertexArray->Bind();
			RHICommand::DrawIndexed(mesh.vertexArray);

		}

        auto group2 = scene->m_Registry.view<TransformComponent,SpriteRendererComponent>();
        for (auto entity : group2)
        {
            auto [xform, sprite] = group2.get<TransformComponent, SpriteRendererComponent>(entity);
            if (sprite.m_Shader)
            {
				sprite.m_Shader->Bind();
                sprite.m_Shader->SetFloat4("u_Color", sprite.m_Color);
                sprite.m_Shader->SetMat4("u_Transform", xform.Matrix());
				sprite.m_Shader->SetMat4("u_ViewProjection", ViewProjectionMatrix);
            }
			else if (scene->m_DefaultShader)
			{
                scene->m_DefaultShader->Bind();
                scene->m_DefaultShader->SetFloat4("u_Color", sprite.m_Color);
                scene->m_DefaultShader->SetMat4("u_Transform", xform.Matrix());
                scene->m_DefaultShader->SetMat4("u_ViewProjection", ViewProjectionMatrix);
			}
			scene->m_QuadVAO->Bind();
            RHICommand::DrawIndexed(scene->m_QuadVAO);

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

	void Renderer::Submit(const Ref<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RHICommand::DrawIndexed(vertexArray);
	}
}