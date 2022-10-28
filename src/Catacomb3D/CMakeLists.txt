cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Catacomb3D LANGUAGES CXX)

target_sources( CatacombGL_Objects
PRIVATE
    AudioRepositoryCatacomb3D.h
    Catacomb3DMenu.cpp
    Catacomb3DMenu.h
    DecorateAll.h
    DecorateBonus.h
    DecorateMisc.h
    DecorateMonsters.h
    DecorateProjectiles.h
    EgaGraphCatacomb3D.h
    GameCatacomb3D.cpp
    GameCatacomb3D.h
    GameDetectionCatacomb3D.h
    GameMapsCatacomb3D.h
    GuiElementBindKeyCat3D.cpp
    GuiElementBindKeyCat3D.h
    GuiElementBoolSelectionCat3D.cpp
    GuiElementBoolSelectionCat3D.h
    GuiElementButtonCat3D.cpp
    GuiElementButtonCat3D.h
    GuiElementEnumSelectionCat3D.cpp
    GuiElementEnumSelectionCat3D.h
    GuiElementEnumSelectionCat3DRadio.cpp
    GuiElementEnumSelectionCat3DRadio.h
    GuiElementIntSelectionCat3D.cpp
    GuiElementIntSelectionCat3D.h
    GuiElementSaveSlotEditableCat3D.cpp
    GuiElementSaveSlotEditableCat3D.h
    GuiElementSaveSlotStaticCat3D.cpp
    GuiElementSaveSlotStaticCat3D.h
    GuiPageFrameCat3D.cpp
    GuiPageFrameCat3D.h
    IntroViewCatacomb3D.cpp
    IntroViewCatacomb3D.h
    SavedGameConverterCatacomb3D.cpp
    SavedGameConverterCatacomb3D.h
    SavedInGameInDosFormatConfigCatacomb3D.h
    SkullNBones.cpp
    SkullNBones.h
)
