cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Abyss LANGUAGES CXX)

add_library( CatacombGL_Abyss OBJECT
    AudioRepositoryAbyss.h
    DecorateAll.h
    DecorateBonus.h
    DecorateMisc.h
    DecorateMonsters.h
    DecorateProjectiles.h
    DecorateV112.cpp
    DecorateV112.h
    EgaGraphAbyss.h
    GameAbyss.cpp
    GameAbyss.h
    GameDetectionAbyss.h
    GameMapsAbyss.h
    GameMapsAbyssV112.cpp
    GameMapsAbyssV112.h
    IntroViewAbyss.cpp
    IntroViewAbyss.h
    SavedGameConverterAbyss.cpp
    SavedGameConverterAbyss.h
    SavedGameInDosFormatConfigAbyss.h
)
