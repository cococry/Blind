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

project "Blind"
	location "Blind"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	pchheader "blindpch.h"
	pchsource "Blind/src/blindpch.cpp"

	targetdir ("bin/" .. project_output .. "/%{prj.name}")
	objdir ("bin-int/" .. project_output .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

		defines 
		{
			"BLIND_PLATFORM_WINDOWS",
			"BLIND_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. project_output .. "/Game")
		}

	filter "configurations:Debug"
		defines "BLIND_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "BLIND_RELEASE"
		optimize "on"

	filter "configurations:Distribution"
		defines "BLIND_DIST"
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
		}
	
	filter "configurations:Debug"
		defines "BLIND_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "BLIND_RELEASE"
		optimize "on"

	filter "configurations:Distribution"
		defines "BLIND_DIST"
		optimize "on"