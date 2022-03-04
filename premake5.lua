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
Dependencies["glm"] = "Blind/vendor/glm"
Dependencies["stb_image"] = "Blind/vendor/stb_image"

group "Dependencies"
	include "Blind/vendor/GLFW"
	include "Blind/vendor/Glad"
	include "Blind/vendor/imgui"
group ""

project "Blind"
	location "Blind"
	kind "StaticLib"
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
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{Dependencies.GLFW}",
		"%{Dependencies.Glad}",
		"%{Dependencies.ImGui}",
		"%{Dependencies.glm}",
		"%{Dependencies.stb_image}"
	}

	links
	{
		"GLFW",
		"ImGui",
		"Glad",
		"opengl32.lib"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BLIND_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM",
			"IMGUI_API=__declspec(dllexport)"
		}

	filter "configurations:Debug"
		defines "BLIND_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "BLIND_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BLIND_DIST"
		runtime "Release"
		optimize "on"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17";
	staticruntime "on"

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
		"Blind/src",
		"Blind/vendor",
		"%{Dependencies.glm}",		
	}
	links
	{
		"Blind"
	}
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines 
		{
			"BLIND_PLATFORM_WINDOWS",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM",
		}
	
	filter "configurations:Debug"
		defines "BLIND_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BLIND_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "BLIND_DIST"
		runtime "Release"
		optimize "on"