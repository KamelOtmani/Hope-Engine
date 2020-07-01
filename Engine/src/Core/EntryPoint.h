#pragma once

#ifdef HOPE_PLATFORM_WINDOWS

extern HEngine::Application* HEngine::CreateApplication();

int main(int argc, char** argv)
{
	HEngine::Log::Init();
	HEngineWARN("Init Engine");
	HLOG("Init Client");
	std::cout << " Welcome LOLO ";
	auto app = HEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif