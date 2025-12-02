if (NOT TARGET amnesia-amfp)
    add_subdirectory("../hpl2" lib/hpl2)

    file(GLOB_RECURSE PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
    list(REMOVE_ITEM PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/_old/LuxEnemy.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/_old/LuxEnemy_ManPig.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/_old/LuxEnemy_Tesla.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/_old/LuxEnemyMover.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/_old/LuxEnemyPathfinder.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/LuxInsanityHandler.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/LuxProp_OilBarrel.cpp")

    add_executable(amnesia-amfp WIN32 ${PROJECT_SOURCES})
    target_link_libraries(amnesia-amfp PRIVATE hpl2)
    target_compile_definitions(amnesia-amfp
        PRIVATE
        $<$<CONFIG:Debug>:_DEBUG _WINDOWS MEMORY_MANAGER_ACTIVE USE_GAMEPAD>
        $<$<CONFIG:Release>:NDEBUG _WINDOWS USE_GAMEPAD>)
endif()
