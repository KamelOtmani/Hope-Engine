#include <HEngine.h>
#include <Core/EntryPoint.h>

#include "Editor/EditorLayer.h"

using namespace HEngine;

class EditorApp : public Application
{
public:
	EditorApp();
	~EditorApp();

private:

};

EditorApp::EditorApp()
{
	PushLayer(new EditorLayer());
}

EditorApp::~EditorApp()
{
}

HEngine::Application* HEngine::CreateApplication()
{
	return new EditorApp();

}