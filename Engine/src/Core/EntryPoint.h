#pragma once

#ifdef HOPE_PLATFORM_WINDOWS

extern HEngine::Application* HEngine::CreateApplication();

int main(int argc, char** argv)
{
	std::cout << " Welcome LOLO ";
	auto app = HEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif