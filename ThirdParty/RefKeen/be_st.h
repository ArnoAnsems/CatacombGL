/* Copyright (C) 2014-2019 NY00123
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

#ifndef BE_ST_H
#define BE_ST_H

#include <stdbool.h>
#include <stdint.h>

/*** General ***/
void BE_ST_InitAudio(void);
void BE_ST_ShutdownAudio(void);

/*** Audio/timer (vanilla Keen kind-of has these mixed) ***/
void BE_ST_StartAudioAndTimerInt(void(*funcPtr)(void));
void BE_ST_StopAudioAndTimerInt(void);
void BE_ST_LockAudioRecursively(void);
void BE_ST_UnlockAudioRecursively(void);
bool BE_ST_IsEmulatedOPLChipReady(void);
// WARNING about using BE_ST_PCSpeakerOn/Off:
//
// You MUST call BE_ST_LockAudioRecursively before calling any of these
// functions. Afterwards, you MUST call BE_ST_UnlockAudioRecursively.
// If you don't, unexpected behaviors may be reproduced
// (e.g., an audio callback thread hang).
//
// The sound frequency is about 1193182Hz/spkVal.
void BE_ST_PCSpeakerOn(uint16_t spkVal);
void BE_ST_PCSpeakerOff(void);
// Used for playback from digitized sound data in signed 16-bit int format.
// Do NOT assume the data is copied; You ***must*** call BE_ST_StopSoundEffect.
// You can also use BE_ST_StartAudioAndTimerInt to set a callback function,
// to be called when reading of sound data is complete. This can happen
// a bit before actual sound playback is complete, in case
// some mechanism of resampling is in use.
void BE_ST_PlayS16SoundEffect(int16_t *data, int numOfSamples);
void BE_ST_StopSoundEffect(void);
// Safe alternatives for Borland's sound and nosound functions from Catacomb Abyss' gelib.c
void BE_ST_BSound(uint16_t frequency);
void BE_ST_BNoSound(void);
// Sends data to emulated OPL2/3 chip (e.g., AdLib), where "reg" is
// written to the address/status port and "val" is written to the data
// port. For the purpose of OPL emulation, this attempts to cover small
// delays (measured in microseconds) as given by the AdLib manual.
void BE_ST_OPL2Write(uint8_t reg, uint8_t val);
// Sets the rate in which the timer interrupt is called, to about
// 1193182Hz/rateVal. May also have an effect on sound generation.
void BE_ST_SetTimer(uint16_t rateVal);
// Resets to 0 an internal counter of calls to timer interrupt,
// and returns the original counter's value
int BE_ST_TimerIntClearLastCalls(void);
// Attempts to wait for a given amount of calls to timer interrupt.
// It may wait a bit more in practice (e.g., due to Sync to VBlank).
// This is taken into account into a following call to the same function,
// which may actually be a bit shorter than requested (as a consequence).
void BE_ST_TimerIntCallsDelayWithOffset(int nCalls);

void BE_ST_ShortSleep(void);
void BE_ST_Delay(uint16_t msec); // Replacement for delay from dos.h

#endif // BE_ST_H
