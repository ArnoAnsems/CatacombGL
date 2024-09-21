cmake_minimum_required(VERSION 3.16)
project(CatacombGL_ThirdParty LANGUAGES CXX C)

include_directories(ReflectionHLE)

add_library( CatacombGL_ThirdParty OBJECT
    ReflectionHLE/backend/audio/be_audio_digi.c
    ReflectionHLE/backend/audio/be_audio_init.c
    ReflectionHLE/backend/audio/be_audio_main_thread.c
    ReflectionHLE/backend/audio/be_audio_main_thread.h
    ReflectionHLE/backend/audio/be_audio_mixer.c
    ReflectionHLE/backend/audio/be_audio_mixer.h
    ReflectionHLE/backend/audio/be_audio_opl.c
    ReflectionHLE/backend/audio/be_audio_pcspkr.c
    ReflectionHLE/backend/audio/be_audio_private.h
    ReflectionHLE/backend/audio/be_audio_resampling.c
    ReflectionHLE/backend/audio/be_audio_resampling.h
    ReflectionHLE/backend/audio/be_audio_sdl.c
    ReflectionHLE/backend/cfg/be_cfg.c
    ReflectionHLE/backend/cfg/be_cfg_sdl.c
    ReflectionHLE/backend/threading/be_atomic.h
    ReflectionHLE/backend/threading/be_atomic_sdl.h
    ReflectionHLE/backend/threading/be_mutex.h
    ReflectionHLE/backend/threading/be_mutex_sdl.h
    ReflectionHLE/backend/timing/be_timing.c
    ReflectionHLE/backend/timing/be_timing.h
    ReflectionHLE/backend/timing/be_timing_callback.c
    ReflectionHLE/nukedopl/opl3.c
    ReflectionHLE/nukedopl/opl3.h
    ReflectionHLE/be_cross.c
    ReflectionHLE/be_cross.h
    ReflectionHLE/be_sound_device_flags.h
    ReflectionHLE/be_st.h
    ReflectionHLE/be_st_cfg.h
    ReflectionHLE/be_st_sdl_private.h
    ReflectionHLE/id_sd.cpp
    ReflectionHLE/id_sd.h
    ReflectionHLE/refkeen_config.h
)
