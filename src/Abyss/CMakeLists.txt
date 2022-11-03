cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Abyss LANGUAGES CXX)

add_library( CatacombGL_Abyss OBJECT
    AudioRepositoryAbyss.h
    DecorateAll.h
    DecorateBonus.h
    DecorateMisc.h
    DecorateMonsters.h
    DecorateProjectiles.h
    EgaGraphAbyss.h
    GameAbyss.cpp
    GameAbyss.h
    GameDetectionAbyss.h
    GameMapsAbyss.h
    IntroViewAbyss.cpp
    IntroViewAbyss.h
    SavedGameConverterAbyss.cpp
    SavedGameConverterAbyss.h
    SavedGameInDosFormatConfigAbyss.h
)
