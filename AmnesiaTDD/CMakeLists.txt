if (NOT TARGET amnesia-tdd)
    add_subdirectory("../hpl2" lib/hpl2)

    file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

    add_executable(amnesia-tdd WIN32 ${PROJECT_SOURCES})
    target_link_libraries(amnesia-tdd PUBLIC hpl2)
    target_compile_definitions(amnesia-tdd
        PUBLIC
        $<$<CONFIG:Debug>:_DEBUG _WINDOWS MEMORY_MANAGER_ACTIVE USE_GAMEPAD>
        $<$<CONFIG:Release>:NDEBUG _WINDOWS USE_GAMEPAD>)
endif()
