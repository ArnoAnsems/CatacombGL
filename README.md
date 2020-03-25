# Introduction
CatacombGL is a Windows source port of Catacomb 3D and the Catacomb Adventure series. These games were originally developed for MS-DOS. Catacomb 3D: The Descent was developed by Id Software in 1991. The Catacomb Adventure series was developed by Softdisk Publishing in 1992-1993 and consists of The Catacomb Abyss, The Catacomb Armageddon and The Catacomb Apocalypse. The original games visualized their 3D graphics in 16 colors and a 320x200 pixels view window. The main goal of the CatacombGL source port is to utilize OpenGL to improve the visual presentation of the Catacomb 3D games.

Main features: 
* Runs native on Windows 7/8/10. 
* Hardware accelerated 3D rendering via OpenGL, supporting widescreen resolutions, a customizable field of view and an uncapped frame rate. 
* Player, projectile and monster movement with floating point precision.
* Configurable mouse and keyboard controls.
* Supports all four Catacomb 3D games as included in the Catacombs Pack from GOG.com.
* Supports The Catacomb Abyss version 1.13 (shareware).

Current limitations as of version 0.4.3: 
* No support for game controllers. 
* No backwards compatibility with saved games from the original DOS game. 
* No "demo" functionality (preview slideshow of Catacomb Armageddon and Apocalypse in shareware version).

# How to install
CatacombGL does not run standalone, but requires the original game data (levels, pictures, sounds, etc) to be present on the system. The Catacomb Abyss game data can be obtained legitimately and free-of-charge by downloading the 1.13 shareware version. Various websites offer the shareware version for download, see for example https://archive.org/details/TheCatacombAbyss. Alternatively, the game data can also be obtained by buying the Catacombs Pack from gog.com, see https://www.gog.com/game/catacombs_pack. The differences between the shareware and commercial versions are purely cosmetic. The Catacombs Pack is required in order to run Catacomb 3D (The Descent), The Catacomb Armageddon or The Catacomb Apocalypse.
The CatacombGL.exe can be run from any location. When started, a selection screen with an overview of all the supported games is shown. In case the Catacombs Pack was installed via the GOG installer, the game data will be auto-detected. Otherwise, it is possible to manually browse to the folder which contains either the Catacombs Pack or the shareware version.

# Controls
The keyboard and mouse controls for moving, shooting, etc. can be customized via the in-game menu. The following keys are reserved and cannot be customized: 
* ESC - open/close the menu 
* Function keys - various shortcuts, such as F3 for saving the game and F4 for restoring the game
* Numerical keys - read scrolls
* Backspace - cheat codes in Armageddon and Apocalypse
* Tilde (~) - show log

# Additional notes
* The original Catacomb 3D allowed the player to reconfigure movement and action keys, but CatacombGL ignores those settings. Instead, CatacombGL has its own keyboard/mouse configuration, which is shared across all four games.
* High scores achieved in Catacomb 3D are not stored in the GOG folder, to avoid file access issues. Instead, CatacombGL stores the high scores in \%appdata%\CatacombGL\CONFIG.C3D.
* To aid in navigating through narrow corridors, CatacombGL allows the player to slide along walls in Catacomb 3D. 

