#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace HEngine
{
    class HOPE_API Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static Ref<spdlog::logger> s_EngineLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };

}
enum LOG_LEVEL
{
    Error,
    Warn,
    Info
};

// Engine Log
#define HEngineINFO(...)     ::HEngine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define HEngineLOG(...)     ::HEngine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define HEngineWARN(...)    ::HEngine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define HEngineERROR(...)   ::HEngine::Log::GetEngineLogger()->error(__VA_ARGS__)

// Client Log
#define HINFO(...)   ::HEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define HLOG(...)   ::HEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HWARN(...)  ::HEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HERROR(...) ::HEngine::Log::GetClientLogger()->error(__VA_ARGS__)