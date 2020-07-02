workspace "Hope Engine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "PCH/hpch.h"
	pchsource "hpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src/",
		"%{prj.name}/src/PCH",	
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HOPE_PLATFORM_WINDOWS",
			"HOPE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "HOPE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HOPE_RELEASE"
		optimize "On"

	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HOPE_PLATFORM_WINDOWS"
		}

    filter "configurations:Debug"
		defines "HOPE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HOPE_RELEASE"
		optimize "On"

project "GLFW"
	location "Engine/vendor/GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Engine/vendor/GLFW/include/GLFW/glfw3.h",
		"Engine/vendor/GLFW/include/GLFW/glfw3native.h",
		"Engine/vendor/GLFW/src/glfw_config.h",
		"Engine/vendor/GLFW/src/context.c",
		"Engine/vendor/GLFW/src/init.c",
		"Engine/vendor/GLFW/src/input.c",
		"Engine/vendor/GLFW/src/monitor.c",
		"Engine/vendor/GLFW/src/vulkan.c",
		"Engine/vendor/GLFW/src/window.c"
	}
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files
		{
			"src/x11_init.c",
			"src/x11_monitor.c",
			"src/x11_window.c",
			"src/xkb_unicode.c",
			"src/posix_time.c",
			"src/posix_thread.c",
			"src/glx_context.c",
			"src/egl_context.c",
			"src/osmesa_context.c",
			"src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"Engine/vendor/GLFW/src/win32_init.c",
			"Engine/vendor/GLFW/src/win32_joystick.c",
			"Engine/vendor/GLFW/src/win32_monitor.c",
			"Engine/vendor/GLFW/src/win32_time.c",
			"Engine/vendor/GLFW/src/win32_thread.c",
			"Engine/vendor/GLFW/src/win32_window.c",
			"Engine/vendor/GLFW/src/wgl_context.c",
			"Engine/vendor/GLFW/src/egl_context.c",
			"Engine/vendor/GLFW/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"