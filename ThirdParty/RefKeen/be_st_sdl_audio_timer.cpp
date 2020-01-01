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

#include <stdlib.h>
#include <string.h>
#include "..\SDL\include\SDL.h"
#include "be_st.h"
#include "../opl/dbopl.h"

#define PC_PIT_RATE 1193182

static SDL_AudioSpec g_sdlAudioSpec;
static SDL_AudioCallback g_sdlOurAudioCallback;
/*static*/ SDL_AudioDeviceID g_sdlAudioDevice;

bool g_sdlAudioSubsystemUp;

static bool g_sdlEmulatedOPLChipReady;
static uint32_t g_sdlSamplesPartNum = 0;
// Simple callback: Scale is samples*PC_PIT_RATE
// Resampling callback (with OPL emulation): Scale is samples*PC_PIT_RATE*OPL_SAMPLE_RATE
static uint64_t g_sdlScaledSamplesPerPartsTimesPITRate = 0;
static uint64_t g_sdlScaledSampleOffsetInSound = 0;
static uint64_t g_sdlScaledSamplesInCurrentPart = 0;

static void(*g_sdlTimerIntFuncPtr)(void) = 0;

static SDL_atomic_t g_sdlTimerIntCounter = { 0 };
#define BE_ST_SET_TIMER_INT_COUNTER_SET(x) SDL_AtomicSet(&g_sdlTimerIntCounter, (x))
#define BE_ST_SET_TIMER_INT_COUNTER_ADD(x) SDL_AtomicAdd(&g_sdlTimerIntCounter, (x))
#define BE_ST_SET_TIMER_INT_COUNTER_INC() SDL_AtomicAdd(&g_sdlTimerIntCounter, 1)
#define BE_ST_SET_TIMER_INT_COUNTER_GET() SDL_AtomicGet(&g_sdlTimerIntCounter)

#define OPL_SAMPLE_RATE 49716
// Use this if the audio subsystem is disabled for most (we want a BYTES rate of 1000Hz, same units as used in values returned by SDL_GetTicks())
#define NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM 1000

typedef int16_t BE_ST_SndSample_T;

// This is used if the sound subsystem is disabled, *or* if it's enabled and BE_ST_FILL_AUDIO_IN_MAIN_THREAD is defined.
//
// If enabled, this buffer is actually split into two subbuffers:
// One for main thread use, the other being shared with the audio callback thread.
static BE_ST_SndSample_T *g_sdlCallbacksSamplesBuffer;
// If two sub-buffers are used, this is the size of a single one
static uint32_t g_sdlCallbacksSamplesBufferOnePartCount;

/*static */uint32_t g_sdlManualAudioCallbackCallLastTicks;
static uint32_t g_sdlManualAudioCallbackCallDelayedSamples;

// Used for filling with samples from BE_ST_OPL2Write,
// in addition to the SDL audio CallBack itself
// (because waits between/after OPL writes are expected)
static BE_ST_SndSample_T *g_sdlALOutSamples;
static uint32_t g_sdlALOutNumOfSamples;
static uint32_t g_sdlALOutSamplesEnd = 0;

// PC Speaker current status
static bool g_sdlPCSpeakerOn = false;
static BE_ST_SndSample_T g_sdlCurrentBeepSample;
static uint32_t g_sdlBeepHalfCycleCounter, g_sdlBeepHalfCycleCounterUpperBound;

static void BEL_ST_Simple_EmuCallBack(void *unused, Uint8 *stream, int len);

static void YM3812Init(int numChips, int clock, int rate);

uint32_t BE_Cross_TypedMin32(uint32_t a, uint32_t b)
{
    return (a < b) ? a : b;
}

uint64_t BE_Cross_TypedMin64(uint64_t a, uint64_t b)
{
    return (a < b) ? a : b;
}

