#pragma once

#include "Renderer/RHI/RHICommand.h"
#include "ECS/Scene.h"
#include "Renderer/EditorCamera.h"

namespace HEngine {

	class Renderer
	{
	public:
	Renderer() { HLOG("Constuct"); }

	static void Initialise();
	//static void setupScene(Scene* scene);
	static void PrepareScene(Scene* scene);
	static void SubmitScene(Scene* scene,EditorCamera& camera);

		// Old stuff
	static void BeginScene();
	static void EndScene();
	   
	static void Submit(const Ref<VertexArray>& vertexArray);

	inline static RHI::API GetAPI() { return RHI::GetAPI(); }
	};


}