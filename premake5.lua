workspace "Blind"
	startproject "Game"
	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Distribution"
	}

project_output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

Dependencies = {}
Dependencies["GLFW"] = "Blind/vendor/GLFW/include"
Dependencies["Glad"] = "Blind/vendor/Glad/include"
Dependencies["ImGui"] = "Blind/vendor/imgui"

include "Blind/vendor/GLFW"
include "Blind/vendor/Glad"
include "Blind/vendor/imgui"

project "Blind"
	location "Blind"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17";
	staticruntime "on"

	targetdir ("bin/" .. project_output .. "/%{prj.name}")
	objdir ("bin-int/" .. project_output .. "/%{prj.name}")

	pchheader "blindpch.h"
	pchsource "Blind/src/blindpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{Dependencies.GLFW}",
		"%{Dependencies.Glad}",
		"%{Dependencies.ImGui}",
	}

	links
	{
		"GLFW",
		"ImGui",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BLIND_PLATFORM_WINDOWS",
			"BLIND_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. project_output .. "/Game")
		}

	filter "configurations:Debug"
		defines "BLIND_DEBUG"
		buildoptions "/MDd"
		symbols "on"
	
	filter "configurations:Release"
		defines "BLIND_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "BLIND_DIST"
		buildoptions "/MD"
		optimize "on"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. project_output .. "/%{prj.name}")
	objdir ("bin-int/" .. project_output .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Blind/vendor/spdlog/include",
		"Blind/src"
	}
	links
	{
		"Blind"
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "latest"

		defines 
		{
			"BLIND_PLATFORM_WINDOWS",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}
	
	filter "configurations:Debug"
		defines "BLIND_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "BLIND_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Distribution"
		defines "BLIND_DIST"
		buildoptions "/MD"
		optimize "on"