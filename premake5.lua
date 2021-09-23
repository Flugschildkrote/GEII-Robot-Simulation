outputdir = (_ACTION .. "/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/")


dep_glad_dir = ("./dependencies/glad/")
dep_SDL2_dir = ("./dependencies/SDL2-2.0.16/")
dep_SDL2_image_dir = ("./dependencies/SDL2_image-2.0.5/")
dep_glm_dir = ("./dependencies/glm-0.9.9.8/")
dep_tinyxml2_dir = ("./dependencies/tinyxml2-9.0.0/")
dep_bullet3_dir = ("./dependencies/bullet3-3.17/")

workspace "GEII_Simulation_Robot"
	language "C++"
	cppdialect "C++11"
	location ("./" .. _ACTION) 

	configurations { "Debug", "Release" }
	platforms { "x86", "x64" }
	
	filter { "configurations:Debug" }
		optimize "Off"
		symbols "On" 
	filter { "configurations:Release" }
		optimize "On"
		symbols "Off"
		
	filter { "platforms:x86" }
		architecture "x86"
	filter { "platforms:x64" }
		architecture "x86_64"
		
	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"

	
project "libGlad" 
	location ("./" .. _ACTION .. "/%{prj.name}") 
	language "C++"
	kind "StaticLib"
	
	targetdir (dep_glad_dir .. "lib/" .. outputdir)
	objdir ("./%{prj.location}/bin-obj/" .. outputdir)
	
	files{
		(dep_glad_dir .. "include/**"),
		(dep_glad_dir .. "src/**"),
	}
	
	includedirs
	{
		(dep_glad_dir ..  "include")
	}
	
project "libTinyXML2" 
	location ("./" .. _ACTION .. "/%{prj.name}") 
	language "C++"
	kind "StaticLib"
	
	targetdir (dep_glad_dir .. "lib/" .. outputdir)
	objdir ("./%{prj.location}/bin-obj/" .. outputdir)
	
	files{
		(dep_tinyxml2_dir .. "tinyxml2.h"),
		(dep_tinyxml2_dir .. "tinyxml2.cpp"),
	}
	
	includedirs
	{
		(dep_tinyxml2_dir)
	}
	

project "Robot Simulator"
	location ("./" .. _ACTION .. "/%{prj.name}") 
	language "C++"
	kind "ConsoleApp"
	debugdir "."
	
	targetdir ("./bin/" .. outputdir)
	objdir ("./%{prj.location}/bin-obj/" .. outputdir)
	dependson { "libGlad", "libTinyXML2" }
	
	files{
		"./Simulation/src/**.cpp",
		"./Simulation/include/**.h",
		"./GlowWormEngine/src/**.cpp",
		"./GlowWormEngine/include/**.h",
		"./*.cpp",
		"./*.h",
	}
	
	includedirs
	{
		("."),
		("./Simulation/include"),
		("./GlowWormEngine/include"),
		
		(dep_glad_dir .. "include"),
		(dep_SDL2_dir .. "include"),
		(dep_SDL2_image_dir .. "include"),
		(dep_bullet3_dir .. "src"),
		(dep_tinyxml2_dir),

		(dep_glm_dir)
	}
	
	libdirs
	{
		(dep_glad_dir .. "lib/" .. outputdir),
		(dep_tinyxml2_dir .. "lib/" .. outputdir),
		(dep_SDL2_dir .. "lib/" .. outputdir),
		(dep_SDL2_image_dir .. "lib/" .. outputdir),
		(dep_bullet3_dir .. "lib/" .. outputdir),
	}
	
	links
	{
		"libGlad",
		"libTinyXML2",
		"SDL2",
		"SDL2_image",
	}
	
		
	postbuildcommands 
	{ 
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_dir .. "lib/" .. outputdir .. "SDL2.dll %{cfg.buildtarget.directory}SDL2.dll"),
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_image_dir .. "lib/" .. outputdir .. "SDL2_image.dll %{cfg.buildtarget.directory}SDL2_image.dll"),
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_image_dir .. "lib/" .. outputdir .. "zlib1.dll %{cfg.buildtarget.directory}zlib1.dll"),
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_image_dir .. "lib/" .. outputdir .. "libjpeg-9.dll %{cfg.buildtarget.directory}libjpeg-9.dll"),
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_image_dir .. "lib/" .. outputdir .. "libpng16-16.dll %{cfg.buildtarget.directory}libpng16-16.dll"),
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_image_dir .. "lib/" .. outputdir .. "libtiff-5.dll %{cfg.buildtarget.directory}libtiff-5.dll"),
		("{COPYFILE} %{wks.location}../" .. dep_SDL2_image_dir .. "lib/" .. outputdir .. "libwebp-7.dll %{cfg.buildtarget.directory}libwebp-7.dll"),
	}
	
	filter { "configurations:Debug" }
		links 
		{
			"LinearMath_Debug",
			"Bullet3Common_Debug",
			"Bullet3Geometry_Debug",
			"BulletCollision_Debug",
			"BulletDynamics_Debug"
		}
	filter { "configurations:Release" }
		links 
		{
			"LinearMath",
			"Bullet3Common",
			"Bullet3Geometry",
			"BulletCollision",
			"BulletDynamics"
		}
	
	
