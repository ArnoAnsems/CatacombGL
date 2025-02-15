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
With the following steps, CatacombGL was built on Ubuntu 24.04 LTS:
* sudo apt-get update
* sudo apt-get install build-essential cmake cmake-data
* sudo add-apt-repository universe
* sudo apt-get install libsdl2-dev

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
* sudo apt-get install git build-essential cmake-curses-gui libsdl2-dev
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
* GoGoOtaku for porting CatacombGL to Linux and CMake.

# History
* Version 0.5.7 (2025-01-04) New in this release:
  * Fix: prevent crash when taking certain exit portals, such as the exit portal that takes the player from The Warrens (level 14) to Access Floor (level 9) in Catacomb 3D. Reported by DEA1HYT.
  * Fix: in the folder selection window, hide inaccessible folders to prevent a crash when selecting such a folder.

* Version 0.5.6 (2024-11-27) New in this release:
  * Added an option to place the camera position slightly behind the player, as seen in the original DOS games. The default camera position is still centered on the player. Suggested by N00123, Blzut3 and WhiteMagicRaven.
  * Added configuration variables WindowedScreenWidth and WindowedScreenHeight, which can be used to set the initial screen resolution in windowed mode. These variables have to be set via the configuration file, since they are not included in the menu. The default is 800 x 600. Suggested by The Kinsie.
  * Integrated the latest audio backend from ReflectionHLE.
  * Fix: prevent music playback from interfering with sound effects in the Catacomb Adventure series. Reported by taufan99. Fix suggested by NY00123.
  * Fix: switch off the music when opening the menu via the F2, F3 or F4 keyboard keys in the Catacomb Adventure series. Reported by NY00123.
  * Fix: allow for two default keyboard keys to be bound to a single action.

* Version 0.5.5 (2024-08-21) New in this release:
  * Support for command line parameters was implemented by wcapes. See the readme file for a list of commands.
  * The sticky walls in Catacomb 3-D are now configurable. Suggested by @ivfall@bitbang.social on Mastodon.
  * Music playback can now be enabled in the Catacomb Adventure Series. The music track is "Too Hot to Handle" by Bobby Prince, which is the same track as used in Catacomb 3-D. The track was already present in the game data of the Catacomb Adventure Series, but not used until now. Suggested by taufan99.
  * The Windows binaries are now built with Microsoft Visual Studio 2022 Community Edition.
  * Upgraded SDL2 to version 2.30.6.
  * The instructions for compiling CatacombGL on Linux using CMake were simplified by arrowgent.
  * Fix: Linux compilation errors were fixed by SymenMulders and bdnugget.
  * Fix: the automap player marker now has a contrasting color when the floor is white or yellow. Reported by youtuber MadJak91.
  * Fix: in the Catacomb Adventure Series, as was seen in the original DOS games, the damage of player shots is round off in novice mode. Reported by youtuber MadJak91.
  * Fix: prevent key bindings from being overwritten by the default values. Reported by vermian.

* Version 0.5.4 (2022-12-30) New in this release:
  * Support for Linux was added through the hard work of GoGoOtaku, who in the process also did the migration to CMake. Additional testing was performed by NY00123 (Ubuntu), AlyxxTheRat (Linux Mint) and Scandy (RaspberryPi). The functionality to auto-detect the default location of the GOG Catacombs Pack in the Wine folder was suggested by Stuart Axon.
  * Fix: close cheat dialog when ESC is pressed. Reported by NY00123.
  * Fix: do not split help screen with black bar. Reported by NY00123.
  * Fix: border flash color should be magenta in Catacomb 3-D. Reported by NY00123.

* Version 0.5.3 (2021-08-28) New in this release:
  * Added emulation of the CRT overscan border. This border can flash when the player gets hit, when destroying an item or when approaching a fake wall. Suggested by NY00123, dosnostalgic and DiscussMetalDan.
  * Added monster, secret and item statistics to the automap (excluding legacy mode). Suggested by yasagani and Mech4Gaming.
  * By default the automap has a black background color. However, when the floor color is also black, the automap will now use dark gray as the background color. This helps in making a distinction between explored and unexplored areas. Suggested by Mech4Gaming.
  * Added the status messages "Damaging blows" and "Curing".
  * A popup is now shown briefly when a game is saved.
  * Fix: magic missiles now appear from the hand instead of the face. Reported by downmythroat753.
  * Fix: prevent getting stuck in portals. Reported by kylesgamesquest.
  * Fix: only accept Y, N, Enter and ESC in confirmation dialogs. Reported by Azuris and Gez.
  * Fix: the Run action is now by default assigned to the Right Shift, as seen in the original Catacomb 3D.
  * Fix: sort sprites more accurately from back to front. This prevents z-fighting when for example some corpses are positioned close to eachother.
  * Fix: remove hidden obstacle in The Subterranean Vault. This was a wall skeleton spawner without an appropriate wall nearby.
  * Fix: remove unreachable skeleton from Mike's Blastable Passage.
  * Fix: remove all yellow blocks when opening the yellow door in the Secret Halls.
  * Fix: allow player to quit Catacomb 3D while demo is running.

