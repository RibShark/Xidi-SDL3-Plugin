cmake_minimum_required(VERSION 4.0)
project(Xidi_SDL3_Plugin VERSION 1.0.1
        DESCRIPTION "A Xidi plugin to accept inputs from any controller supported by the SDL3 Gamepad API."
        LANGUAGES CXX)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_STANDARD 23)

if (NOT DEFINED ARCHITECTURE)
    if (CMAKE_SIZEOF_VOID_P GREATER 4)
        set(ARCHITECTURE "64")
    else()
        set(ARCHITECTURE "32")
    endif()
endif()

add_library(${PROJECT_NAME} SHARED src/PluginEntry.cpp
        src/SDL3Backend.cpp
        src/SDL3Backend.h)
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "SDL.XidiPlugin.${ARCHITECTURE}" CXX_STANDARD 23 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 Hid)

target_include_directories(${PROJECT_NAME} PRIVATE "include")