void BE_ST_InitAudio(void)
{
    g_sdlAudioSubsystemUp = false;
    g_sdlEmulatedOPLChipReady = false;
    {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
        {
            //BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "SDL audio system initialization failed,\n%s\n", SDL_GetError());
        }
        else
        {
            SDL_AudioSpec spec;
            spec.freq = 49716;
            spec.format = AUDIO_S16SYS;
            spec.channels = 1;
            // Should be some power-of-two roughly proportional to the sample rate; Using 1024 for 48000Hz.
            for (spec.samples = 1; spec.samples < 49716 / 64; spec.samples *= 2)
            {
            }
            g_sdlOurAudioCallback = BEL_ST_Simple_EmuCallBack;

            spec.callback = g_sdlOurAudioCallback;

            spec.userdata = NULL;
            //BE_Cross_LogMessage(BE_LOG_MSG_NORMAL, "Initializing audio subsystem, requested spec: freq %d, format %u, channels %d, samples %u; size is set to %u\n", (int)spec.freq, (unsigned int)spec.format, (int)spec.channels, (unsigned int)spec.samples, (unsigned int)spec.size);
            g_sdlAudioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, &g_sdlAudioSpec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
            if (g_sdlAudioDevice <= 0)
            {
                //BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "Cannot open SDL audio device,\n%s\n", SDL_GetError());
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
            }
            else
            {
                {
                    //BE_Cross_LogMessage(BE_LOG_MSG_NORMAL, "Audio subsystem initialized, received spec: freq %d, format %u, channels %d, samples %u, size %u\n", (int)g_sdlAudioSpec.freq, (unsigned int)g_sdlAudioSpec.format, (int)g_sdlAudioSpec.channels, (unsigned int)g_sdlAudioSpec.samples, (unsigned int)g_sdlAudioSpec.size);
                    g_sdlAudioSubsystemUp = true;
                }
            }
        }
    }

    // If the audio subsystem is off, let us simulate a byte rate
    // of 1000Hz (same as SDL_GetTicks() time units)
    if (!g_sdlAudioSubsystemUp)
    {
        g_sdlAudioSpec.freq = (NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM / sizeof(BE_ST_SndSample_T));
        g_sdlOurAudioCallback = BEL_ST_Simple_EmuCallBack;

        g_sdlCallbacksSamplesBuffer = (BE_ST_SndSample_T *)malloc(NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM);
//        if (!g_sdlCallbacksSamplesBuffer)
//            BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: Out of memory! (Failed to allocate g_sdlCallbacksSamplesBuffer.)");
        g_sdlCallbacksSamplesBufferOnePartCount = NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM / sizeof(BE_ST_SndSample_T);

        goto finish;
    }

    //if (g_refKeenCfg.oplEmulation)
    {
        YM3812Init(1, 3579545, OPL_SAMPLE_RATE);
        g_sdlEmulatedOPLChipReady = true;
        g_sdlALOutNumOfSamples = 2 * (2 * g_sdlAudioSpec.samples*OPL_SAMPLE_RATE / g_sdlAudioSpec.freq); // Leave some room for calls to BE_ST_OPL2Write
        g_sdlALOutSamples = (BE_ST_SndSample_T *)malloc(sizeof(BE_ST_SndSample_T) * g_sdlALOutNumOfSamples);
//        if (g_sdlALOutSamples == NULL)
//            BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: Out of memory! (Failed to allocate g_sdlALOutSamples.)");
    }

    // As stated above, BE_ST_BSound may be called,
    // so better start generation of samples
    SDL_PauseAudioDevice(g_sdlAudioDevice, 0);

finish:
    // Regardless of the audio subsystem being off or on, have *some*
    // rate set (being ~18.2Hz). In DEMOCAT from The Catacomb Abyss v1.13,
    // BE_ST_BSound may be called, so be ready to generate samples.
    BE_ST_SetTimer(0);

    g_sdlManualAudioCallbackCallLastTicks = SDL_GetTicks();
    g_sdlManualAudioCallbackCallDelayedSamples = 0;
}

