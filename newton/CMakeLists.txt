cmake_minimum_required(VERSION 3.20)
project(newton)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCES "core/*.cpp" "newton/*.cpp" "physics/*.cpp")

add_library(newton STATIC
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES})

target_include_directories(newton
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/newton"
    "${CMAKE_CURRENT_SOURCE_DIR}/physics")

target_compile_definitions(newton
    PUBLIC
    $<$<CONFIG:Debug>:_NEWTON_STATIC_LIB _DEBUG _LIB>
    $<$<CONFIG:Release>:_NEWTON_STATIC_LIB NDEBUG _LIB>)
