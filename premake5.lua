workspace "Aluminium-Store"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Aluminium-Store"

outputDir = "%{cfg.system}-%{cfg.buildcfg}"

include "Aluminium-Store/lib/GLFW"

project "Aluminium-Store"
    location "Aluminium-Store"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("Build/Intermediate/" .. outputDir .. "/%{prj.name}")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/GLFW/include",
        
        "Aluminium-Server/lib/asio/include",
        
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

project "Aluminium-Server"
    location "Aluminium-Server"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir("Build/" .. outputDir .. "/%{prj.name}")
    objdir("Build/Intermediate/" .. outputDir .. "/%{prj.name}")

    files {

        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

    }

    includedirs {

        "%{prj.name}/src",

        "%{prj.name}/lib/asio/include"

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