/* Copyright (C) 2014-2016 NY00123
 *
 * This file is part of Reflection Keen.
 *
 * Reflection Keen is free software; you can redistribute it and/or modify
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

#ifndef	_BE_ST_
#define _BE_ST_

//#include <stdbool.h>
#include <stdint.h>


/*** General ***/
void BE_ST_InitCommon(void); // Before game or launcher
void BE_ST_PrepareForGameStartup(void); // Before game

void BE_ST_PollEvents(void);

/*** Audio/timer (vanilla Keen kind-of has these mixed) ***/
void BE_ST_StartAudioSDService(void (*funcPtr)(void));
void BE_ST_StopAudioSDService(void);
void BE_ST_LockAudioRecursively(void);
void BE_ST_UnlockAudioRecursively(void);
void BE_ST_PrepareForManualAudioSDServiceCall(void);
bool BE_ST_IsEmulatedOPLChipReady(void);
// Should be used in ID_SD.C only - Frequency is about 1193182Hz/spkVal
void BE_ST_PCSpeakerOn(uint16_t spkVal);
void BE_ST_PCSpeakerOff(void);
// Safe alternatives for Borland's sound and nosound functions from Catacomb Abyss' gelib.c
void BE_ST_BSound(uint16_t frequency);
void BE_ST_BNoSound(void);
// Drop-in replacement for id_sd.c:alOut
void BE_ST_ALOut(uint8_t reg,uint8_t val);
// Here, the actual rate is about 1193182Hz/speed
// NOTE: isALMusicOn is irrelevant for Keen Dreams (even with its music code)
void BE_ST_SetTimer(uint16_t speed, bool isALMusicOn);
uint32_t BE_ST_GetTimeCount(void);
void BE_ST_SetTimeCount(uint32_t newcount);
// Use this as a replacement for busy loops waiting for some ticks to pass
// e.g., "while (TimeCount-srctimecount<timetowait)"
void BE_ST_TimeCountWaitFromSrc(uint32_t srctimecount, int16_t timetowait);
// Same as above, but instead waits to reach dsttimecount
// e.g., a replacement for "while (TimeCount<dsttimecount)"
void BE_ST_TimeCountWaitForDest(uint32_t dsttimecount);
void BE_ST_ShortSleep(void);
void BE_ST_ShutdownAll(void);

#endif
