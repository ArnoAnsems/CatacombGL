cmake_minimum_required(VERSION 3.16)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(CatacombGL LANGUAGES CXX VERSION 0.5.3)

option(BUILD_TESTS "Build Tests" OFF)

set(CMAKE_CXX_STANDARD 17)

if(MINGW)
    # This might not only be required for mingw
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libstdc++")
endif()

find_package(OpenGL)

add_library( CatacombGL_Objects OBJECT
    ThirdParty/RefKeen/be_st.h
    ThirdParty/RefKeen/be_st_sdl_audio_timer.cpp
    ThirdParty/RefKeen/id_sd.cpp
    ThirdParty/RefKeen/id_sd.h
    ThirdParty/opl/dbopl.cpp
    ThirdParty/opl/dbopl.h
)

add_subdirectory(src/Engine)
add_subdirectory(src/Abyss)
add_subdirectory(src/Apocalypse)
add_subdirectory(src/Armageddon)
add_subdirectory(src/Catacomb3D)

if(WIN32)
    include(FetchContent)

    if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
		cmake_policy(SET CMP0135 NEW)
	endif()

    FetchContent_Declare(
        sdldev
        URL     https://github.com/libsdl-org/SDL/releases/download/release-2.24.1/SDL2-devel-2.24.1-VC.zip
        URL_MD5 63dd0eca2c6715c8170b1cd3e84017fc
    )

    FetchContent_MakeAvailable(sdldev)
    set(SDL2_PATH ${sdldev_SOURCE_DIR})
    find_package(SDL2 REQUIRED CONFIG PATHS ${sdldev_SOURCE_DIR}/cmake NO_DEFAULT_PATH)

    target_link_libraries( CatacombGL_Objects
        "shlwapi"
        SDL2::SDL2main
    )

else()
    find_package(SDL2 REQUIRED)

endif()

if(SDL2_VERSION VERSION_GREATER_EQUAL "2.0.12")
    target_link_libraries( CatacombGL_Objects
        SDL2::SDL2
    )

else()
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries( CatacombGL_Objects
        ${SDL2_LIBRARIES}
    )

endif()

add_executable(CatacombGL)
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
    CatacombGL_Objects
    OpenGL::GL
    OpenGL::GLU
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

