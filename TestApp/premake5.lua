project "TestApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir (bin_dir)
    objdir (int_dir)
    
    files
    {
        "src/**.cpp",
        "src/**.hpp"        
    }
    
    includedirs
    {
        "%{prj.location}/src",
        "%{IncludeDirs.FE}",
        "%{IncludeDirs.spdlog}"
    }

    links 
    {
        "FrecklesEngine",
        "GLFW"
    }

    filter "system:linux"
        pic "On"

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
 
    filter "configurations:Release"
       defines  "NDEBUG" 
       optimize "On"
