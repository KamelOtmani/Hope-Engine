#pragma once

#include "Renderer/Mesh.h"
#include "Renderer/Core/VertexArray.h"
#include "Renderer/Material.h"
#include <glm/gtc/matrix_transform.hpp>

namespace HEngine {


	struct Component
	{
		uint32_t id;
	};

	struct TransformComponent : Component
	{
		Vec3 Position{ 0.0f };
		Vec3 Rotation{ 0.0f };
		Vec3 Scale{ 1.0f };

		Mat4 ModelMatrix()
		{
			return glm::translate(glm::mat4(1.0f), Position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 1.0f)) *
				glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct MeshComponent : Component
	{
		std::vector<FVertex> _data;
		std::vector<uint32_t> _indices;
	};

	struct MaterialComponent : Component
	{
		Shader* shader;
	};

	struct CameraComponent : Component
	{
		float fov = 90.f;
		float nearPlane = 0.1f;
		float farPlane = 10.0f;
		float hight = 1600.0f;
		float width = 900.0f;
		float aspectRatio = 16.0f / 9.0f;

		Mat4 Projection()
		{
			return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
			//return glm::ortho(0.0f, hight, 0.0f, width, nearPlane, farPlane);
		}
	};

	class Model 
	{
	public:
		Model() { HLOG("Model CONSTRUCTOR"); };
		virtual ~Model() {};

		MaterialComponent mat;
		MeshComponent mesh;
		TransformComponent transform;

		void updateMesh();
		bool shouldUpdate() { return bShouldUpdate; };

		std::shared_ptr<VertexArray>& getVertexArray() { return vao; };

	private:
		std::shared_ptr<VertexArray> vao;
		bool bShouldUpdate = false;
	};

	class Camera
	{
	public:
		Camera() { transform.Position = Vec3(0.0f, 0.0f, 3.0f); };
		virtual ~Camera() {};

		TransformComponent transform;
		CameraComponent cam;

		Mat4 ViewProjectionMatrix()
		{
			return cam.Projection() * glm::inverse(transform.ModelMatrix());
		}
	};

}