workspace "TankEnginePremake"  
    configurations { "Debug", "Release" }
    platforms { "Win64" }
    -- filter { "platforms:Win32" }
    --     system "Windows"
    --     architecture "x86"
    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"

project "Engine"
    kind "SharedLib"
    language "C++"
    includedirs { "%{prj.name}/include" }
    objdir "%{prj.name}/obj/%{cfg.buildcfg}"
    targetdir "%{prj.name}/bin/%{cfg.buildcfg}"
    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" } 
    defines { "BUILD_DLL" }
    filter "configurations:Debug"
        defines { "DEBUG" }  
        symbols "On"
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    objdir "obj/%{cfg.buildcfg}"
    targetdir "bin/%{cfg.buildcfg}"
    libdirs { "bin/%{cfg.buildcfg}", "%{prj.name}/lib" }
    links { "Engine", "opengl32", "glfw3", "glfw3dll" }
    defines { "GLM_ENABLE_EXPERIMENTAL" }
    includedirs { "include",
        "Engine/src",
        "%{prj.name}/src",
        "%{prj.name}/include",
        "%{prj.name}/include/glm",
        "%{prj.name}/include/imgui",
        "%{prj.name}/include/imgui/backends"
    }
    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/include/imgui/imgui.cpp",
        "%{prj.name}/include/imgui/imgui_demo.cpp",
        "%{prj.name}/include/imgui/imgui_draw.cpp",
        "%{prj.name}/include/imgui/imgui_tables.cpp",
        "%{prj.name}/include/imgui/imgui_widgets.cpp",
        "%{prj.name}/include/imgui/backends/imgui_impl_glfw.cpp",
        "%{prj.name}/include/imgui/backends/imgui_impl_opengl3.cpp",
        "%{prj.name}/include/glad/glad.cpp",
    }
    prebuildcommands {
        "{COPYFILE} Engine/bin/%{cfg.buildcfg}/Engine.dll bin/%{cfg.buildcfg}/Engine.dll",
        "{COPYFILE} Engine/bin/%{cfg.buildcfg}/Engine.lib bin/%{cfg.buildcfg}/Engine.lib"
    }