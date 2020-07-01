#include <HEngine.h>

class SandboxApp : public HEngine::Application
{
public:
	SandboxApp();
	~SandboxApp();

private:

};

SandboxApp::SandboxApp()
{
}

SandboxApp::~SandboxApp()
{
}

HEngine::Application* HEngine::CreateApplication()
{
	return new SandboxApp();

}