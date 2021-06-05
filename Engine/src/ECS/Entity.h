#pragma once

#include "Renderer/Mesh.h"
#include "Renderer/Core/VertexArray.h"
#include "Renderer/Material.h"

namespace HEngine {


	struct Component
	{
		uint32_t id;
	};

	struct TransformComponent : Component
	{
		Vec3 Position{ 0.0f };
		Vec3 Rotation{ 0.0f };
		Vec3 Scale{ 0.0f };
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



}