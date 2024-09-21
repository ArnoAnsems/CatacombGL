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

#include "../audio/be_audio_mixer.h"
#include "../threading/be_atomic.h"
//#include "../video/be_video.h"
#include "be_st.h"
#include "be_st_sdl_private.h"
#include "be_timing.h"

//extern bool g_sdlForceGfxControlUiRefresh;

static uint32_t g_sdlTicksOffset = 0;

extern uint64_t g_sdlScaledSamplesPerPartsTimesPITRate;
extern uint64_t g_sdlScaledSamplesInCurrentPart;

#if (defined BE_ST_FILL_AUDIO_IN_MAIN_THREAD) && (defined BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO)
#error "Two incompatible macros are defined"
#endif

#if (defined BE_ST_FILL_AUDIO_IN_MAIN_THREAD) || (defined BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO)
static int g_sdlTimerIntCounter = 0;
#else
static BE_ST_AtomicInt_T g_sdlTimerIntCounter = {0};
#endif

int BE_ST_SET_TIMER_INT_COUNTER_SET(int x);
int BE_ST_SET_TIMER_INT_COUNTER_GET(void);
int BE_ST_SET_TIMER_INT_COUNTER_ADD(int x);
int BE_ST_SET_TIMER_INT_COUNTER_INC(void);

#ifdef BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO
static uint32_t g_pitTimerScaledDivisor;
static uint32_t g_sdlLastTicksForIntCounter;
static uint64_t g_sdlScaledTicksRemainder;

static void BEL_ST_RefreshTimerIntCounter(void)
{
	uint32_t ticks = BEL_ST_GetTicksMS();
	uint64_t scaledAccuTicks =
		(ticks - g_sdlLastTicksForIntCounter) * (uint64_t)PC_PIT_RATE
		+ g_sdlScaledTicksRemainder;
	uint64_t ratio = scaledAccuTicks / g_pitTimerScaledDivisor;
	g_sdlScaledTicksRemainder = scaledAccuTicks % g_pitTimerScaledDivisor;
	g_sdlTimerIntCounter += ratio;
	g_sdlLastTicksForIntCounter = ticks;
}
#else
#define BEL_ST_RefreshTimerIntCounter()
#endif

#if (defined BE_ST_FILL_AUDIO_IN_MAIN_THREAD) || (defined BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO)

int BE_ST_SET_TIMER_INT_COUNTER_SET(int x)
{
	BEL_ST_RefreshTimerIntCounter();
	int ret = g_sdlTimerIntCounter;
	g_sdlTimerIntCounter = x;
	return ret;
}

int BE_ST_SET_TIMER_INT_COUNTER_GET(void)
{
	BEL_ST_RefreshTimerIntCounter();
	return g_sdlTimerIntCounter;
}

int BE_ST_SET_TIMER_INT_COUNTER_ADD(int x)
{
	BEL_ST_RefreshTimerIntCounter();
	int ret = g_sdlTimerIntCounter;
	g_sdlTimerIntCounter += x;
	return ret;
}

int BE_ST_SET_TIMER_INT_COUNTER_INC(void) { return g_sdlTimerIntCounter++; }

#else

int BE_ST_SET_TIMER_INT_COUNTER_SET(int x) { return BE_ST_AtomicInt_Set(&g_sdlTimerIntCounter, x); }
int BE_ST_SET_TIMER_INT_COUNTER_GET(void) { return BE_ST_AtomicInt_Get(&g_sdlTimerIntCounter); }
int BE_ST_SET_TIMER_INT_COUNTER_ADD(int x) { return BE_ST_AtomicInt_PostAdd(&g_sdlTimerIntCounter, x); }
int BE_ST_SET_TIMER_INT_COUNTER_INC(void) { return BE_ST_AtomicInt_PostInc(&g_sdlTimerIntCounter); }

#endif

void BE_ST_InitTiming(void)
{
	g_sdlTicksOffset = 0;
#ifdef BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO
	g_pitTimerScaledDivisor = 1000 * 65536;
	g_sdlLastTicksForIntCounter = BEL_ST_GetTicksMS();
#endif
}

void BE_ST_SetTimer(uint16_t rateVal)
{
	BE_ST_LockAudioRecursively(); // RECURSIVE lock

	// Note that 0 should be interpreted as 65536
#ifdef BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO
	BEL_ST_RefreshTimerIntCounter();
	g_pitTimerScaledDivisor = 1000 * (rateVal ? rateVal : 65536);
#endif
	BEL_ST_AudioMixerUpdateFromPITRateWord(rateVal ? rateVal : 65536);

	BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
}

