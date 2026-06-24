cmake_minimum_required(VERSION 3.16)

# CMake will not change any file in the source folder.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# The build process should be kept in a different folder from the main source code.
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


project(CatacombGL LANGUAGES CXX C VERSION 0.5.7)

# GoogleTests are by default not part of the build process, but can be added
# by selecting the option in CMake.
option(BUILD_TESTS "Build Tests" OFF)

# Sets the C++ Language Standard to "ISO C++17 Standard" in Visual Studio.
set(CMAKE_CXX_STANDARD 17)

if(MINGW)
    # This might not only be required for mingw
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libstdc++")
endif()

find_package(OpenGL)

add_subdirectory(ThirdParty)
add_subdirectory(src/Engine)
add_subdirectory(src/Abyss)
add_subdirectory(src/Apocalypse)
add_subdirectory(src/Armageddon)
add_subdirectory(src/Catacomb3D)

if(WIN32)
    if (MSVC)
        # On Visual Studio for Windows, the SDL3 development package is fetched from Github.
        include(FetchContent)

        if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
            cmake_policy(SET CMP0135 NEW)
        endif()

        FetchContent_Declare(
            sdldev
            URL      https://github.com/libsdl-org/SDL/releases/download/release-3.4.10/SDL3-devel-3.4.10-VC.zip
            URL_HASH SHA256=e2b336b10b037934af98308027410732ef7b22f2c6697d58092aa1c209fae7d7
        )

        FetchContent_MakeAvailable(sdldev)
        set(SDL3_PATH ${sdldev_SOURCE_DIR})
        find_package(SDL3 REQUIRED CONFIG PATHS ${sdldev_SOURCE_DIR}/cmake NO_DEFAULT_PATH)
    else()
        # On MinGW, uses the system library of SDL3
        find_package(SDL3 REQUIRED)
    endif()

    target_link_libraries( CatacombGL_Engine
        "shlwapi"
        SDL3::SDL3
    )
    target_link_libraries( CatacombGL_ThirdParty
        "shlwapi"
        SDL3::SDL3
    )

else()
    # On Linux, it is assumed that the SDL3 development package is already installed.
    find_package(SDL3 REQUIRED)

endif()

add_executable(CatacombGL)

function(link_each_target_to_libs libs targets)
    foreach(target ${targets})
        target_link_libraries( ${target}
            ${libs}
        )
    endforeach()
endfunction()

set( CATACOMBGL_TARGETS
    "CatacombGL_Engine"
    "CatacombGL_Abyss"
    "CatacombGL_Armageddon"
    "CatacombGL_Apocalypse"
    "CatacombGL_Catacomb3D"
    "CatacombGL_ThirdParty"
)

link_each_target_to_libs( SDL3::SDL3
    "${CATACOMBGL_TARGETS}"
)

add_subdirectory(src/System)

if(WIN32)
    target_sources( CatacombGL
    PUBLIC
        ${CMAKE_SOURCE_DIR}/res/win/CatacombGL.ico
        ${CMAKE_SOURCE_DIR}/res/win/CatacombGL.rc
        ${CMAKE_SOURCE_DIR}/res/win/resource.h
    )

    set_source_files_properties(
        ${CMAKE_SOURCE_DIR}/res/win/CatacombGL.rc
        PROPERTIES LANGUAGE RC
    )

endif()

target_link_libraries( CatacombGL
    CatacombGL_ThirdParty
    CatacombGL_Engine
    CatacombGL_Abyss
    CatacombGL_Armageddon
    CatacombGL_Apocalypse
    CatacombGL_Catacomb3D
    OpenGL::GL
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTS)
    add_subdirectory(src/Test)
endif()

include(CPack)
if(WIN32)
    set(CMAKE_INSTALL_BINDIR ".")
    set(CMAKE_INSTALL_LIBDIR ".")
    install(FILES $<TARGET_RUNTIME_DLLS:CatacombGL> TYPE LIB)
endif()
install(TARGETS CatacombGL RUNTIME)

