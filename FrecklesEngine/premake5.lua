project "FrecklesEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(bin_dir)
    objdir(int_dir)

    staticruntime "on"

    files
    {
        "src/**.cpp",
        "src/**.hpp",
        "vendor/stb/**.cpp",
        "vendor/stb/**.h"
    }

    includedirs
    {
        "src",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.glad}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.stb}",
        "%{IncludeDirs.ImGui}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }
	
	links
	{
		"ImGui"
	}	

    filter "system:linux"
        links
        {
            "Glad",
            "GLFW"			
        }
        pic "On"

    filter "system:windows"
        links
        {
            "Glad", 
            "GLFW"
        }

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        runtime "Release"
        optimize "On"

    group "Dependencies"
        include "FrecklesEngine/vendor/glad"
        include "FrecklesEngine/vendor/glfw"
		include "FrecklesEngine/vendor/ImGui"
    group ""