static void BEL_ST_TicksDelayWithOffset(int sdltickstowait)
{
	if (sdltickstowait <= (int32_t)g_sdlTicksOffset)
	{
		// Already waited for this time earlier, no need to do so now
		if (sdltickstowait > 0)
		{
			g_sdlTicksOffset -= sdltickstowait;
		}
		//BE_ST_PollEvents(); // Still safer to do this
		return;
	}
	uint32_t nextSdlTicks = BEL_ST_GetTicksMS() + sdltickstowait - g_sdlTicksOffset;
	//BEL_ST_UpdateHostDisplay();
	//BE_ST_PollEvents();
	uint32_t currSdlTicks = BEL_ST_GetTicksMS();
	uint32_t lastRefreshTime = currSdlTicks;
	while ((int32_t)(currSdlTicks - nextSdlTicks) < 0)
	{
		BEL_ST_SleepMS(1);
		//BE_ST_PollEvents();
		currSdlTicks = BEL_ST_GetTicksMS();
		// Refresh graphics from time to time in case a part of the window is overridden by anything,
		// like the Steam Overlay, but also check if we should refresh the graphics more often
//		if (g_sdlForceGfxControlUiRefresh || (currSdlTicks - lastRefreshTime > 100))
//		{
//			BEL_ST_UpdateHostDisplay();
//			currSdlTicks = BEL_ST_GetTicksMS(); // Just be a bit more pedantic
//			lastRefreshTime = currSdlTicks;
//		}
	} 
	g_sdlTicksOffset = (currSdlTicks - nextSdlTicks);
}

void BE_ST_WaitForNewVerticalRetraces(int16_t number)
{
	// TODO (REFKEEN) Make a difference based on HW?

	// Simulate waiting while in vertical retrace first, and then
	// waiting while NOT in vertical retrace. In practice, we jump
	// to the very beginning of the next "refresh cycle".
	// This is repeated for a total of 'length' times.

	uint32_t currSdlTicks = BEL_ST_GetTicksMS();
	uint32_t nextSdlTicks = (int)number*1000000/70086 + currSdlTicks - g_sdlTicksOffset;

	// First iteration takes a bit less time again, so we want
	// the value "mod" about 1000/70.086 (VGA adapter);
	// For the sake of a bit better precision we further multiply by 1000

	// FIXME: The way it's done here is wrong, and leads to faster fades
	// in Wolfenstein 3D v1.4. Let's disable this for now.
//	nextSdlTicks -= (((uint64_t)1000*(uint64_t)nextSdlTicks) % ((uint64_t)1000000000/70086))/1000;
	g_sdlTicksOffset = 0; // Can reset this, taking g_sdlTicksOffset into account above
	BEL_ST_TicksDelayWithOffset(nextSdlTicks-currSdlTicks);
}

// Call during a busy loop of some unknown duration (e.g., waiting for key press/release)
void BE_ST_ShortSleep(void)
{
	BEL_ST_SleepMS(1);
	// TODO: Make this more efficient
	//BEL_ST_UpdateHostDisplay();
	//BE_ST_PollEvents();
}


void BE_ST_Delay(uint16_t msec) // Replacement for delay from dos.h
{
	BEL_ST_TicksDelayWithOffset(msec);
}

// Resets to 0 an internal counter of calls to timer interrupt,
// and returns the original counter's value
int BE_ST_TimerIntClearLastCalls(void)
{
	return BE_ST_SET_TIMER_INT_COUNTER_SET(0);
}

static int g_sdlTimerIntCounterOffset = 0;

// Attempts to wait for a given amount of calls to timer interrupt.
// It may wait a bit more in practice (e.g., due to Sync to VBlank).
// This is taken into account into a following call to the same function,
// which may actually be a bit shorter than requested (as a consequence).
void BE_ST_TimerIntCallsDelayWithOffset(int nCalls)
{
	if (nCalls <= g_sdlTimerIntCounterOffset)
	{
		// Already waited for this time earlier, no need to do so now
		if (nCalls > 0)
		{
			g_sdlTimerIntCounterOffset -= nCalls;
			BE_ST_SET_TIMER_INT_COUNTER_SET(0);
		}
		//BE_ST_PollEvents(); // Still safer to do this
		return;
	}

	// Call this BEFORE updating host display or doing anything else!!!
	// (Because of things like VSync which may add their own delays)
	int oldCount = BE_ST_SET_TIMER_INT_COUNTER_ADD(g_sdlTimerIntCounterOffset);
	int newCount;

	//BEL_ST_UpdateHostDisplay();
	//BE_ST_PollEvents();
	uint32_t currSdlTicks;
	uint32_t lastRefreshTime = BEL_ST_GetTicksMS();

	do
	{
		BEL_ST_SleepMS(1);
		//BE_ST_PollEvents();
		currSdlTicks = BEL_ST_GetTicksMS();
		// Refresh graphics from time to time in case a part of the window is overridden by anything,
		// like the Steam Overlay, but also check if we should refresh the graphics more often
//		if (g_sdlForceGfxControlUiRefresh || (currSdlTicks - lastRefreshTime > 100))
//		{
//			BEL_ST_UpdateHostDisplay();
//			currSdlTicks = BEL_ST_GetTicksMS(); // Just be a bit more pedantic
//			lastRefreshTime = currSdlTicks;
//		}

		newCount = BE_ST_SET_TIMER_INT_COUNTER_GET();
	}
	while (newCount - oldCount < nCalls);
	// Do go through BE_ST_SET_TIMER_INT_COUNTER_SET instead of directly
	// accessing 'newCount', in case counter has just been updated again
	g_sdlTimerIntCounterOffset = (BE_ST_SET_TIMER_INT_COUNTER_SET(0) - oldCount) - nCalls;
}
