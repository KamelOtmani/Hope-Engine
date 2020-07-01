#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace HEngine
{
    class HOPE_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_EngineLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}
enum LOG_LEVEL
{
    Error,
    Warn,
    Info
};

// Engine Log
#define HEngineLOG(...)     ::HEngine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define HEngineWARN(...)    ::HEngine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define HEngineERROR(...)   ::HEngine::Log::GetEngineLogger()->error(__VA_ARGS__)

// Client Log
#define HLOG(...)   ::HEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define HWARN(...)  ::HEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HERROR(...) ::HEngine::Log::GetClientLogger()->error(__VA_ARGS__)