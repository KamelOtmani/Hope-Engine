project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hpch.h"
	pchsource "src/PCH/hpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}",
		-- "%{IncludeDir.assimp}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		-- "assimp-vc142-mtd.lib",
		-- "assimp",
		"opengl32.lib"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "noPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HOPE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HOPE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HOPE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HOPE_DIST"
		runtime "Release"
		optimize "on"
	-- filter "system:windows"
	-- systemversion "latest"

	-- defines
	-- {
	-- 	"HOPE_PLATFORM_WINDOWS",
	-- 	"HOPE_BUILD_DLL",
	-- 	"GLFW_INCLUDE_NONE"
	-- }


	-- filter "configurations:Debug"
	-- 	defines "HOPE_DEBUG"
	-- 	runtime "Debug"
	-- 	symbols "On"

	-- filter "configurations:Release"
	-- 	defines "HOPE_RELEASE"
	-- 	runtime "Release"
	-- 	optimize "On"


