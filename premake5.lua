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

    flags
	{
		"MultiProcessorCompile"
	}
    
    
group "Dependencies"
    include "FrecklesEngine/vendor/glad"
    include "FrecklesEngine/vendor/glfw"
group ""
    
include "FrecklesEngine"

include "TestApp"