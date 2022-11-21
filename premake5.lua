workspace "Aluminium-Store"
    architecture "x64"
    configurations { "Debug", "Release" }

outputDir = "%{cfg.system}-%{cfg.buildcfg}"

include "Aluminium-Store/lib/GLFW"

project "Aluminium-Store"
    location "Aluminium-Store"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir("Build/" .. outputDir)
    objdir("Build/Intermediate/" .. outputDir)

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/GLFW/include",

    }

    links {

        "GLFW"

    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "AL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "AL_RELEASE"
        runtime "Release"
        optimize "on"