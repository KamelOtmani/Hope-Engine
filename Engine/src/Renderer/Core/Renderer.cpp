#include "hpch.h"
#include "Renderer.h"
#include "ECS/Entity.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include <ECS\Components.h>
#include <glad\glad.h>

namespace HEngine {
	void Renderer::Initialise()
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	void Renderer::PrepareScene(Scene* scene)
	{
		//HLOG("Preparing scene");
		RHICommand::SetClearColor(Vec4(0.0, 0.0, 0.0, 1));
		RHICommand::Clear();
		scene->UpdateLightsInfo();
	}
	void Renderer::SubmitScene(Scene* scene,EditorCamera& camera)
	{
		Mat4 ViewProjectionMatrix = Mat4(1.0f);
		ViewProjectionMatrix = camera.GetViewProjection();
        Vec3 lightPos = Vec3{ 0.0f };
        Vec4 LightColor = Vec4{ 1.0f };
		float LightIntensity = 1.0f;


		{
			auto group = scene->m_Registry.view<TransformComponent,MeshRendererComponent>();
			for (auto& entity : group)
			{
				auto [xform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
				if (!mesh.bEmpty && mesh.bSouldRender)
				{
					if (mesh.material->shader != nullptr)
					{

						mesh.material->shader->Bind();
                        mesh.material->shader->SetInt("u_num_point_light", static_cast<int>(scene->PointLightList.size()));
						int i = 0;
						for (auto& pointlight : scene->PointLightList)
						{
							mesh.material->shader->SetFloat3("u_PointLights[" + std::to_string(i) + "].position", pointlight.Position);
                            mesh.material->shader->SetFloat4("u_PointLights[" + std::to_string(i) + "].color", pointlight.Color);
							i++;
						}
                        mesh.material->shader->SetFloat3("u_CameraPositionWS", camera.GetPosition());
						mesh.material->shader->SetMat4("u_Transform", xform.Matrix());
                        mesh.material->shader->SetMat4("u_ViewProjection", ViewProjectionMatrix);
                        mesh.material->shader->SetMat4("u_View", camera.GetViewMatrix());
                        mesh.material->shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
						mesh.material->ApplyMaterial();
					}
					mesh.vertexArray->Bind();
					RHICommand::DrawIndexed(mesh.vertexArray);
				}

			}
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
                sprite.m_Shader->SetMat4("u_View", camera.GetViewMatrix());
                sprite.m_Shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
            }
			else if (scene->m_DefaultShader)
			{
                scene->m_DefaultShader->Bind();
                scene->m_DefaultShader->SetFloat4("u_Color", sprite.m_Color);
                scene->m_DefaultShader->SetMat4("u_Transform", xform.Matrix());
                scene->m_DefaultShader->SetMat4("u_ViewProjection", ViewProjectionMatrix);
                scene->m_DefaultShader->SetMat4("u_View", camera.GetViewMatrix());
                scene->m_DefaultShader->SetMat4("u_Projection", camera.GetProjectionMatrix());
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