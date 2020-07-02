#pragma once

#ifdef HOPE_PLATFORM_WINDOWS
    #ifdef HOPE_BUILD_DLL
        #define HOPE_API __declspec(dllexport)
    #else
        #define HOPE_API __declspec(dllimport)
    #endif
#else
    #error HEngine only supports Windows!
#endif

// enable asserts
#if HOPE_DEBUG
    #define HENABLE_ASSERTS = true;
#else 
    #define HENABLE_ASSERTS = false;
#endif // HOPE_DEBUG

#ifdef HENABLE_ASSERTS
#define HASSERT(x, ...) { if(!(x)) { HEngineERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define HENGINE_ASSERT(x, ...) { if(!(x)) { HERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define HASSERT(x, ...) 
#define HENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x) 