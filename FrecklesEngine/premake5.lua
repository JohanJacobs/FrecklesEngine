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
        "src/**.hpp"
    }

    includedirs
    {
        "src",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.glad}"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_"
    }

    links
    {
        "GLFW",
        "Glad"
    }

    filter "system:linux"
        pic "On"

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"

