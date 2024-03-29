project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Engine/vendor/spdlog/include",
		"%{wks.location}/Engine/src",
		"%{wks.location}/Engine/vendor",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HOPE_PLATFORM_WINDOWS"
		}

		filter "system:windows"
		systemversion "latest"

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