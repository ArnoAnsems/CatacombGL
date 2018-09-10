# Introduction
CatacombGL is a Windows source port of the DOS game The Catacomb Abyss. The Catacomb Abyss was originally developed in 1992 by Softdisk Publishing. It is a first person shooter with a fantasy theme. The original game visualized its 3D graphics in 16 colors and a 320x200 pixels view window. The main goal of the CatacombGL source port is to utilize OpenGL to improve the visual presentation of The Catacomb Abyss. This source port is currently a work-in-progress and as such in the alpha development phase. Some functionality has yet to be implemented and existing functionality was only tested on a very limited set of test systems. However, with the current version it is possible to play through the entire game, as all levels, monsters and bonus items are present and functional. 

Main features: 
* Runs native on Windows 7/8/10. 
* Hardware accelerated 3D rendering via OpenGL, supporting widescreen resolutions, a customizable field of view and an uncapped frame rate. 
* Player, projectile and monster movement with floating point precision. 
* Supports The Catacomb Abyss version 1.13 (shareware) and version 1.24 (as included in the Catacombs Pack from gog.com). 
* WASD and mouselook.

Current limitations as of version 0.1.0: 
* Only windowed mode supported, although it is possible to cover the whole desktop by maximizing the window. 
* No support for game controllers. 
* No backwards compatibility with saved games from the original DOS game. 
* Only Adlib sound card emulation; no PC speaker emulation. 
* No "demo" functionality (preview slideshow of Catacomb Armageddon and Apocalypse in shareware version). 
* No help pages. 
* No fade-in effect when entering a level. 

# How to install
CatacombGL does not run standalone, but requires the original The Catacomb Abyss game data (levels, pictures, sounds, etc) to be present on the system. The game data can be obtained legitimately and free-of-charge by downloading the 1.13 shareware version of The Catacomb Abyss. Various websites offer the shareware version for download, see for example https://archive.org/details/TheCatacombAbyss. Alternatively, the game data can also be obtained by buying the Catacombs Pack from gog.com, see https://www.gog.com/game/catacombs_pack. The differences between the shareware and commercial versions are purely cosmetic. 

# Controls
The keyboard and mouse controls for moving, shooting, etc. can be customized via the in-game menu. The following keys are reserved and cannot be customized: 
ESC - open/close the menu 
Function keys - various shortcuts, such as F3 for saving the game and F4 for restoring the game
Numerical keys - read scrolls

# License
CatacombGL is developed by Arno Ansems and licensed under the GNU GPLv3 license. See gpl3.txt for details. 
CatacombGL would not have been possible without reusing code that was generously made available by various other projects, namely: 
* The Catacomb Abyss, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombAbyss). 
* The Catacomb Apocalypse, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombApocalypse). 
* Reflection Keen, developed by NY00123, released under the GPLv2 license, see gpl2.txt (https://github.com/NY00123/refkeen). 
* DOSBox OPL emulator, developed by the DOSBox Team, released under the GPLv2 license, see gpl2.txt (https://github.com/NY00123/refkeen/tree/master/src/opl).
* Simple DirectMedia Layer, developed by Sam Lantinga. See SDL2.txt for license details. (http://www.libsdl.org/)
* GoogleTest, developed by the GoogleTest team. See GoogleTest.txt for license details. (https://github.com/google/googletest). 

# Roadmap
On the short term, the plan is to implement some of the missing features in The Catacomb Abyss, such as the help screens and a proper full screen mode. On the long-term roadmap is support for The Catacomb Armageddon, eventually followed by support for all the games in the Catacomb 3D series. 
