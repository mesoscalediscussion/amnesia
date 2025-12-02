if (NOT TARGET hpl2)
    if (CMAKE_TOOLCHAIN_FILE)
        include(${CMAKE_TOOLCHAIN_FILE})
    endif()

    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_subdirectory("../angelscript" lib/angelscript)
    add_subdirectory("../newton" lib/newton)
    find_package(SDL3 REQUIRED)
    find_package(OpenAL REQUIRED)
    find_package(DevIL REQUIRED)
    find_package(FLTK REQUIRED)
    find_package(ogg REQUIRED)
    find_package(unofficial-theora REQUIRED)
    find_package(Vorbis REQUIRED)
    find_package(freealut REQUIRED)
    find_package(ZLIB REQUIRED)
    find_package(OpenGL REQUIRED)

    file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")
    list(REMOVE_ITEM PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/impl/PlatformUnix.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/impl/PlatformSDL.cpp")

    add_library(hpl2 STATIC
        ${PROJECT_SOURCES})

    target_include_directories(hpl2
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        ${FLTK_INCLUDE_DIR})

    target_link_libraries(hpl2 PUBLIC
        angelscript
        newton
        SDL3::SDL3
        OpenAL::OpenAL
        DevIL::IL
        DevIL::ILU
        DevIL::ILUT
        ${FLTK_BASE_LIBRARY_RELEASE}
        Ogg::ogg
        unofficial::theora::theora
        Vorbis::vorbis
        Vorbis::vorbisfile
        Vorbis::vorbisenc
        FreeALUT::alut
        ZLIB::ZLIB
        OpenGL::GL
        OpenGL::GLU)

    target_compile_definitions(hpl2
        PUBLIC
        $<$<CONFIG:Debug>:_NEWTON_STATIC_LIB USE_SDL3 WINDOWS_IGNORE_PACKING_MISMATCH _DEBUG _USRDLL HPL_EXPORTS MEMORY_MANAGER_ACTIVE>
        $<$<CONFIG:Release>:_NEWTON_STATIC_LIB USE_SDL3 WINDOWS_IGNORE_PACKING_MISMATCH NDEBUG _USRDLL HPL_EXPORTS MEMORY_MANAGER_ACTIVE>)
endif()
