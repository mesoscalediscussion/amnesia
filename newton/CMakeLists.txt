if (NOT TARGET newton)
    file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/newton/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/physics/*.cpp")

    add_library(newton STATIC
        ${PROJECT_SOURCES})

    target_include_directories(newton
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/core"
        "${CMAKE_CURRENT_SOURCE_DIR}/newton"
        "${CMAKE_CURRENT_SOURCE_DIR}/physics")

    target_compile_definitions(newton
        PUBLIC
        $<$<CONFIG:Debug>:_NEWTON_STATIC_LIB _DEBUG _LIB>
        $<$<CONFIG:Release>:_NEWTON_STATIC_LIB NDEBUG _LIB>)
endif()
