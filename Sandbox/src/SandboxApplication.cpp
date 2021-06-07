#include <HEngine.h>
#include <Core/EntryPoint.h>

#include "Editor/EditorLayer.h"

using namespace HEngine;

class SandboxApp : public Application
{
public:
	SandboxApp();
	~SandboxApp();

private:

};

SandboxApp::SandboxApp()
{
	PushLayer(new EditorLayer());
}

SandboxApp::~SandboxApp()
{
}

HEngine::Application* HEngine::CreateApplication()
{
	return new SandboxApp();

}