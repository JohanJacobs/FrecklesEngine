project "TestApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "c++17"

    targetdir (bin_dir)
    objdir (bin_int_dir)
    
    files
    {
        "src/**.cpp",
        "src/**.hpp"
    }
    
    includedirs
    {
        "%{prj.location}/src",
        "%{IncludeDirs.FE}"
    }

    links 
    {
        "FrecklesEngine"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"
 
    filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "On"
