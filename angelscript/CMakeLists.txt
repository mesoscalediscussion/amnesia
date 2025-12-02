cmake_minimum_required(VERSION 3.20)
project(angelscript)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCES "source/*.cpp")

add_library(angelscript STATIC
    ${PROJECT_SOURCES})

target_include_directories(angelscript PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_definitions(angelscript
    PUBLIC
    $<$<CONFIG:Debug>:ANGELSCRIPT_EXPORT AS_DEBUG _DEBUG _LIB>
    $<$<CONFIG:Release>:ANGELSCRIPT_EXPORT NDEBUG _LIB>)
