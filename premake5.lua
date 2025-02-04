workspace "TankEnginePremake"  
    configurations { "Debug", "Release" }
    platforms { "Win64" }
    -- filter { "platforms:Win32" }
    --     system "Windows"
    --     architecture "x86"
    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"
    filter "configurations:Debug"
        defines { "DEBUG" }  
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    libdirs {
        "%{prj.name}/bin/%{cfg.buildcfg}"
    }
    links { "assimp-vc143-mt", "opengl32", "glfw3", "glfw3dll" }
    includedirs {
        "include",
        "include/imgui",
        "include/imgui/backends",
        "%{prj.name}/include",
        "%{prj.name}/src",
        "vendor/assimp/include",
    }
    files {
        "include/stb_image.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "include/imgui/imgui.cpp",
        "include/imgui/imgui_demo.cpp",
        "include/imgui/imgui_draw.cpp",
        "include/imgui/imgui_tables.cpp",
        "include/imgui/imgui_widgets.cpp",
        "include/imgui/backends/imgui_impl_glfw.cpp",
        "include/imgui/backends/imgui_impl_opengl3.cpp",
        "include/glad/glad.cpp",
    }
    defines { "BUILD_DLL", "GLM_ENABLE_EXPERIMENTAL", "FMT_UNICODE=0" }
    objdir "%{prj.name}/obj/%{cfg.buildcfg}"
    targetdir "%{prj.name}/bin/%{cfg.buildcfg}"

project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    libdirs { "bin/%{cfg.buildcfg}" }
    links { "Engine", "glfw3", "glfw3dll" }
    includedirs {
        "Engine/src",
        "include",
        "include/imgui",
        "include/imgui/backends",
        "%{prj.name}/src",
        "%{prj.name}/include",
    }
    files {
        "include/stb_image.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "include/imgui/imgui.cpp",
        "include/imgui/imgui_demo.cpp",
        "include/imgui/imgui_draw.cpp",
        "include/imgui/imgui_tables.cpp",
        "include/imgui/imgui_widgets.cpp",
        "include/imgui/backends/imgui_impl_glfw.cpp",
        "include/imgui/backends/imgui_impl_opengl3.cpp",
        "include/glad/glad.cpp",
    }
    defines { "GLM_ENABLE_EXPERIMENTAL", "FMT_UNICODE=0" }
    prebuildcommands {
        "{COPYDIR} Engine/bin/%{cfg.buildcfg} bin/%{cfg.buildcfg}",
    }
    objdir "obj/%{cfg.buildcfg}"
    targetdir "bin/%{cfg.buildcfg}"