# License
CatacombGL is licensed under the GNU GPLv3 license. See gpl3.txt for details. 
CatacombGL would not have been possible without reusing code that was generously made available by various other projects, namely:
* Catacomb 3D, developed by Id Software. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/Catacomb3D). 
* The Catacomb Abyss, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombAbyss). 
* The Catacomb Armageddon, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombArmageddon). 
* The Catacomb Apocalypse, developed by Softdisk Publishing. The source code was published by Flatrock Software under the GNU GPLv2 license, see gpl2.txt (https://github.com/CatacombGames/CatacombApocalypse). 
* Reflection Keen, developed by NY00123, released under the GPLv2 license, see gpl2.txt (https://github.com/NY00123/refkeen). 
* DOSBox OPL emulator, developed by the DOSBox Team, released under the GPLv2 license, see gpl2.txt (https://github.com/NY00123/refkeen/tree/master/src/opl).
* Simple DirectMedia Layer, developed by Sam Lantinga. See SDL2.txt for license details. (http://www.libsdl.org/)
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
* NY00123 for developing the Reflection Keen source port. The part of Reflection Keen that handles music and sound effects is reused in CatacombGL.

# History
* Version 0.4.3 (2020-03-25) New in this release:
  * Improved the performance of the OpenGL renderer.
  * Added an "Extended" mode for the "Show frame rate" setting. When set, additional information such as the name of the graphics card is displayed.
  * Added a "Loading Control Panel" animation to Catacomb 3D. Suggested by Dinoaur.
  * Fix: support for OpenGL 2.x hardware. Suggested by InDOOMnesia.
  * Fix: melee enemies in Catacomb 3D will no longer attempt to attack from too far away. Reported by Dinoaur.
  * Fix: added the fizzle fade effect when opening the title screen in Catacomb 3D. Reported by Dinoaur.
  * Fix: prevent the wizard's hand from bopping while shooting magick missiles repeatedly. Reported by Dinoaur.

# History
* Version 0.4.2 (2020-02-20) New in this release:
  * Added the "fizzle fade" effect when entering a level or travelling through a magic gate. Based on code from Wolf4SDL.
  * Fix: corrected the aspect ratio of the 3D view to match with the original 4:3 monitors.
  * Fix: do not play a sound while browsing the menu in Catacomb 3D. Reported by Dinoaur.
  * Fix: moved the hand in Catacomb 3D to the center. Reported by Dinoaur.
  * Fix: do not show popup messages about keys when interacting with doors in Catacomb 3D. Reported by Dinoaur.
  * Fix: do not play music during the transition to a new level. Reported by Dinoaur.
  * Fix: pause the game when opening the menu during level transition. Reported by @pagb666.
  * Fix: display framerate in the introduction screens.

* Version 0.4.1 (2020-01-11) New in this release:
  * Added option for auto fire. Thanks to Master O for the suggestion. When auto fire is enabled, keeping the fire button pressed will automatically fire four projectiles per second. This is not part of the original gameplay and therefore disabled by default. But it might help in reducing stress on the finger by preventing excessive clicking.
  * Fix: mouse sensitivity issues, as the mouse speed was affected by frame rate and way too sensitive at quick turns. Reported by @pagb666 and Jello.
  * Fix: mouse buttons X1 and X2 can now be assigned to actions. Thanks to Jello for reporting the issue and playtesting the fix.
  * Fix: part of the hand graphic in Catacomb 3D could be visible underneath the statusbar in windowed mode.

* Version 0.4.0 (2020-01-02) New in this release:
  * Support for Catacomb 3D: The Descent! All levels and monsters are present. Requires the Catacombs Pack.
  * Added PC Speaker emulation, which was realized by integrating the sound code of the Reflection Keen source code.
  * Fix: crash on some systems due to accidently creating the OpenGL context twice. Reported by Technopeasant.
  * Fix: changed the tic rate from 60 to 70 Hz. The DEF.H source file in Abyss seems to suggest a tic rate of 60 Hz (see MAXFREEZETIME), but in reality all games run at 70 tics per second.
  * Fix: look for the GOG Catacombs Pack at the new registry location (\GOG.com\GAMES\1207659189).
  * Fix: the time freeze effect now counts down from 100.
  * Fix: there were small rendering glitches in the frame that surrounds the menu.
  * Fix: do not allow cheat codes when the player is dead.
  * Fix: while a name is entered for a new saved game slot, do not allow to switch to another slot.
  * Fix: it is no longer allowed to save a game under an empty name.

* Version 0.3.0 (2019-08-23) New in this release:
  * Support for the Catacomb Apocalypse! All levels and monsters are present. Requires the Catacombs Pack.
  * Lowered the minimum mouse sensitivity, to allow for slower mouse movement. Suggested by DoomN00b.
  * Fix: the level warp dialog now also accepts input from the numerical keypad.
  * Fix: various memory leaks were resolved with help from the Dr. Memory tool.
  * Fix: some keyboard keys could not be mapped in the controls menu. Reported by DoomN00b.
  * Fix: non-ASCII characters were not shown, as reported by DoomN00b. This was a limitation of the font from the original games. The font is now extended with many non-ASCII characters, like for example 'æ' and 'ß'.

* Version 0.2.1 (2019-05-14) New in this release:
  * The game now creates a log file at %appdata%\CatacombGL\CatacombGL_log.txt. The log file can also be viewed in-game via the tilde (~) key.
  * Fix: there was a buffer overflow during initialization of the location names, which could randomly crash the game shortly after starting it.
  * Fix: the hand of the player was not shown in the Catacomb Abyss Shareware.
  * Fix: the FOV could not be adjusted via the arrow keys in the video menu.
  * Fix: a warning message is now provided in case the name of an exising save game file is typed in the save game menu.
  * Fix: the Armageddon game files were not detected when CatacombGL.exe was residing in the same folder.
  * Fix: picked up keys were not added to the inventory in case of low frame rates (< 10 fps)
  * Fix: the bat was able to fly through the player, but gets properly clipped now.

* Version 0.2.0 (2019-03-31) New in this release:
  * Support for the Catacomb Armageddon! All levels and monsters are present. Requires the Catacombs Pack.
  * Game selection screen with the option to manually browse to a folder with game data. Suggested by Master O.
  * Support for x64 builds. Implemented by Arsen "Honeybunch" Tufankjian.
  * Support for full screen and borderless windowed mode.
  * In game help screens. Accessible via the F1 key.
  * Customizable player turn speed. Suggested by doomjedi.
  * Customizable player run button. By default, the run button is not bound to any key, since the original games did not support running. Suggested by doomjedi and Tricob.
  * Fix: enemies with a melee attack now randomly show the attack animation even when out of range.
  * Fix: fireballs from the player can now pass through keys, gems and scrolls.
  * Fix: fireballs from the player now pass through enemies during the 'rise' animations.
  * Fix: hour glass (freeze time powerup) is now destructable.
  * Fix: speed of water monsters is set to 2200 when above water and 1200 when submerged.
  * Fix: monsters with projectiles aim better when standing close to the player.
  * Fix: projectiles from monsters can no longer cause walls to explode.
  * Fix: nemesis could fail to drop the red key when many projectiles were active after using the freeze time powerup.
  * Fix: in the restore game or save game menu, scrolling did not work properly when the list of saved games did not fill up the whole page.
  
* Version 0.1.0 (2018-09-10) Initial release. This source port is currently a work-in-progress and as such in the alpha development phase. Some functionality has yet to be implemented and existing functionality was only tested on a very limited set of test systems. However, with the current version it is possible to play through the entire game, as all levels, monsters and bonus items are present and functional.
