project "TestApp3D"
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
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.glm}" ,
        "%{IncludeDirs.ImGui}",
        "%{IncludeDirs.entt}",
        "%{IncludeDirs.debugbreak}"
    }

    links 
    {
        "FrecklesEngine"        
    }

    filter "system:linux"
        links
        {            
            "GLFW",
            "Glad",
            "ImGui"
        }
        
        pic "On"
    
    filter "system:windows"
        
    filter "configurations:Debug"
        runtime "Debug"
        defines "DEBUG"
        symbols "On"
 
    filter "configurations:Release"
        runtime "Release"
        defines  "NDEBUG" 
        optimize "On"