* Version 0.5.2 (2021-04-26) New in this release:
  * Fix: prevent empty save and load menu when no saved games available. Reported by Azuris.

* Version 0.5.1 (2021-04-24) New in this release:
  * Added the "Demo" to the Catacomb Abyss shareware. The demo is a slideshow that originally served as a preview of the Catacomb Adventure series. It can be accessed via the menu or by pressing F10 in the opening screen.
  * Added the "Hints & Solutions" to the Catacomb Adventure series. The Hints & Solutions is a slideshow with top down maps of each level. It is only available in the GOG Catacombs Pack, so not in the shareware version. It can be accessed via the menu.
  * Adapted and improved the menu:
   - In the Catacomb Adventure series, navigating to the previous menu page is done via ESC now, just like in Catacomb 3D. 
   - A menu item can be selected by pressing its first letter. Suggested by NY00123.
   - Added the option "Reset to defaults" to reset all settings to the defaults of the source port.
   - Added the option "Reset to classic look" to reset settings in such a way that they resemble the original DOS game as closely as possible.
  * Added the option "Prevent softlock", to work around design flaws that can prevent the player from completing a level. Currently this impacts only the first level of Catacomb Armageddon. When enabled, upon entering the level a key and a door will change color. 
  * Fix: pause chest pickup when menu, automap or scroll is displayed. Reported by neonvortex.
  * Fix: hide Catacomb 3D destructible walls on the top-down automap. Reported by NY00123.
  * Fix: in the Catacomb 3D warp cheat, start level numbering at 1. Reported by NY00123.
  * Fix: resume music when closing the menu on automap or cheat dialog. Reported by NY00123.
  * Fix: in the Catacomb Adventure series, draw the statusbar numbers with tiles i.s.o. the regular font.
  * Fix: moved the central dot of the crystal sphere (radar), such that it matches with the original DOS game.
  * Fix: prevent mouse stutter when VSync is disabled. This bug was spotted in a recorded stream of Artyom Havok.
  * Fix: do not use potions when player health is already at 100. Reported by coldner.

* Version 0.5.0 (2020-12-19) New in this release:
  * Added an automap. The automap can be opened via a configurable key, with the default being the 'O' key. The automap can be visualized in four different styles, which is configurable via the Video menu. The automap feature was suggested by Master O, with additional feedback provided by SiFi270 and NY00123.
  * Added an option to simulate the original 320x200 screen resolution, which can be enabled via the Video menu. Suggested by Dinoaur.
  * Added a configurable "Strafe" key, which when pressed changes the turn keys into strafe keys. This was also possible in the original games. Suggested by Dinoaur.
  * Added an option for a mana bar, which can be enabled via the Controls menu. When enabled, the mana bar will restrict the use of the primary magic missiles. Suggested by Hellbent.
  * Fix: create folder prior to storing the log file. Reported by InDOOMnesia.
  * Fix: touching a wall in Catacomb 3D will now make a sound. Reported by Dinoaur.
  * Fix: prevented enemies from occasionally spotting the player through solid walls.
  * Fix: keep player hand high while fire button is pressed in the Catacomb Adventure series.

* Version 0.4.3 (2020-03-25) New in this release:
  * Improved the performance of the OpenGL renderer.
  * Added an "Extended" mode for the "Show frame rate" setting. When set, additional information such as the name of the graphics card is displayed.
  * Added a "Loading Control Panel" animation to Catacomb 3D. Suggested by Dinoaur.
  * Fix: support for OpenGL 2.x hardware. Suggested by InDOOMnesia.
  * Fix: melee enemies in Catacomb 3D will no longer attempt to attack from too far away. Reported by Dinoaur.
  * Fix: added the fizzle fade effect when opening the title screen in Catacomb 3D. Reported by Dinoaur.
  * Fix: prevent the wizard's hand from bopping while shooting magick missiles repeatedly. Reported by Dinoaur.

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
