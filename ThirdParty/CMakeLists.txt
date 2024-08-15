cmake_minimum_required(VERSION 3.16)
project(CatacombGL_ThirdParty LANGUAGES CXX)

add_library( CatacombGL_ThirdParty OBJECT
    RefKeen/be_st.h
    RefKeen/be_st_sdl_audio_timer.cpp
    RefKeen/id_sd.cpp
    RefKeen/id_sd.h
    opl/dbopl.cpp
    opl/dbopl.h
)
