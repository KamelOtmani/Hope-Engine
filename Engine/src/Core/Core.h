#pragma once

#include <memory>
#include <vector>

#define HOPE_DYNAMIC_LINK 0

#ifdef HOPE_PLATFORM_WINDOWS
    #if HOPE_DYNAMIC_LINK

        #ifdef HOPE_BUILD_DLL
            #define HOPE_API __declspec(dllexport)
        #else
            #define HOPE_API __declspec(dllimport)
        #endif
    #else
        #define HOPE_API
#endif // HOPE_DYNAMIC_LINK
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

#define HBIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> CreateScope(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Vector = std::vector<T>;