static uint32_t g_sdlTicksOffset = 0;

void BE_ST_InitTiming(void)
{
    g_sdlTicksOffset = 0;
}

void BE_ST_ShutdownAudio(void)
{
    if (g_sdlAudioSubsystemUp)
    {
        SDL_PauseAudioDevice(g_sdlAudioDevice, 1);
        SDL_CloseAudioDevice(g_sdlAudioDevice);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        g_sdlAudioSubsystemUp = false;
    }

    free(g_sdlCallbacksSamplesBuffer);
    g_sdlCallbacksSamplesBuffer = NULL;
    free(g_sdlALOutSamples);
    g_sdlALOutSamples = NULL;

    g_sdlTimerIntFuncPtr = 0; // Just in case this may be called after the audio subsystem was never really started (manual calls to callback)
}

void BE_ST_StartAudioAndTimerInt(void(*funcPtr)(void))
{
    BE_ST_LockAudioRecursively();

    g_sdlTimerIntFuncPtr = funcPtr;
    BE_ST_SET_TIMER_INT_COUNTER_SET(0);

    BE_ST_UnlockAudioRecursively();
}

void BE_ST_StopAudioAndTimerInt(void)
{
    BE_ST_LockAudioRecursively();

    g_sdlTimerIntFuncPtr = 0;

    BE_ST_UnlockAudioRecursively();
}

void BE_ST_LockAudioRecursively(void)
{
}

void BE_ST_UnlockAudioRecursively(void)
{
}

// Use this ONLY if audio subsystem isn't properly started up
void BE_ST_PrepareForManualAudioCallbackCall(void)
{
    uint32_t currTicks = SDL_GetTicks();

    // If e.g., we call this function from BE_ST_PrepareForGameStartupWithoutAudio
    if (!g_sdlOurAudioCallback)
        return;

    if (currTicks == g_sdlManualAudioCallbackCallLastTicks)
        return;

    // Using g_sdlAudioSpec.req as the rate, we (generally) lose precision in the following division,
    // so we use g_sdlManualAudioCallbackCallDelayedSamples to accumulate lost samples.
    uint64_t dividend = ((uint64_t)g_sdlAudioSpec.freq)*(currTicks - g_sdlManualAudioCallbackCallLastTicks) + g_sdlManualAudioCallbackCallDelayedSamples;
    uint32_t samplesPassed = static_cast<uint32_t>(dividend / 1000);
    g_sdlManualAudioCallbackCallDelayedSamples = dividend % 1000;

    uint32_t samplesToProcess = samplesPassed;
    // Buffer has some constant size, so loop if required (which may hint at an overflow)
    for (; samplesToProcess >= g_sdlCallbacksSamplesBufferOnePartCount; samplesToProcess -= g_sdlCallbacksSamplesBufferOnePartCount)
        g_sdlOurAudioCallback(NULL, (Uint8 *)g_sdlCallbacksSamplesBuffer, g_sdlCallbacksSamplesBufferOnePartCount * sizeof(BE_ST_SndSample_T));
    if (samplesToProcess > 0)
        g_sdlOurAudioCallback(NULL, (Uint8 *)g_sdlCallbacksSamplesBuffer, samplesToProcess * sizeof(BE_ST_SndSample_T));
    g_sdlManualAudioCallbackCallLastTicks = currTicks;
}

bool BE_ST_IsEmulatedOPLChipReady(void)
{
    return g_sdlEmulatedOPLChipReady;
}

// Frequency is about 1193182Hz/spkVal
void BE_ST_PCSpeakerOn(uint16_t spkVal)
{
    g_sdlPCSpeakerOn = true;
    g_sdlCurrentBeepSample = 0;
    g_sdlBeepHalfCycleCounter = 0;
    g_sdlBeepHalfCycleCounterUpperBound = g_sdlAudioSpec.freq * spkVal;
}

