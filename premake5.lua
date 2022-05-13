workspace "FrecklesEngine"
    architecture "x86_64"
    configurations { "Debug", "Release" }

    bin_dir = "%{wks.location}/bin/%{prj.name}"
    int_dir = "%{wks.location}/bin-int/%{prj.name}"

    IncludeDirs = {}
    IncludeDirs["FE"] = "%{wks.location}/FrecklesEngine/src"
    IncludeDirs["spdlog"] = "%{wks.location}/FrecklesEngine/vendor/spdlog/include"
    IncludeDirs["glfw"] = "%{wks.location}/FrecklesEngine/vendor/glfw/include"
    IncludeDirs["glad"] = "%{wks.location}/FrecklesEngine/vendor/glad/include"
    IncludeDirs["glm"] = "%{wks.location}/FrecklesEngine/vendor/glm"
    IncludeDirs["stb"] = "%{wks.location}/FrecklesEngine/vendor/stb"
    IncludeDirs["ImGui"] = "%{wks.location}/FrecklesEngine/vendor/ImGui"

    flags
	{
		"MultiProcessorCompile",
        "FatalWarnings"
	}   
        
include "FrecklesEngine"
include "TestApp"
include "TestApp3d"