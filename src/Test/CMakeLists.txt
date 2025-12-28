cmake_minimum_required(VERSION 3.16)
project(CatacombGL_Test LANGUAGES CXX)

enable_testing()

find_package(GTest)

if(NOT GTest_FOUND)
    include(FetchContent)

    FetchContent_Declare(
      googletest
      GIT_REPOSITORY    https://github.com/google/googletest.git
      GIT_TAG           58d77fa8070e8cec2dc1ed015d66b454c8d78850 # release-1.12.1
    )

    set(INSTALL_GTEST OFF)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable( CatacombGL_Test
    CommandLineParser_Test.cpp
    ConsoleVariableBool_Test.cpp
    ConsoleVariableEnum_Test.cpp
    ConsoleVariableInt_Test.cpp
    ConsoleVariableString_Test.cpp
    FramesCounter_Test.cpp
    GameAbyss_Test.cpp
    GameApocalypse_Test.cpp
    GameArmageddon_Test.cpp
    GameCatacomb3D_Test.cpp
    Gui_TestBase.cpp
    Gui_TestBase.h
    GuiElementEditText_Test.cpp
    GuiElementList_Test.cpp
    GuiMenu_Test.cpp
    HelpPages_Test.cpp
    LevelLocationNames_Test.cpp
    RendererStub.cpp
    SavedGameConverterAbyss_Test.cpp
    SavedGameConverterApocalypse_Test.cpp
    SavedGameConverterArmageddon_Test.cpp
    SavedGameConverterCatacomb3D_Test.cpp
    SavedGameInDosFormatLoader_Test.cpp
    SavedGameInDosFormat_Data.h
    SavedGameInDosFormat_Test.cpp
    SavedGamesInDosFormat_Test.cpp
    StringUtility_Test.cpp
    TextureAtlas_Test.cpp
    ViewPorts_Test.cpp
    main.cpp
)

if(WIN32)
    add_custom_command(TARGET CatacombGL_Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:CatacombGL_Test>
        $<TARGET_FILE_DIR:CatacombGL_Test>
        COMMAND_EXPAND_LISTS
    )
endif()

target_link_libraries( CatacombGL_Test
PRIVATE
    CatacombGL_ThirdParty
    CatacombGL_Engine
    CatacombGL_Abyss
    CatacombGL_Armageddon
    CatacombGL_Apocalypse
    CatacombGL_Catacomb3D
    GTest::gtest_main
    GTest::gtest
)

include(GoogleTest)
gtest_discover_tests( CatacombGL_Test
    DISCOVERY_TIMEOUT 3600
)

