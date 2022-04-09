workspace "FrecklesEngine"
    architecture "x86_64"
    configurations { "Debug", "Release" }

    bin_dir = "%{wks.location}/bin/%{prj.name}"
    int_dir = "%{wks.location}/bin-int/%{prj.name}"

    IncludeDirs = {}
    IncludeDirs["FE"] = "%{wks.location}/FrecklesEngine/src"
    IncludeDirs["spdlog"] = "%{wks.location}/FrecklesEngine/vendor/spdlog/include"
    IncludeDirs["glfw"] = "%{wks.location}/FrecklesEngine/vendor/glfw/include"

    flags
	{
		"MultiProcessorCompile"
	}
    
include "TestApp"
include "FrecklesEngine"

group "Dependencies"
    include "FrecklesEngine/vendor/glfw/"
group ""