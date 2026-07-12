# Introduction
CatacombGL is a source port of Catacomb 3D and the Catacomb Adventure series. These games were originally developed for MS-DOS. Catacomb 3D: The Descent was developed by Id Software in 1991. The Catacomb Adventure series was developed by Softdisk Publishing in 1992-1993 and consists of The Catacomb Abyss, The Catacomb Armageddon and The Catacomb Apocalypse. The original games visualized their 3D graphics in 16 colors and a 320x200 pixels view window. The main goal of the CatacombGL source port is to utilize OpenGL to improve the visual presentation of the Catacomb 3D games.

Main features: 
* Runs native on Windows 7/8/10/11 and Linux. 
* Hardware accelerated 3D rendering via OpenGL, supporting widescreen resolutions, a customizable field of view and an uncapped frame rate. 
* Player, projectile and monster movement with floating point precision.
* Configurable mouse and keyboard controls.
* Supports all four Catacomb 3D games as included in the Catacombs Pack from GOG.com.
* Supports The Catacomb Abyss version 1.13 (shareware).

Current limitations as of version 0.5.7: 
* No support for game controllers. 
* No backwards compatibility with saved games from the original DOS game. 

# How to install
CatacombGL does not run standalone, but requires the original game data (levels, pictures, sounds, etc) to be present on the system. The Catacomb Abyss game data can be obtained legitimately and free-of-charge by downloading the 1.13 shareware version. Various websites offer the shareware version for download, see for example https://archive.org/details/TheCatacombAbyss. Alternatively, the game data can also be obtained by buying the Catacombs Pack from gog.com, see https://www.gog.com/game/catacombs_pack. The differences between the shareware and commercial versions are purely cosmetic. The Catacombs Pack is required in order to run Catacomb 3D (The Descent), The Catacomb Armageddon or The Catacomb Apocalypse.
The CatacombGL.exe (Windows) or CatacombGL.a (Linux) can be run from any location. When started, a selection screen with an overview of all the supported games is shown. In case the Catacombs Pack was installed via the GOG installer (Windows), the game data will be auto-detected. Otherwise, it is possible to manually browse to the folder which contains either the Catacombs Pack or the shareware version.

# How to build on Linux
With the following steps, CatacombGL was built on Ubuntu 26.04 LTS:
* sudo apt-get update
* sudo apt-get install build-essential cmake cmake-data
* sudo add-apt-repository universe
* sudo apt-get install libsdl3-dev

Ccmake gui is optional (sudo apt-get install ccmake-curses-gui) <br />
<br />
General Linux compilation instruction:
```
git clone https://github.com/ArnoAnsems/CatacombGL
cd CatacombGL
mkdir build
cd build
cmake ..
make -j3
 ```
 
With the following steps, CatacombGL was built on RaspberryPi:
* sudo apt-get install git build-essential cmake-curses-gui libsdl3-dev
* git clone "https://github.com/arnoansems/catacombgl"
* ccmake <path to CatacombGL source>
* Configure and generate the build via the CMake GUI
* make -j3

# Controls
The keyboard and mouse controls for moving, shooting, etc. can be customized via the in-game menu. The following keys are reserved and cannot be customized: 
* ESC - open/close the menu 
* Function keys - various shortcuts, such as F3 for saving the game and F4 for restoring the game
* Numerical keys - read scrolls
* Backspace - cheat codes in Armageddon and Apocalypse
* Tilde (~) - show log

# Command line parameters
* --descent Skips the game selection screen and starts Catacomb 3-D THe Descent.
* --abyss Skips the game selection screen and starts Catacomb Abyss (registered version).
* --armageddon Skips the game selection screen and starts Catacomb Armageddon.
* --apocalypse Skips the game selection screen and starts Catacomb Apocalypse.
* --abyss_sw13 Skips the game selection screen and starts Catacomb Abyss shareware v1.13
* --ini <filename> Loads the ini file from the specified location.
* --log <filename> Stores the log file into the specified location.

# Additional notes
* The original Catacomb 3D allowed the player to reconfigure movement and action keys, but CatacombGL ignores those settings. Instead, CatacombGL has its own keyboard/mouse configuration, which is shared across all four games.
* High scores achieved in Catacomb 3D are not stored in the GOG folder, to avoid file access issues. Instead, CatacombGL stores the high scores in \%appdata%\CatacombGL\CONFIG.C3D (Windows) or ~/.config/CatacombGL/CONFIG.C3D (Linux).
* To aid in navigating through narrow corridors, CatacombGL allows the player to slide along walls in Catacomb 3D.
* The automap can be opened via a configurable key, with the default being the 'O' key. The automap can be visualized in four different styles, which is configurable via the Video menu. The "original" style is based on the Catacomb 3D debug automap. Pressing the Ctrl key in the original style automap will cycle it through several different view modes, just like in the original Catacomb 3D. Only locations that were visited by the player are shown on the automap. The automap can also be opened via a cheat code, which is F10+O in Catacomb 3D and Abyss, or Backspace+O in Armageddon and Apocalypse. When this cheat code is used, then all locations are shown immediately.

# License
CatacombGL is licensed under the GNU GPLv3 license. See gpl3.txt for details. 
CatacombGL would not have been possible without reusing code that was generously made available by various other projects, namely:
* Catacomb 3D, developed by Id Software. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/Catacomb3D). 
* The Catacomb Abyss, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombAbyss). 
* The Catacomb Armageddon, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombArmageddon). 
* The Catacomb Apocalypse, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombApocalypse). 
* ReflectionHLE, developed by NY00123, released under the GPLv2 license, see gpl2.txt (https://github.com/ReflectionHLE/ReflectionHLE).
* Nuked-OPL3-fast, developed by Tony Gies, based on the Nuked OPL3 emulator by Nuke.YKT, released under the LGPLv2.1 license, see lgpl2.1.txt (https://github.com/tgies/Nuked-OPL3-fast).
* Simple DirectMedia Layer, developed by Sam Lantinga. See SDL3.txt for license details. (http://www.libsdl.org/)
* GoogleTest, developed by the GoogleTest team. See GoogleTest.txt for license details. (https://github.com/google/googletest). 

# Tools
The following software tools have proven to be very helpful in the development of CatacombGL.
* Microsoft Visual Studio 2019 (https://visualstudio.microsoft.com/downloads/)
* Wolf3D Data Compiler, by Adam Biser (http://winwolf3d.dugtrio17.com).
* Dr. Memory, Memory Debugger for Windows, Linux, and Mac (https://drmemory.org/)

# Credits
CatacombGL is developed by Arno Ansems.
Special thanks goes to:
* Id Software and Softdisk Publishing for developing the Catacomb 3D games.
* Flatrock Software for releasing the source code of the Catacomb 3D games.
* GOG.com and catacomb.games for providing a platform to obtain the Catacomb 3D games.
* Roland Ansems for playtesting every level and reporting various bugs.
* NY00123 for developing the ReflectionHLE source port. The part of ReflectionHLE that handles music and sound effects is reused in CatacombGL.
* GoGoOtaku for porting CatacombGL to Linux and CMake.