void BE_ST_PCSpeakerOff(void)
{
    g_sdlPCSpeakerOn = false;
}

void BE_ST_BSound(uint16_t frequency)
{
    BE_ST_LockAudioRecursively();
    BE_ST_PCSpeakerOn(PC_PIT_RATE / (uint32_t)frequency);
    BE_ST_UnlockAudioRecursively();
}

void BE_ST_BNoSound(void)
{
    BE_ST_LockAudioRecursively();
    BE_ST_PCSpeakerOff();
    BE_ST_UnlockAudioRecursively();
}

/*******************************************************************************
OPL emulation, powered by dbopl from DOSBox and using bits of code from Wolf4SDL
*******************************************************************************/

static Chip oplChip;

static inline void YM3812Init(int numChips, int clock, int rate)
{
    DBOPL_InitTables();
    Chip__Chip(&oplChip);
    Chip__Setup(&oplChip, rate);
}

static inline void YM3812Write(Chip *which, Bit32u reg, Bit8u val)
{
    Chip__WriteReg(which, reg, val);
}

#define OPL_NUM_OF_SAMPLES 2048 // About 40ms of OPL sound data

static inline void YM3812UpdateOne(Chip *which, BE_ST_SndSample_T *stream, int length)
{
    Bit32s buffer[OPL_NUM_OF_SAMPLES * 2];
    int i;

    // length should be at least the max. samplesPerMusicTick
    // in Catacomb 3-D and Keen 4-6, which is param_samplerate / 700.
    // So 512 is sufficient for a sample rate of 358.4 kHz, which is
    // significantly higher than the OPL rate anyway.
    if (length > OPL_NUM_OF_SAMPLES)
        length = OPL_NUM_OF_SAMPLES;

    Chip__GenerateBlock2(which, length, buffer);

    // GenerateBlock2 generates a number of "length" 32-bit mono samples
    // so we only need to convert them to 16-bit mono samples
    for (i = 0; i < length; i++)
    {
        // Scale volume
        Bit32s sample = 2 * buffer[i];
        if (sample > 16383) sample = 16383;
        else if (sample < -16384) sample = -16384;
        stream[i] = sample;
    }
}

void BE_ST_OPL2Write(uint8_t reg, uint8_t val)
{
    BE_ST_LockAudioRecursively(); // RECURSIVE lock

    // Per the AdLib manual, this function should simulate 6 reads
    // of the register port after writing to it (3.3 microseconds),
    // and then 35 more reads of the register port after
    // writing to the data port (23 microseconds).
    //
    // The above appears to be important for reproduction
    // of the fuse breakage sound in Keen 5 at the least,
    // as well as a few sound effects in The Catacomb Abyss
    // (hitting a locked gate, teleportation sound effect).
    YM3812Write(&oplChip, reg, val);
    // FIXME: For now we roughly simulate the above delays with a
    // hack, using a "magic number" that appears to make this work.
    unsigned int length = OPL_SAMPLE_RATE / 10000;

    if (length > g_sdlALOutNumOfSamples - g_sdlALOutSamplesEnd)
    {
        //BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "BE_ST_OPL2Write overflow, want %u, have %u\n", length, g_sdlALOutNumOfSamples - g_sdlALOutSamplesEnd); // FIXME - Other thread
        length = g_sdlALOutNumOfSamples - g_sdlALOutSamplesEnd;
    }
    if (length)
    {
        YM3812UpdateOne(&oplChip, &g_sdlALOutSamples[g_sdlALOutSamplesEnd], length);
        g_sdlALOutSamplesEnd += length;
    }

    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
}

