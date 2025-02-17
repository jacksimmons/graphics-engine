cmake_minimum_required(VERSION 3.12)
include(ExternalProject)

# Engine DLL
project("Engine")
    file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_NAME}/src/*.hpp"
        "${PROJECT_NAME}/src/*.cpp"
    )
    file(GLOB_RECURSE LUA_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_NAME}/include/lua/*.c"
    )
    add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
        ${LUA_SOURCES}
    )
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}" FILES
        ${PROJECT_SOURCES}
        ${LUA_SOURCES}
    )
    target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_20")
    target_include_directories(${PROJECT_NAME} PRIVATE
        "include"
        "include/glm"
        "${PROJECT_NAME}/include"
        "${PROJECT_NAME}/include/lua"
        "${PROJECT_NAME}/src"
        "vendor/assimp/include"
    )
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_NAME}/lib"
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        TANK_DLL
        GLM_ENABLE_EXPERIMENTAL
        FMT_UNICODE=0
        ROOT_DIRECTORY="${ROOT_DIR}"
    )
    # Provide root directory to the application
    get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
    # Copy all required DLL/LIBs into the output dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_NAME}/lib $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )


# Application EXE
project("Application")
    file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_NAME}/src/*.hpp"
        "${PROJECT_NAME}/src/*.cpp"
        "${PROJECT_NAME}/include/imgui/imgui.cpp"
        "${PROJECT_NAME}/include/imgui/imgui_demo.cpp"
        "${PROJECT_NAME}/include/imgui/imgui_draw.cpp"
        "${PROJECT_NAME}/include/imgui/imgui_tables.cpp"
        "${PROJECT_NAME}/include/imgui/imgui_widgets.cpp"
        "${PROJECT_NAME}/include/imgui/backends/imgui_impl_glfw.cpp"
        "${PROJECT_NAME}/include/imgui/backends/imgui_impl_opengl3.cpp"
        "${PROJECT_NAME}/include/glad/glad.cpp"
    )
    add_executable(${PROJECT_NAME} "${PROJECT_NAME}/src/editor.cpp"
        ${PROJECT_SOURCES}
    )
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}" FILES
        ${PROJECT_SOURCES}
    )
    find_package(OpenGL REQUIRED)
    target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_20")
    target_include_directories(${PROJECT_NAME} PRIVATE
        "include"
        "include/glm"
        "Engine/src"
        "${PROJECT_NAME}/src"
        "${PROJECT_NAME}/include"
        "${PROJECT_NAME}/include/imgui"
        "${PROJECT_NAME}/include/imgui/backends"
    )
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_NAME}/lib"
    )
    target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        "Engine"
        "glfw3"
        "glfw3dll"
        "assimp-vc143-mt"
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLM_ENABLE_EXPERIMENTAL
        FMT_UNICODE=0
        ROOT_DIRECTORY="${ROOT_DIR}"
    )
    # Copy all required DLL/LIBs into the output dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_NAME}/lib $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )