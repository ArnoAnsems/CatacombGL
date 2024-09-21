/* Copyright (C) 2014-2024 NY00123
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <limits.h>
#include <string.h>

#include "../audio/be_audio_private.h"
//#include "../input/be_input_tables.h"
#include "be_cross.h"
//#include "be_features.h"
//#include "be_gamever.h"
#include "be_st.h"

#define BE_ST_DEFAULT_FARPTRSEGOFFSET 0x14

RefKeenConfig g_refKeenCfg;
RefkeenDynamicConfig g_refKeenDynamicCfg;

typedef enum {
	BE_ST_CFG_VAL_ENUM,
	BE_ST_CFG_VAL_HEX_INT,
	BE_ST_CFG_VAL_INT,
	BE_ST_CFG_VAL_DIMS,
	BE_ST_CFG_VAL_STR,
	BE_ST_CFG_VAL_DISPLAY_NUM,
	BE_ST_CFG_VAL_SDL_RENDERER,
} BE_ST_CFG_VAL_T;

typedef struct {
	// Pointer to internal configuration value; Width of window dimensions.
	void *setting;
	// Height of window dimensions; Hidden setting marker otherwise.
	void *ptraux;
	// Configuration key
	const char *key;
	// Classification of configuration value
	BE_ST_CFG_VAL_T valType;
	// Values of variable meanings; An intptr one is
	// allowed to hold a pointer, but doesn't have to.
	int aux0;
	intptr_t aux1, aux2;
} BE_ST_CFG_Setting_T;

// Enumerated by SDL_GameControllerButton, for most
static const char *g_sdlControlSchemeKeyMapCfgVals[] = {
	"a", "b", "x", "y", "", "", "", "lstick", "rstick", "lshoulder", "rshoulder", "dpadup", "dpaddown", "dpadleft", "dpadright", "", "paddle1", "paddle2", "paddle3", "paddle4",
	"ltrigger", "rtrigger", // Actually axes but these are added as extras
};

// Enumerated by other enums, as well as "bool"
static const char *g_be_setting_bool_vals[] = {"false", "true"};
#ifdef REFKEEN_ENABLE_LAUNCHER
static const char *g_be_setting_wintype_vals[] = {"default", "full", "software"};
#endif
static const char *g_be_setting_vsync_vals[] = {"auto", "off", "on"};
static const char *g_be_setting_scaletype_vals[] = {"aspect", "fill"};
static const char *g_be_setting_mousegrab_vals[] = {"auto", "off", "commonly"};
#ifdef BE_ST_ENABLE_SETTING_SB
static const char *g_be_setting_sb_vals[] = {"off", "sb", "sbpro" ,"sb16"};
#endif
#ifdef REFKEEN_CONFIG_ENABLE_TOUCHINPUT
static const char *g_be_setting_touchinput_vals[] = {"auto", "off", "forced"};
#endif

#define DEF_ENUM(setting, key, strs, def) \
	{&g_refKeenCfg.setting, 0, key, BE_ST_CFG_VAL_ENUM, def, (intptr_t)strs, BE_Cross_ArrayLen(strs)},
#define DEF_INT(setting, key, def, min, max) \
	{&g_refKeenCfg.setting, 0, key, BE_ST_CFG_VAL_INT, def, min, max},
#define DEF_DIMS(width, height, key, defw, defh) \
	{&g_refKeenCfg.width, &g_refKeenCfg.height, key, BE_ST_CFG_VAL_DIMS, defw, defh},
#define DEF_STR(setting, key) \
	{&g_refKeenCfg.setting, 0, key, BE_ST_CFG_VAL_STR, sizeof(g_refKeenCfg.setting)},
#define DEF_CUSTOM_INT(setting, key, type, def) \
	{&g_refKeenCfg.setting, 0, key, type, def},

#define DEF_BOOL(setting, key, def) \
	DEF_ENUM(setting, key, g_be_setting_bool_vals, def)

#define DEF_CTRL_NONKEY_BINDS_KDREAMS_ENUMS(i, k, def) \
	DEF_ENUM(kdreams.binds[BE_ST_CTRL_BIND_KDREAMS_ ## i].mouse, "mbind_" k, g_be_st_mouseFeatureIdToNameMap, BE_ST_CTRL_MOUSE_BUT_MAX) \
	DEF_ENUM(kdreams.binds[BE_ST_CTRL_BIND_KDREAMS_ ## i].pad, "pbind_" k, g_be_st_padFeatureIdToNameMap, def)

#define DEF_CTRL_BINDS_KDREAMS_ENUMS(i, k, def) \
	DEF_ENUM(kdreams.binds[BE_ST_CTRL_BIND_KDREAMS_ ## i].key, "kbind_" k, g_be_st_keyIdToNameMap, 0) \
	DEF_CTRL_NONKEY_BINDS_KDREAMS_ENUMS(i, k, def)

#define DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(i, k, def) \
	DEF_ENUM(kdreams.binds[BE_ST_CTRL_BIND_KDREAMS_ ## i].pad, "altcontrolscheme_" k, g_sdlControlSchemeKeyMapCfgVals, def)

#define DEF_CTRL_NONKEY_BINDS_CAT3D_ENUMS(i, k, def) \
	DEF_ENUM(cat3d.binds[BE_ST_CTRL_BIND_CAT3D_ ## i].mouse, "mbind_" k, g_be_st_mouseFeatureIdToNameMap, BE_ST_CTRL_MOUSE_BUT_MAX) \
	DEF_ENUM(cat3d.binds[BE_ST_CTRL_BIND_CAT3D_ ## i].pad, "pbind_" k, g_be_st_padFeatureIdToNameMap, def)

#define DEF_CTRL_BINDS_CAT3D_ENUMS(i, k, def) \
	DEF_ENUM(cat3d.binds[BE_ST_CTRL_BIND_CAT3D_ ## i].key, "kbind_" k, g_be_st_keyIdToNameMap, 0) \
	DEF_CTRL_NONKEY_BINDS_CAT3D_ENUMS(i, k, def)

#define DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(i, k, def) \
	DEF_ENUM(cat3d.binds[BE_ST_CTRL_BIND_CAT3D_ ## i].pad, "altcontrolscheme_" k, g_sdlControlSchemeKeyMapCfgVals, def)

#define DEF_CTRL_NONKEY_BINDS_WOLF3D_ENUMS(i, k, def) \
	DEF_ENUM(wolf3d.binds[BE_ST_CTRL_BIND_WOLF3D_ ## i].mouse, "mbind_" k, g_be_st_mouseFeatureIdToNameMap, BE_ST_CTRL_MOUSE_BUT_MAX) \
	DEF_ENUM(wolf3d.binds[BE_ST_CTRL_BIND_WOLF3D_ ## i].pad, "pbind_" k, g_be_st_padFeatureIdToNameMap, def)

#define DEF_CTRL_BINDS_WOLF3D_ENUMS(i, k, def) \
	DEF_ENUM(wolf3d.binds[BE_ST_CTRL_BIND_WOLF3D_ ## i].key, "kbind_" k, g_be_st_keyIdToNameMap, 0) \
	DEF_CTRL_NONKEY_BINDS_WOLF3D_ENUMS(i, k, def)

#define DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(i, k, def) \
	DEF_ENUM(wolf3d.binds[BE_ST_CTRL_BIND_WOLF3D_ ## i].pad, "altcontrolscheme_" k, g_sdlControlSchemeKeyMapCfgVals, def)

#define DEF_HIDDEN_ENUM(setting, key, strs, def) \
	{&g_refKeenCfg.setting, &g_refKeenCfg.setting, key, BE_ST_CFG_VAL_ENUM, def, (intptr_t)strs, BE_Cross_ArrayLen(strs)},

#define DEF_HIDDEN_BOOL(setting, key, def) \
	DEF_HIDDEN_ENUM(setting, key, g_be_setting_bool_vals, def)

#define DEF_HIDDEN_HEX_INT(setting, key, def, min, max) \
	{&g_refKeenCfg.setting, &g_refKeenCfg.setting, key, BE_ST_CFG_VAL_HEX_INT, def, min, max},


static BE_ST_CFG_Setting_T g_be_st_settings[] = {
#ifdef REFKEEN_CONFIG_USER_FULLSCREEN_TOGGLE
	DEF_BOOL(isFullscreen, "fullscreen", false)
#endif
#ifdef REFKEEN_CONFIG_USER_FULLSCREEN_RES_SETTING
	DEF_DIMS(fullWidth, fullHeight, "fullres", 0, 0)
#endif
	DEF_DIMS(winWidth, winHeight, "windowres", 0, 0)
#ifdef REFKEEN_ENABLE_LAUNCHER
//	DEF_DIMS(launcherWinWidth, launcherWinHeight, "launcherwindowres", 0, 0)
	DEF_ENUM(launcherWinType, "launcherwindowtype", g_be_setting_wintype_vals, LAUNCHER_WINDOW_DEFAULT)
	DEF_STR(launcherExeArgs, "launcherexeargs")
#endif
	DEF_STR(lastSelectedGameExe, "lastselectedgameexe")
	//DEF_ENUM(lastSelectedGameVer, "lastselectedgamever", refkeen_gamever_strs, BE_GAMEVER_LAST)
	DEF_CUSTOM_INT(displayNum, "displaynum", BE_ST_CFG_VAL_DISPLAY_NUM, 0)
	DEF_BOOL(rememberDisplayNum, "rememberdisplaynum", true)
	DEF_CUSTOM_INT(sdlRendererDriver, "sdlrenderer", BE_ST_CFG_VAL_SDL_RENDERER, -1)
	DEF_ENUM(vSync, "vsync", g_be_setting_vsync_vals, VSYNC_OFF)
	DEF_BOOL(isBilinear, "bilinear", true)
	DEF_ENUM(scaleType, "scaletype", g_be_setting_scaletype_vals, SCALE_ASPECT)
	DEF_INT(scaleFactor, "scalefactor", 2, 1, INT_MAX)
	DEF_BOOL(forceFullSoftScaling, "forcefullsoftscaling", false)
	DEF_BOOL(showEndoom, "showendoom", true)
	DEF_ENUM(mouseGrab, "mousegrab", g_be_setting_mousegrab_vals, MOUSEGRAB_AUTO)
	DEF_INT(sndInterThreadBufferRatio, "sndinterthreadbufferratio", 2, 1, INT_MAX)
	// 49716 may lead to unexpected behaviors on Android
	DEF_INT(sndSampleRate, "sndsamplerate", 48000, 1, INT_MAX)
	DEF_BOOL(sndSubSystem, "sndsubsystem", true)
	DEF_BOOL(oplEmulation, "oplemulation", true)
#ifdef BE_ST_ENABLE_SETTING_SB
	DEF_ENUM(sb, "sbemu", g_be_setting_sb_vals, SOUNDBLASTER_SB16)
#endif
	DEF_INT(pcSpkVol, "pcspkvol", BE_AUDIO_VOL_MAX, BE_AUDIO_VOL_MIN, BE_AUDIO_VOL_MAX)
	DEF_INT(oplVol, "oplvol", BE_AUDIO_VOL_MAX, BE_AUDIO_VOL_MIN, BE_AUDIO_VOL_MAX)
#ifdef BE_ST_ENABLE_SETTING_DIGIVOL
	DEF_INT(digiVol, "digivol", BE_AUDIO_VOL_MAX, BE_AUDIO_VOL_MIN, BE_AUDIO_VOL_MAX)
#endif
#ifndef REFKEEN_RESAMPLER_NONE
	DEF_BOOL(useResampler, "useresampler", true)
#endif
#ifdef REFKEEN_CONFIG_ENABLE_TOUCHINPUT
	#ifdef REFKEEN_CONFIG_AUTODETECT_TOUCHINPUT_BY_DEFAULT
	DEF_ENUM(touchInputToggle, "touchinput", g_be_setting_touchinput_vals, TOUCHINPUT_AUTO)
	#else
	DEF_ENUM(touchInputToggle, "touchinput", g_be_setting_touchinput_vals, TOUCHINPUT_OFF)
	#endif
	DEF_BOOL(touchInputDebugging, "touchinputdebugging", false)
#endif
	DEF_BOOL(altControlScheme, "altcontrolscheme", true)

	DEF_HIDDEN_BOOL(manualGameVerMode, "manualgamevermode", false)
#ifdef BE_CROSS_ENABLE_FARPTR_CFG
	DEF_HIDDEN_HEX_INT(farPtrSegOffset, "farptrsegoffset", BE_ST_DEFAULT_FARPTRSEGOFFSET, 0U, 65535U)
#endif
};

#ifdef REFKEEN_HAS_VER_KDREAMS
static BE_ST_CFG_Setting_T g_be_st_kdreams_settings[] = {
	DEF_BOOL(kdreams.absMouseMotion, "absmousemotion", false)
	DEF_BOOL(kdreams.useLeftStick, "lstick", true)
	DEF_BOOL(kdreams.useRightStick, "rstick", false)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(UP, "up", BE_ST_CTRL_BUT_DPAD_UP)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(DOWN, "down", BE_ST_CTRL_BUT_DPAD_DOWN)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(LEFT, "left", BE_ST_CTRL_BUT_DPAD_LEFT)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(RIGHT, "right", BE_ST_CTRL_BUT_DPAD_RIGHT)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(JUMP, "jump", BE_ST_CTRL_BUT_A)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(THROW, "throw", BE_ST_CTRL_BUT_B)
	DEF_CTRL_BINDS_KDREAMS_ENUMS(STATS, "stats", BE_ST_CTRL_BUT_X)
	DEF_CTRL_NONKEY_BINDS_KDREAMS_ENUMS(FUNCKEYS, "funckeys", BE_ST_CTRL_BUT_MAX) // HACK for left trigger
	DEF_CTRL_NONKEY_BINDS_KDREAMS_ENUMS(DEBUGKEYS, "debugkeys", BE_ST_CTRL_BUT_LSTICK)
};

static BE_ST_CFG_Setting_T g_be_st_kdreams_legacy_settings[] = {
	DEF_BOOL(kdreams.useLeftStick, "altcontrolscheme_lstick", true)
	DEF_BOOL(kdreams.useRightStick, "altcontrolscheme_rstick", false)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(UP, "up", BE_ST_CTRL_BUT_DPAD_UP)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(DOWN, "down", BE_ST_CTRL_BUT_DPAD_DOWN)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(LEFT, "left", BE_ST_CTRL_BUT_DPAD_LEFT)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(RIGHT, "right", BE_ST_CTRL_BUT_DPAD_RIGHT)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(JUMP, "jump", BE_ST_CTRL_BUT_A)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(THROW, "throw", BE_ST_CTRL_BUT_B)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(STATS, "stats", BE_ST_CTRL_BUT_X)
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(FUNCKEYS, "funckeys", BE_ST_CTRL_BUT_MAX) // HACK for left trigger
	DEF_CTRL_LEGACY_BINDS_KDREAMS_ENUMS(DEBUGKEYS, "debugkeys", BE_ST_CTRL_BUT_LSTICK)
};
#endif

#ifdef REFKEEN_HAS_VER_CATACOMB_ALL
static BE_ST_CFG_Setting_T g_be_st_cat3d_settings[] = {
	DEF_BOOL(cat3d.useLeftStick, "lstick", true)
	DEF_BOOL(cat3d.useRightStick, "rstick", false)
	DEF_BOOL(cat3d.analogMotion, "analogmotion", false)
	DEF_BOOL(cat3d.novert, "novert", false)
	// FIXME: HACK (extra 2 are for triggers)
	DEF_CTRL_BINDS_CAT3D_ENUMS(UP, "up", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_CAT3D_ENUMS(DOWN, "down", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_CAT3D_ENUMS(LEFT, "left", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_CAT3D_ENUMS(RIGHT, "right", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_CAT3D_ENUMS(FIRE, "fire", BE_ST_CTRL_BUT_LSHOULDER)
	DEF_CTRL_BINDS_CAT3D_ENUMS(STRAFE, "strafe", BE_ST_CTRL_BUT_B)
	DEF_CTRL_BINDS_CAT3D_ENUMS(DRINK, "drink", BE_ST_CTRL_BUT_A)
	DEF_CTRL_BINDS_CAT3D_ENUMS(BOLT, "bolt", BE_ST_CTRL_BUT_X)
	DEF_CTRL_BINDS_CAT3D_ENUMS(NUKE, "nuke", BE_ST_CTRL_BUT_Y)
	DEF_CTRL_BINDS_CAT3D_ENUMS(FASTTURN, "fastturn", BE_ST_CTRL_BUT_RSHOULDER)
#if (defined REFKEEN_HAS_VER_CAT3D) || (defined REFKEEN_HAS_VER_CATABYSS)
	DEF_CTRL_NONKEY_BINDS_CAT3D_ENUMS(SCROLLS, "scrolls", BE_ST_CTRL_BUT_MAX+1) // HACK for right trigger
#endif
#ifdef REFKEEN_HAS_VER_CATADVENTURES
	DEF_CTRL_NONKEY_BINDS_CAT3D_ENUMS(FUNCKEYS, "funckeys", BE_ST_CTRL_BUT_MAX) // HACK for left trigger
#endif
	DEF_CTRL_NONKEY_BINDS_CAT3D_ENUMS(DEBUGKEYS, "debugkeys", BE_ST_CTRL_BUT_LSTICK)
};

static BE_ST_CFG_Setting_T g_be_st_cat3d_legacy_settings[] = {
	DEF_BOOL(cat3d.useLeftStick, "altcontrolscheme_lstick", true)
	DEF_BOOL(cat3d.useRightStick, "altcontrolscheme_rstick", false)
	DEF_BOOL(cat3d.analogMotion, "altcontrolscheme_analogmotion", false)
	DEF_BOOL(cat3d.novert, "novert", false) // Used to be in reflection-catacomb.cfg
	// FIXME: HACK (extra 2 are for triggers)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(UP, "up", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(DOWN, "down", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(LEFT, "left", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(RIGHT, "right", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(FIRE, "fire", BE_ST_CTRL_BUT_LSHOULDER)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(STRAFE, "strafe", BE_ST_CTRL_BUT_B)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(DRINK, "drink", BE_ST_CTRL_BUT_A)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(BOLT, "bolt", BE_ST_CTRL_BUT_X)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(NUKE, "nuke", BE_ST_CTRL_BUT_Y)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(FASTTURN, "fastturn", BE_ST_CTRL_BUT_RSHOULDER)
#if (defined REFKEEN_HAS_VER_CAT3D) || (defined REFKEEN_HAS_VER_CATABYSS)
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(SCROLLS, "scrolls", BE_ST_CTRL_BUT_MAX+1) // HACK for right trigger
#endif
#ifdef REFKEEN_HAS_VER_CATADVENTURES
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(FUNCKEYS, "funckeys", BE_ST_CTRL_BUT_MAX) // HACK for left trigger
#endif
	DEF_CTRL_LEGACY_BINDS_CAT3D_ENUMS(DEBUGKEYS, "debugkeys", BE_ST_CTRL_BUT_LSTICK)
};
#endif


#ifdef REFKEEN_HAS_VER_WOLF3D_ALL
static BE_ST_CFG_Setting_T g_be_st_wolf3d_settings[] = {
	DEF_BOOL(wolf3d.lowFPS, "lowfps", false)
	DEF_BOOL(wolf3d.useLeftStick, "lstick", true)
	DEF_BOOL(wolf3d.useRightStick, "rstick", false)
	DEF_BOOL(wolf3d.analogMotion, "analogmotion", false)
	DEF_BOOL(wolf3d.novert, "novert", false)
	// FIXME: HACK (extra 2 are for triggers)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(UP, "up", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(DOWN, "down", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(LEFT, "left", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(RIGHT, "right", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(FIRE, "fire", BE_ST_CTRL_BUT_LSHOULDER)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(STRAFE, "strafe", BE_ST_CTRL_BUT_B)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(USE, "use", BE_ST_CTRL_BUT_MAX+1) // HACK for right trigger
	DEF_CTRL_BINDS_WOLF3D_ENUMS(RUN, "run", BE_ST_CTRL_BUT_RSHOULDER)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(WEAPON1, "weapon1", BE_ST_CTRL_BUT_DPAD_DOWN)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(WEAPON2, "weapon2", BE_ST_CTRL_BUT_DPAD_RIGHT)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(WEAPON3, "weapon3", BE_ST_CTRL_BUT_DPAD_LEFT)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(WEAPON4, "weapon4", BE_ST_CTRL_BUT_DPAD_UP)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(WEAPON5, "weapon5", BE_ST_CTRL_BUT_X)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(WEAPON6, "weapon6", BE_ST_CTRL_BUT_Y)
	DEF_CTRL_BINDS_WOLF3D_ENUMS(MAP, "map", BE_ST_CTRL_BUT_A)
	DEF_CTRL_NONKEY_BINDS_WOLF3D_ENUMS(FUNCKEYS, "funckeys", BE_ST_CTRL_BUT_MAX) // HACK for left trigger
	DEF_CTRL_NONKEY_BINDS_WOLF3D_ENUMS(DEBUGKEYS, "debugkeys", BE_ST_CTRL_BUT_LSTICK)
};

static BE_ST_CFG_Setting_T g_be_st_wolf3d_legacy_settings[] = {
	DEF_BOOL(wolf3d.useLeftStick, "altcontrolscheme_lstick", true)
	DEF_BOOL(wolf3d.useRightStick, "altcontrolscheme_rstick", false)
	DEF_BOOL(wolf3d.analogMotion, "altcontrolscheme_analogmotion", false)
	// FIXME: HACK (extra 2 are for triggers)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(UP, "up", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(DOWN, "down", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(LEFT, "left", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(RIGHT, "right", BE_ST_CTRL_BUT_GUIDE)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(FIRE, "fire", BE_ST_CTRL_BUT_LSHOULDER)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(STRAFE, "strafe", BE_ST_CTRL_BUT_B)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(USE, "use", BE_ST_CTRL_BUT_MAX+1) // HACK for right trigger
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(RUN, "run", BE_ST_CTRL_BUT_RSHOULDER)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(WEAPON1, "weapon1", BE_ST_CTRL_BUT_DPAD_DOWN)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(WEAPON2, "weapon2", BE_ST_CTRL_BUT_DPAD_RIGHT)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(WEAPON3, "weapon3", BE_ST_CTRL_BUT_DPAD_LEFT)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(WEAPON4, "weapon4", BE_ST_CTRL_BUT_DPAD_UP)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(WEAPON5, "weapon5", BE_ST_CTRL_BUT_X)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(WEAPON6, "weapon6", BE_ST_CTRL_BUT_Y)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(MAP, "map", BE_ST_CTRL_BUT_A)
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(FUNCKEYS, "funckeys", BE_ST_CTRL_BUT_MAX) // HACK for left trigger
	DEF_CTRL_LEGACY_BINDS_WOLF3D_ENUMS(DEBUGKEYS, "debugkeys", BE_ST_CTRL_BUT_LSTICK)
};
#endif

// These ones are implementation-defined
void BEL_ST_ParseSetting_DisplayNum(int *displayNum, const char *buffer);
void BEL_ST_ParseSetting_SDLRendererDriver(int *driver, const char *buffer);
void BEL_ST_SaveSDLRendererDriverToConfig(FILE *fp, const char *key, int driver);

static void BEL_ST_SetConfigDefaults(BE_ST_CFG_Setting_T *settings, int n)
{
	for (int i = 0; i < n; ++i)
		switch (settings[i].valType)
		{
		case BE_ST_CFG_VAL_STR:
			break;
		case BE_ST_CFG_VAL_DIMS:
			*(int *)settings[i].setting = settings[i].aux0;
			*(int *)settings[i].ptraux = settings[i].aux1;
			break;
		default:
			*(int *)settings[i].setting = settings[i].aux0;
		}
}

static void BEL_ST_ParseEnum(int *val, const char *list[], int len, const char *buffer)
{
	for (int i = 0; i < len; ++i)
		if (list[i] && !strcmp(buffer, list[i]))
		{
			*val = i;
			break;
		}
}

static void BEL_ST_WriteEnum(FILE *fp, const char *key, const char *list[], int len, int val)
{
	fprintf(fp, "%s=%s\n", key, ((val >= 0) && (val < len)) ? list[val] : "");
}

static void BEL_ST_ParseHexInt(int *val, int min, int max, const char *buffer)
{
	long ret = strtol(buffer, 0, 16);
	if ((ret >= min) && (ret <= max))
		*val = ret;
}

static void BEL_ST_WriteHexInt(FILE *fp, const char *key, int val)
{
	fprintf(fp, "%s=0x%x\n", key, val);
}

static void BEL_ST_ParseInt(int *val, int min, int max, const char *buffer)
{
	int ret = atoi(buffer);
	if ((ret >= min) && (ret <= max))
		*val = ret;
}

static void BEL_ST_WriteInt(FILE *fp, const char *key, int val)
{
	fprintf(fp, "%s=%d\n", key, val);
}

static void BEL_ST_ParseDims(int *w, int *h, const char *buffer)
{
	sscanf(buffer, "%dx%d", w, h);
}

static void BEL_ST_WriteDims(FILE *fp, const char *key, int w, int h)
{
	fprintf(fp, "%s=%dx%d\n", key, w, h);
}

static void BEL_ST_ParseString(char *val, int len, const char *buffer)
{
	BE_Cross_safeandfastcstringcopy(val, val + len, buffer);
}

static void BEL_ST_WriteString(FILE *fp, const char *key, const char *val)
{
	fprintf(fp, "%s=%s\n", key, val);
}

static void BEL_ST_ParseSetting(BE_ST_CFG_Setting_T *setting, const char *valStr)
{
	switch (setting->valType)
	{
	case BE_ST_CFG_VAL_ENUM:
		BEL_ST_ParseEnum((int *)setting->setting, (const char **)setting->aux1, setting->aux2, valStr);
		break;
	case BE_ST_CFG_VAL_DIMS:
		BEL_ST_ParseDims((int *)setting->setting, (int *)setting->ptraux, valStr);
		break;
	case BE_ST_CFG_VAL_HEX_INT:
		BEL_ST_ParseHexInt((int *)setting->setting, setting->aux1, setting->aux2, valStr);
		break;
	case BE_ST_CFG_VAL_INT:
		BEL_ST_ParseInt((int *)setting->setting, setting->aux1, setting->aux2, valStr);
		break;
	case BE_ST_CFG_VAL_DISPLAY_NUM:
		BEL_ST_ParseSetting_DisplayNum((int *)setting->setting, valStr);
		break;
	case BE_ST_CFG_VAL_SDL_RENDERER:
		BEL_ST_ParseSetting_SDLRendererDriver((int *)setting->setting, valStr);
		break;
	case BE_ST_CFG_VAL_STR:
		BEL_ST_ParseString((char *)setting->setting, setting->aux0, valStr);
		break;
	}
}

static void BEL_ST_SaveSetting(FILE *fp, const BE_ST_CFG_Setting_T *setting)
{
	switch (setting->valType)
	{
	case BE_ST_CFG_VAL_ENUM:
		BEL_ST_WriteEnum(fp, setting->key, (const char **)setting->aux1, setting->aux2, *(int *)setting->setting);
		break;
	case BE_ST_CFG_VAL_DIMS:
		BEL_ST_WriteDims(fp, setting->key, *(int *)setting->setting, *(int *)setting->ptraux);
		break;
	case BE_ST_CFG_VAL_HEX_INT:
		BEL_ST_WriteHexInt(fp, setting->key, *(int *)setting->setting);
		break;
	case BE_ST_CFG_VAL_INT:
		BEL_ST_WriteInt(fp, setting->key, *(int *)setting->setting);
		break;
	case BE_ST_CFG_VAL_DISPLAY_NUM:
		BEL_ST_WriteInt(fp, setting->key, *(int *)setting->setting);
		break;
	case BE_ST_CFG_VAL_SDL_RENDERER:
		BEL_ST_SaveSDLRendererDriverToConfig(fp, setting->key, *(int *)setting->setting);
		break;
	case BE_ST_CFG_VAL_STR:
		BEL_ST_WriteString(fp, setting->key, (const char *)setting->setting);
		break;
	}
}

void BEL_ST_SetConfig(void)
{
	BEL_ST_SetConfigDefaults(g_be_st_settings, BE_Cross_ArrayLen(g_be_st_settings));
}
