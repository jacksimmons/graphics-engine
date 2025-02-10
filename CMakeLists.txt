cmake_minimum_required(VERSION 3.12)

# Engine DLL
project("Engine")
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_NAME}/src/*.hpp"
    "${PROJECT_NAME}/src/*.cpp"
)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_include_directories(${PROJECT_NAME} PRIVATE
    "include"
    "include/glm"
    "${PROJECT_NAME}/include"
    "${PROJECT_NAME}/src"
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    BUILD_DLL
    GLM_ENABLE_EXPERIMENTAL
    FMT_UNICODE=0
)

# Application EXE
project("Application")
add_executable(${PROJECT_NAME} "${PROJECT_NAME}/src/editor.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_20")
target_include_directories(${PROJECT_NAME} PRIVATE
    "include"
    "include/glm"
    "Engine/src"
    "${PROJECT_NAME}/src"
    "${PROJECT_NAME}/include"
    "${PROJECT_NAME}/include/imgui"
    "${PROJECT_NAME}/include/imgui/backends"
    "vendor/assimp/include"
)
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
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})

# Provide root directory to the application
get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLM_ENABLE_EXPERIMENTAL
    FMT_UNICODE=0
    ROOT_DIRECTORY="${ROOT_DIR}"
)
find_package(OpenGL REQUIRED)
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
# Copy all required DLL/LIBs into the output dir
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_NAME}/lib $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
