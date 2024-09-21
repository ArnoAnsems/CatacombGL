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

void BE_ST_InitAudio(void);

 /*** Audio/timer (vanilla Keen kind-of has these mixed) ***/
void BE_ST_StartAudioAndTimerInt(void (*funcPtr)(void));
void BE_ST_StopAudioAndTimerInt(void);
void BE_ST_StartDigiAudioInt(void (*funcPtr)(void));
void BE_ST_StopDigiAudioInt(void);
void BE_ST_LockAudioRecursively(void);
void BE_ST_UnlockAudioRecursively(void);
bool BE_ST_IsEmulatedOPLChipReady(void);
bool BE_ST_IsEmulatedSBReady(void);
bool BE_ST_IsEmulatedSBProReady(void);
// WARNING about using BE_ST_PCSpeaker*:
//
// You MUST call BE_ST_LockAudioRecursively before calling any of these
// functions. Afterwards, you MUST call BE_ST_UnlockAudioRecursively.
// If you don't, unexpected behaviors may be reproduced
// (e.g., an audio callback thread hang).
//
// For BE_ST_PCSpeakerSetInvFreq: The frequency is about 1193182Hz/spkInvFreq.
// To turn the PC Speaker off, call BE_ST_PCSpeakerSetConstVal(0).
void BE_ST_PCSpeakerSetInvFreq(uint16_t spkInvFreq);
void BE_ST_PCSpeakerSetConstVal(bool isUp);
// Used for playback from digitized sound data with
// unsigned 8-bit or signed 16-bit int sample format.
// Do NOT assume the data is copied; You ***must*** call BE_ST_StopSoundEffect.
// You can also use BE_ST_StartAudioAndTimerInt to set a callback function,
// to be called when reading of sound data is complete. This can happen
// a bit before actual sound playback is complete, in case
// some mechanism of resampling is in use.
void BE_ST_SetDigiSoundFreq(int freq);
static inline void BE_ST_SetDigiSoundFreqFromSBTimeValue(uint8_t timevalue)
{
	BE_ST_SetDigiSoundFreq(1000000 / (256 - timevalue));
}

// This is used for setting digitized sound volumes for
// stereo panning, as done for the SB Pro or SB 16 in Wolf3D.
// Two 4-bit volumes in the range 0-15 should be packed into a byte.
void BE_ST_SetDigiSBProSoundVolumes(uint8_t volBits);

// Same but for getting the volumes
uint8_t BE_ST_GetDigiSBProSoundVolumes(void);

// Similar functions, but for the SB Pro's FM synthesis.
void BE_ST_SetFMSBProSoundVolumes(uint8_t volBits);
uint8_t BE_ST_GetFMSBProSoundVolumes(void);

// Compatibility macro that was originally added for Super 3-D Noah's Ark
#define BE_ST_SetLineInSBProSoundVolumes(volBits)

void BE_ST_PlaySoundEffect(void* data, int numOfSamples, int bits);
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

void BE_ST_InitTiming(void);
void BEL_ST_SetConfig(void);
void BE_ST_ShutdownAudio(void);

// MUST be included here (since be_st_cfg.h depends on be_st.h)
#include "be_st_cfg.h"

#endif // BE_ST_H
