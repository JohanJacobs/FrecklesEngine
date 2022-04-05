project "FrecklesEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(bin_dir)
    objdir(bin_int_dir)


    files
    {
        "src/**.cpp",
        "src/**.hpp"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
