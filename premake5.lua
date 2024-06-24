workspace "DessertGameEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to a root folder (Solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "DessertGameEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "DessertGameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "DessertGameEngine/vendor/imgui"
IncludeDir["glm"] = "DessertGameEngine/vendor/glm"
IncludeDir["stb_image"] = "DessertGameEngine/vendor/stb_image"

include "DessertGameEngine/vendor/GLFW"
include "DessertGameEngine/vendor/Glad"
include "DessertGameEngine/vendor/imgui"


startproject = "Sandbox"

project "DessertGameEngine"
	location "DessertGameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/DessertGameEngine"
	objdir "bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/DessertGameEngine"

	pchheader "dgepch.h"
	pchsource "DessertGameEngine/src/dgepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"$(SolutionDir)DessertGameEngine/src",
		"$(SolutionDir)DessertGameEngine/vendor/spdlog/include",
		"$(SolutionDir)%{IncludeDir.GLFW}",
		"$(SolutionDir)%{IncludeDir.Glad}",
		"$(SolutionDir)%{IncludeDir.ImGui}",
		"$(SolutionDir)%{IncludeDir.glm}",
		"$(SolutionDir)%{IncludeDir.stb_image}",

	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DGE_PLATFORM_WINDOWS",
			"DGE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}


	filter "configurations:Debug"
		defines "DGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DGE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DGE_DIST"
		runtime "Release"
		optimize "on"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Sandbox")
	objdir ("bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Sandbox")

	files
	{
		"Sandbox/src/**.h",
		"Sandbox/src/**.cpp"
	}

	includedirs
	{
		"DessertGameEngine/vendor/spdlog/include",
		"DessertGameEngine/src",
		"DessertGameEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"DessertGameEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DGE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DGE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DGE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DGE_DIST"
		runtime "Release"
		optimize "on"