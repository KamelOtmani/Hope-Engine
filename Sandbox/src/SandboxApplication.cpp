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
		
	}

	void OnEvent(HEngine::Event& event) override
	{
		
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