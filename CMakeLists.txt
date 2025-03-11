        ${PROJECT_SOURCES}
cmake_minimum_required(VERSION 3.12) # Required for CSharp
project("TankEngine")
    add_subdirectory("Application")
    add_subdirectory("Tank-ScriptCore")
    add_subdirectory("Engine")
