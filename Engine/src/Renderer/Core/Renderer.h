#pragma once

#include "Renderer/RHI/RHICommand.h"
#include "ECS/Scene.h"

namespace HEngine {

	class Renderer
	{
	public:
	Renderer() { HLOG("Constuct"); }

	static void Initialise();
	//static void setupScene(Scene* scene);
	static void prepareScene(Scene* scene);
	static void submitScene(Scene* scene);

		// Old stuff
	static void BeginScene();
	static void EndScene();
	   
	static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

	inline static RHI::API GetAPI() { return RHI::GetAPI(); }
	};


}