/**********************************************************************
PC Speaker emulation; The function adds audio data to the stream.
ASSUMPTION: The speaker is outputting sound (g_sdlPCSpeakerOn == true).
**********************************************************************/
static inline void PCSpeakerUpdateOne(BE_ST_SndSample_T *stream, int length)
{
    for (int loopVar = 0; loopVar < length; loopVar++, stream++)
    {
        *stream = g_sdlCurrentBeepSample;
        g_sdlBeepHalfCycleCounter += 2 * PC_PIT_RATE;
        if (g_sdlBeepHalfCycleCounter >= g_sdlBeepHalfCycleCounterUpperBound)
        {
            g_sdlBeepHalfCycleCounter %= g_sdlBeepHalfCycleCounterUpperBound;
            // Changing between 0 and max. possible value - too loud
            g_sdlCurrentBeepSample ^= 24575;
        }
    }
}

// WARNING: Possibly the wrong place to call the OPL emulator,
// but otherwise a separate dedicated thread may be required
static void BEL_ST_Simple_EmuCallBack(void *unused, Uint8 *stream, int len)
{
    BE_ST_SndSample_T *currSamplePtr = (BE_ST_SndSample_T *)stream;
    memset(stream, 0, len);
    /////////////////////////////
    BE_ST_LockAudioRecursively(); // RECURSIVE lock
    /////////////////////////////

    while (len)
    {
        if (!g_sdlScaledSampleOffsetInSound)
        {
            // FUNCTION VARIABLE (We should use this and we want to kind-of separate what we have here from original code.)
            if (g_sdlTimerIntFuncPtr)
            {
                g_sdlTimerIntFuncPtr();
                BE_ST_SET_TIMER_INT_COUNTER_INC();
            }
        }
        // Now generate sound
        uint32_t currNumOfSamples = BE_Cross_TypedMin32(len / sizeof(BE_ST_SndSample_T), static_cast<uint32_t>(g_sdlScaledSamplesInCurrentPart - g_sdlScaledSampleOffsetInSound));
        // PC Speaker
        if (g_sdlPCSpeakerOn)
            PCSpeakerUpdateOne(currSamplePtr, currNumOfSamples);
        /*** AdLib (including hack for BE_ST_OPL2Write delays) ***/
        if (g_sdlEmulatedOPLChipReady)
        {
            // We may have pending AL data ready, but probably less than required
            // for filling the stream buffer, so generate some silence.
            //
            // Make sure we don't overthrow the AL buffer, though.
            uint32_t targetALSamples = currNumOfSamples;
            if (targetALSamples > g_sdlALOutNumOfSamples)
            {
                //BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "BEL_ST_Simple_EmuCallBack AL overflow, want %u, have %u\n", targetALSamples, g_sdlALOutNumOfSamples); // FIXME - Other thread
                targetALSamples = g_sdlALOutNumOfSamples;
            }
            // TODO Output overflow warning if there's any
            if (targetALSamples > g_sdlALOutSamplesEnd)
            {
                YM3812UpdateOne(&oplChip, &g_sdlALOutSamples[g_sdlALOutSamplesEnd], targetALSamples - g_sdlALOutSamplesEnd);
                g_sdlALOutSamplesEnd = targetALSamples;
            }
            // Mix with AL data
            for (uint32_t i = 0; i < targetALSamples; ++i)
                currSamplePtr[i] = (currSamplePtr[i] + g_sdlALOutSamples[i]) / 2;
            // Move pending AL data
            if (targetALSamples >= g_sdlALOutSamplesEnd)
            {
                g_sdlALOutSamplesEnd = 0;
            }
            else if (targetALSamples < g_sdlALOutSamplesEnd)
            {
                memmove(g_sdlALOutSamples, &g_sdlALOutSamples[targetALSamples], sizeof(BE_ST_SndSample_T) * (g_sdlALOutSamplesEnd - targetALSamples));
                g_sdlALOutSamplesEnd -= targetALSamples;
            }
        }
        // We're done for now
        currSamplePtr += currNumOfSamples;
        g_sdlScaledSampleOffsetInSound += currNumOfSamples;
        len -= sizeof(BE_ST_SndSample_T)*currNumOfSamples;
        // End of part?
        if (g_sdlScaledSampleOffsetInSound >= g_sdlScaledSamplesInCurrentPart)
        {
            g_sdlScaledSampleOffsetInSound = 0;
            if (++g_sdlSamplesPartNum == PC_PIT_RATE)
                g_sdlSamplesPartNum = 0;
            g_sdlScaledSamplesInCurrentPart = (g_sdlSamplesPartNum + 1) * g_sdlScaledSamplesPerPartsTimesPITRate / PC_PIT_RATE - g_sdlSamplesPartNum * g_sdlScaledSamplesPerPartsTimesPITRate / PC_PIT_RATE;
        }
    }

    ///////////////////////////////
    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
    ///////////////////////////////
}

