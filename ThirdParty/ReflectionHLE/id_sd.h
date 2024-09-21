/* Catacomb 3-D Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <stdint.h>

#include "../../src/Engine/PCSound.h"
#include "../../src/Engine/AdlibSound.h"


#ifndef	__ID_SD__
#define	__ID_SD__

#define	TickBase	70		// 70Hz per tick - used as a base for timer 0

typedef	enum	{
					sdm_Off,
					sdm_PC,sdm_AdLib,
				}	SDMode;
typedef	enum	{
					smm_Off,smm_AdLib
				}	SMMode;


//	PC Sound stuff
#define	pcTimer		0x42
#define	pcTAccess	0x43
#define	pcSpeaker	0x61

#define	pcSpkBits	3

// 	Registers for the Sound Blaster card - needs to be offset by n0
#define	sbReset		0x206
#define	sbReadData	0x20a
#define	sbWriteCmd	0x20c
#define	sbWriteData	0x20c
#define	sbWriteStat	0x20c
#define	sbDataAvail	0x20e



// 	Registers for the AdLib card
// Operator stuff
#define	alChar		0x20
#define	alScale		0x40
#define	alAttack	0x60
#define	alSus		0x80
#define	alWave		0xe0
// Channel stuff
#define	alFreqL		0xa0
#define	alFreqH		0xb0
#define	alFeedCon	0xc0
// Global stuff
#define	alEffects	0xbd


//
//	Sequencing stuff
//
#define	sqMaxTracks	10
#define	sqMaxMoods	1	// DEBUG

#define	sev_Null		0	// Does nothing
#define	sev_NoteOff		1	// Turns a note off
#define	sev_NoteOn		2	// Turns a note on
#define	sev_NotePitch	3	// Sets the pitch of a currently playing note
#define	sev_NewInst		4	// Installs a new instrument
#define	sev_NewPerc		5	// Installs a new percussive instrument
#define	sev_PercOn		6	// Turns a percussive note on
#define	sev_PercOff		7	// Turns a percussive note off
#define	sev_SeqEnd		-1	// Terminates a sequence



#define	sqmode_Normal		0
#define	sqmode_FadeIn		1
#define	sqmode_FadeOut		2

#define	sqMaxFade		64	// DEBUG


extern	SDMode		SoundMode;
extern	SMMode		MusicMode;
//NOT DECLARED HERE - USE WRAPPERS LIKE SD_GetTimeCount
//extern	id0_longword_t	TimeCount;					// Global time in ticks
#ifdef REFKEEN_VER_CATADVENTURES
extern 	SDMode		oldsoundmode;
#endif

// Function prototypes
extern	void	SD_Startup(void),
				SD_Shutdown(void),
				SD_Default(bool gotit,SDMode sd,SMMode sm),
				SD_WaitSoundDone(void),
				SD_StartMusic(FileChunk* music),
				SD_MusicOn(void),
				SD_MusicOff(void),
				SD_SetUserHook(void (*hook)(void));
extern bool    SD_SetSoundMode(SDMode mode);
extern SDMode  SD_GetSoundMode();
extern bool    SD_SetMusicMode(SMMode mode);
extern SMMode  SD_GetMusicMode();
extern bool    SD_SoundPlaying(void);

extern	void	SDL_PCPlaySound(PCSound* sound),
				SDL_PCStopSound(void),
				SDL_ALPlaySound(AdlibSound* sound),
				SDL_ALStopSound(void),
                SD_StopSound(void);

#endif
