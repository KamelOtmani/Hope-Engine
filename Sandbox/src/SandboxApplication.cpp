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
		if (HEngine::Input::IsKeyPressed(HKEY_TAB))
		{
			HLOG("Tab was pressed");
		}
		
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
	PushLayer(new HEngine::ImGuiLayer());
}

SandboxApp::~SandboxApp()
{
}

HEngine::Application* HEngine::CreateApplication()
{
	return new SandboxApp();

}