void BE_ST_SetTimer(uint16_t rateVal)
{
    BE_ST_LockAudioRecursively(); // RECURSIVE lock

    // Note that 0 should be interpreted as 65536
    g_sdlScaledSamplesPerPartsTimesPITRate = (rateVal ? rateVal : 65536) * g_sdlAudioSpec.freq;
    // Since the following division may lead to truncation, g_sdlScaledSamplesInCurrentPart
    // can change during playback by +-1 (otherwise music may be a bit faster than intended).
    g_sdlScaledSamplesInCurrentPart = g_sdlScaledSamplesPerPartsTimesPITRate / PC_PIT_RATE;

    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
}

static void BEL_ST_TicksDelayWithOffset(int sdltickstowait);

// Call during a busy loop of some unknown duration (e.g., waiting for key press/release)
void BE_ST_ShortSleep(void)
{
    SDL_Delay(1);
}

void BE_ST_Delay(uint16_t msec) // Replacement for delay from dos.h
{
    BEL_ST_TicksDelayWithOffset(msec);
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
        return;
    }
    uint32_t nextSdlTicks = SDL_GetTicks() + sdltickstowait - g_sdlTicksOffset;
    uint32_t currSdlTicks = SDL_GetTicks();
    uint32_t lastRefreshTime = currSdlTicks;
    while ((int32_t)(currSdlTicks - nextSdlTicks) < 0)
    {
        SDL_Delay(1);
        currSdlTicks = SDL_GetTicks();
        // Refresh graphics from time to time in case a part of the window is overridden by anything,
        // like the Steam Overlay, but also check if we should refresh the graphics more often
        if (currSdlTicks - lastRefreshTime > 100)
        {
            currSdlTicks = SDL_GetTicks(); // Just be a bit more pedantic
            lastRefreshTime = currSdlTicks;
        }
    }
    g_sdlTicksOffset = (currSdlTicks - nextSdlTicks);
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
        return;
    }

    // Call this BEFORE updating host display or doing anything else!!!
    // (Because of things like VSync which may add their own delays)
    int oldCount = BE_ST_SET_TIMER_INT_COUNTER_ADD(g_sdlTimerIntCounterOffset);
    int newCount;
    uint32_t currSdlTicks;
    uint32_t lastRefreshTime = SDL_GetTicks();

    do
    {
        SDL_Delay(1);
        currSdlTicks = SDL_GetTicks();
        // Refresh graphics from time to time in case a part of the window is overridden by anything,
        // like the Steam Overlay, but also check if we should refresh the graphics more often
        if (currSdlTicks - lastRefreshTime > 100)
        {
            currSdlTicks = SDL_GetTicks(); // Just be a bit more pedantic
            lastRefreshTime = currSdlTicks;
        }

        newCount = BE_ST_SET_TIMER_INT_COUNTER_GET();
    } while (newCount - oldCount < nCalls);
    // Do call SDL_AtomicSet instead of accessing 'newCount', in case counter has just been updated again
    g_sdlTimerIntCounterOffset = (BE_ST_SET_TIMER_INT_COUNTER_SET(0) - oldCount) - nCalls;
}
