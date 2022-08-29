cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Cross LANGUAGES CXX)

target_sources( CatacombGL
PUBLIC
    CatacombGL.cpp
    Finder.cpp
    RendererOpenGL.cpp
    SystemSDL.h
    WindowSDL.h
    Finder.h
    RendererOpenGL.h
    SystemSDL.cpp
    WindowSDL.cpp
)

