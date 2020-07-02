#include <HEngine.h>
class ExampleLayer : public HEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		HLOG("ExampleLayer::Update");
	}

	void OnEvent(HEngine::Event& event) override
	{
		HINFO("{0}", event);
	}

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