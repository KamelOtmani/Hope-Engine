#include <HEngine.h>
#include <ECS\Entity.h>
#include "Renderer/Shader.h"

using namespace HEngine;

class ExampleLayer : public HEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		_scene = std::make_unique<HEngine::Scene>();

		Model newModel;
		Model lol;


		auto newShader = new HEngine::Shader("assets/shaders/simpleShader.glsl");
		_scene->shaderList.push_back(newShader);

		newModel.mesh._data.push_back({ Vec3{-0.5f, -0.5f, 0.0f},Vec4{1.0f, 0.f,0.f,1.0f} });
		newModel.mesh._data.push_back({ Vec3{0.5f, -0.5f, 0.0f}, Vec4{1.0f, 0.f,0.f,1.0f} });
		newModel.mesh._data.push_back({ Vec3{0.0f, -0.0f, 0.0f}, Vec4{1.0f, 0.f,0.f,1.0f} });

		newModel.mesh._indices = { 0, 1, 2 };
		newModel.mat.shader = _scene->shaderList[0];
		newModel.transform.Position = Vec3{ 0.0f };
		newModel.updateMesh();
		_scene->entityList.push_back(newModel);

		lol.mesh._data.push_back({ Vec3{-0.5f, -0.5f, 0.0f},Vec4{0.1f, 0.1f,0.8f,1.0f} });
		lol.mesh._data.push_back({ Vec3{0.5f, -0.5f, 0.0f}, Vec4{0.1f, 0.1f,0.8f,1.0f} });
		lol.mesh._data.push_back({ Vec3{0.5f, 0.5f, 0.0f} , Vec4{0.1f, 0.1f,0.8f,1.0f} });
		lol.mesh._data.push_back({ Vec3{-0.5f, 0.5f, 0.0f}, Vec4{0.1f, 0.1f,0.8f,1.0f} });
		lol.mat.shader = _scene->shaderList[0];

		lol.mesh._indices = { 0, 1, 2,2,3,0};
		lol.transform.Position = Vec3{ 0.0f };
		lol.updateMesh();
		_scene->entityList.push_back(lol);

	}

	void OnUpdate() override
	{
		//HEngine::Renderer::prepareScene(_scene.get());
		HEngine::Renderer::submitScene(_scene.get());
	}

	void OnEvent(HEngine::Event& event) override
	{
		
	}

	std::unique_ptr<HEngine::Scene> _scene;

};

class SandboxApp : public HEngine::Application
{
public:
	SandboxApp();
	~SandboxApp();

private:

};

SandboxApp::SandboxApp()
{
	PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp()
{
}

HEngine::Application* HEngine::CreateApplication()
{
	return new SandboxApp();

}