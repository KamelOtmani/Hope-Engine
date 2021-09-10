#pragma once

#ifdef HOPE_PLATFORM_WINDOWS

extern HEngine::Application* HEngine::CreateApplication();

int main(int argc, char** argv)
{
    HEngine::Log::Init();
    HEngineINFO("Init Engine");
    HLOG("Init Client");
    auto app = HEngine::CreateApplication();
    HEngineINFO("Created Application successfully !");
    app->Run();
    delete app;
}

#endif
