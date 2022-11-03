cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Armageddon LANGUAGES CXX)

add_library( CatacombGL_Armageddon OBJECT
    AudioRepositoryArmageddon.h
    DecorateAll.h
    DecorateBonus.h
    DecorateMisc.h
    DecorateMonsters.h
    DecorateProjectiles.h
    EgaGraphArmageddon.h
    GameArmageddon.cpp
    GameArmageddon.h
    GameDetectionArmageddon.h
    GameMapsArmageddon.h
    IntroViewArmageddon.cpp
    IntroViewArmageddon.h
    SavedGameConverterArmageddon.cpp
    SavedGameConverterArmageddon.h
    SavesGameInDosFormatConfigArmageddon.h
)
