#pragma once

#ifdef HOPE_PLATFORM_WINDOWS
    #ifdef HOPE_BUILD_DLL
        #define HOPE_API __declspec(dllexport)
    #else
        #define HOPE_API __declspec(dllimport)
    #endif
#else
    #error Hazel only supports Windows!
#endif