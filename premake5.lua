workspace "FrecklesEngine"
    architecture "x86_64"
    configurations { "Debug", "Release" }

    bin_dir = "%{wks.location}/bin/%{prj.name}"
    bin_int_dir = "%{wks.location}/bin-int/%{prj.name}"

    IncludeDirs = {}
    IncludeDirs["FE"] = "%{wks.location}/FrecklesEngine/src"
    IncludeDirs["spdlog"] = "%{wks.location}/FrecklesEngine/vendor/spdlog/include"
    
include "TestApp"
include "FrecklesEngine"
