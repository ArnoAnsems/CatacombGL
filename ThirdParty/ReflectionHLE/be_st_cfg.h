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

#ifndef BE_ST_CFG_H
#define BE_ST_CFG_H

//#include "be_features.h"
#include "be_st.h"
#include "refkeen_config.h"

// NOTE: These structs are generally used just in the ReflectionHLE backend.
// There might still be exceptions to this.

typedef enum { VSYNC_AUTO, VSYNC_OFF, VSYNC_ON } VSyncSettingType;
typedef enum { SCALE_ASPECT, SCALE_FILL } ScaleTypeSettingType;
typedef enum { TOUCHINPUT_AUTO, TOUCHINPUT_OFF, TOUCHINPUT_FORCED } TouchInputSettingType;
typedef enum { MOUSEGRAB_AUTO, MOUSEGRAB_OFF, MOUSEGRAB_COMMONLY } MouseGrabSettingType;
typedef enum {
	SOUNDBLASTER_OFF, SOUNDBLASTER_SB, SOUNDBLASTER_SBPRO, SOUNDBLASTER_SB16
} SoundBlasterSettingType;

enum { BE_AUDIO_VOL_MIN = 0, BE_AUDIO_VOL_MAX = 15 };

#ifdef REFKEEN_ENABLE_LAUNCHER
#define LAUNCHER_EXE_ARGS_BUFFERLEN 80
typedef enum { LAUNCHER_WINDOW_DEFAULT, LAUNCHER_WINDOW_FULL, LAUNCHER_WINDOW_SOFTWARE } LauncherWindowSettingType;
#endif

#define SELECTED_EXE_FILENAME_BUFFERLEN 13

// Notes: Certain bool fields are actually int for compatibility with be_cfg.c.
// Generally speaking, only a limited set of field types is used.

typedef struct
{
	int/*bool*/ isFullscreen;
	int fullWidth, fullHeight;
	int winWidth, winHeight;
#ifdef REFKEEN_ENABLE_LAUNCHER
	// Now using just winWidth and winHeight due to seamless launcher->game transitions
	//int launcherWinWidth, launcherWinHeight;
	int launcherWinType;
	char launcherExeArgs[LAUNCHER_EXE_ARGS_BUFFERLEN];
#endif
	char lastSelectedGameExe[SELECTED_EXE_FILENAME_BUFFERLEN];
	int lastSelectedGameVer;
	int displayNum;
	int/*bool*/ rememberDisplayNum;
	int sdlRendererDriver;
	int vSync;
	int/*bool*/ isBilinear;
	int scaleType;
	int scaleFactor;
	int/*bool*/ forceFullSoftScaling;
	int/*bool*/ showEndoom;
	int mouseGrab;
	int sndInterThreadBufferRatio;
	int sndSampleRate;
	int/*bool*/ sndSubSystem;
	int/*bool*/ oplEmulation;
	int sb;
	int pcSpkVol, oplVol, digiVol;
#ifndef REFKEEN_RESAMPLER_NONE
	int/*bool*/ useResampler;
#endif
	int touchInputToggle;
	int/*bool*/ touchInputDebugging;
	int/*bool*/ altControlScheme;
	int/*bool*/ manualGameVerMode;
	unsigned int farPtrSegOffset; // Actually used just in The Catacomb Armageddon/Apocalypse

	// Game specific settings
#ifdef REFKEEN_HAS_VER_KDREAMS
	struct
	{
		int/*bool*/ absMouseMotion;
		int/*bool*/ useLeftStick;
		int/*bool*/ useRightStick;
		struct
		{
			int key;
			int mouse;
			int pad;
		} binds[BE_ST_CTRL_BIND_KDREAMS_TOTAL];
	} kdreams;
#endif
#ifdef REFKEEN_HAS_VER_CATACOMB_ALL
	struct
	{
		int/*bool*/ useLeftStick;
		int/*bool*/ useRightStick;
		int/*bool*/ analogMotion;
		int/*bool*/ novert;
		struct
		{
			int key;
			int mouse;
			int pad;
		} binds[BE_ST_CTRL_BIND_CAT3D_TOTAL];
	} cat3d;
#endif
#ifdef REFKEEN_HAS_VER_WOLF3D_ALL
	struct
	{
		int/*bool*/ lowFPS;
		int/*bool*/ useLeftStick;
		int/*bool*/ useRightStick;
		int/*bool*/ analogMotion;
		int/*bool*/ novert;
		struct
		{
			int key;
			int mouse;
			int pad;
		} binds[BE_ST_CTRL_BIND_WOLF3D_TOTAL];
	} wolf3d;
#endif
} RefKeenConfig;

extern RefKeenConfig g_refKeenCfg;

// These ones are set on game startup, as they can be used by
// game-agnostic code while still depending on the exact game
typedef struct {
	bool absMouseMotion;
	bool novert;
} RefkeenDynamicConfig;

extern RefkeenDynamicConfig g_refKeenDynamicCfg;

#endif
