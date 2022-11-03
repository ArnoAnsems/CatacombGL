cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Apocalypse LANGUAGES CXX)

add_library( CatacombGL_Apocalypse OBJECT
    AudioRepositoryApocalypse.h
    DecorateAll.h
    DecorateBonus.h
    DecorateMisc.h
    DecorateMonsters.h
    DecorateProjectiles.h
    EgaGraphApocalypse.h
    GameApocalypse.cpp
    GameApocalypse.h
    GameDetectionApocalypse.h
    GameMapsApocalypse.h
    IntroViewApocalypse.cpp
    IntroViewApocalypse.h
    SavedGameConverterApocalypse.cpp
    SavedGameConverterApocalypse.h
    SavedGameInDosFormatConfigApocalypse.h
)
