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

// Used for digitized sound playback
static int16_t *g_sdlSoundEffectCurrPtr;
static uint32_t g_sdlSoundEffectSamplesLeft;

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

// Nearest-neighborhood sample rate conversion, used as
// a simplistic alternative to any resampling library
static int *g_sdlSampleRateConvTable;
static int g_sdlSampleRateConvTableSize;
// Current location in the conversion process;
static int g_sdlSampleRateConvCurrIndex; // Index to g_sdlSampleRateConvTable
static int g_sdlSampleRateConvCounter; // Counter for current cell of g_sdlALSampleRateConvTable
BE_ST_SndSample_T g_sdlSampleRateConvLastValue; // Last input sample

// Used for filling with samples from BE_ST_OPL2Write,
// in addition to the SDL audio CallBack itself
// (because waits between/after OPL writes are expected)
static BE_ST_SndSample_T *g_sdlALOutSamples;
static uint32_t g_sdlALOutNumOfSamples;
static uint32_t g_sdlALOutSamplesEnd = 0;

// Used with resampling callback only
static BE_ST_SndSample_T *g_sdlMiscOutSamples;
static uint32_t g_sdlMiscOutNumOfSamples;
static uint32_t g_sdlMiscOutSamplesEnd = 0;

// PC Speaker current status
static bool g_sdlPCSpeakerOn = false;
static BE_ST_SndSample_T g_sdlCurrentBeepSample;
static uint32_t g_sdlBeepHalfCycleCounter, g_sdlBeepHalfCycleCounterUpperBound;

static void BEL_ST_Simple_EmuCallBack(void *unused, Uint8 *stream, int len);
static void BEL_ST_Resampling_EmuCallBack(void *unused, Uint8 *stream, int len);
static void BEL_ST_Simple_DigiCallBack(void *unused, Uint8 *stream, int len);
static void BEL_ST_Resampling_DigiCallBack(void *unused, Uint8 *stream, int len);

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
    int inSampleRate = 0;
    bool doDigitized = (inSampleRate != 0);
    if (!doDigitized)
        inSampleRate = OPL_SAMPLE_RATE;

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

            if (doDigitized)
                g_sdlOurAudioCallback = (49716 == inSampleRate) ? BEL_ST_Simple_DigiCallBack : BEL_ST_Resampling_DigiCallBack;
            else
                g_sdlOurAudioCallback = (49716 == inSampleRate) ? BEL_ST_Simple_EmuCallBack : BEL_ST_Resampling_EmuCallBack;

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
        g_sdlAudioSpec.freq = doDigitized ? inSampleRate : (NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM / sizeof(BE_ST_SndSample_T));
        g_sdlOurAudioCallback = doDigitized ? BEL_ST_Simple_DigiCallBack : BEL_ST_Simple_EmuCallBack;

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

    if ((doDigitized || g_sdlEmulatedOPLChipReady) && (g_sdlAudioSpec.freq != inSampleRate))
    {
        // Should allocate this first, for g_sdlSrcData.data_in
        g_sdlMiscOutNumOfSamples = 2 * g_sdlAudioSpec.samples;
        g_sdlMiscOutSamples = (BE_ST_SndSample_T *)malloc(sizeof(BE_ST_SndSample_T) * g_sdlMiscOutNumOfSamples);
//        if (g_sdlMiscOutSamples == NULL)
//            BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: Out of memory! (Failed to allocate g_sdlMiscOutSamples.)");


        {
            // The sum of all entries should be g_sdlAudioSpec.freq,
            // "uniformly" distributed over g_sdlALSampleRateConvTable
            g_sdlSampleRateConvTable = (int *)malloc(sizeof(int) * inSampleRate);
            //if (g_sdlSampleRateConvTable == NULL)
            //   BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: Failed to allocate memory for sample rate conversion!");
            g_sdlSampleRateConvTableSize = inSampleRate;
            for (int i = 0; i < inSampleRate; ++i)
            {
                // Using uint64_t cause an overflow is possible
                g_sdlSampleRateConvTable[i] = ((uint64_t)(i + 1)*(uint64_t)g_sdlAudioSpec.freq / inSampleRate) - (uint64_t)i*(uint64_t)g_sdlAudioSpec.freq / inSampleRate;
            }
            g_sdlSampleRateConvCurrIndex = 0;
            g_sdlSampleRateConvCounter = 0;
        }
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
        if ((g_sdlOurAudioCallback == BEL_ST_Resampling_EmuCallBack) || (g_sdlOurAudioCallback == BEL_ST_Resampling_DigiCallBack))
        {
            free(g_sdlSampleRateConvTable);
        }
        SDL_CloseAudioDevice(g_sdlAudioDevice);
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        g_sdlAudioSubsystemUp = false;
    }

    free(g_sdlCallbacksSamplesBuffer);
    g_sdlCallbacksSamplesBuffer = NULL;
    free(g_sdlALOutSamples);
    g_sdlALOutSamples = NULL;
    free(g_sdlMiscOutSamples);
    g_sdlMiscOutSamples = NULL;

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
    uint32_t samplesPassed = dividend / 1000;
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

/**************************
A common resampling wrapper
**************************/

static inline void BEL_ST_DoResample(uint32_t *outConsumed, uint32_t *outProduced, BE_ST_SndSample_T *inPtr, BE_ST_SndSample_T *outPtr, uint32_t numOfAvailInputSamples, uint32_t maxSamplesToOutput)
{
    // Just some type differences
    uint32_t samples_consumed, samples_produced;
    samples_consumed = samples_produced = 0;
    /*** Note: Casting to unsigned in order to suppress comparison-related warnings ***/
    while (((unsigned)samples_consumed < numOfAvailInputSamples) && ((unsigned)samples_produced < maxSamplesToOutput))
    {
        if (g_sdlSampleRateConvCounter)
        {
            outPtr[samples_produced++] = g_sdlSampleRateConvLastValue;
            --g_sdlSampleRateConvCounter;
        }
        else
        {
            g_sdlSampleRateConvLastValue = inPtr[samples_consumed++];
            g_sdlSampleRateConvCounter = g_sdlSampleRateConvTable[g_sdlSampleRateConvCurrIndex];
            g_sdlSampleRateConvCurrIndex = (g_sdlSampleRateConvCurrIndex + 1) % g_sdlSampleRateConvTableSize;
        }
    }
    *outConsumed = samples_consumed;
    *outProduced = samples_produced;
}


// WARNING: Possibly the wrong place to call the OPL emulator,
// but otherwise a separate dedicated thread may be required
static void BEL_ST_Simple_EmuCallBack(void *unused, Uint8 *stream, int len)
{
    BE_ST_SndSample_T *currSamplePtr = (BE_ST_SndSample_T *)stream;
    uint32_t currNumOfSamples;
#if SDL_VERSION_ATLEAST(1,3,0)
    memset(stream, 0, len);
#endif
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
        currNumOfSamples = BE_Cross_TypedMin32(len / sizeof(BE_ST_SndSample_T), g_sdlScaledSamplesInCurrentPart - g_sdlScaledSampleOffsetInSound);
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
            if (targetALSamples < g_sdlALOutSamplesEnd)
                memmove(g_sdlALOutSamples, &g_sdlALOutSamples[targetALSamples], sizeof(BE_ST_SndSample_T) * (g_sdlALOutSamplesEnd - targetALSamples));
            g_sdlALOutSamplesEnd -= targetALSamples;
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

// WARNING: Possibly the wrong place to call the OPL emulator *and* the
// resampler, but otherwise a separate dedicated thread (or two) may be required
static void BEL_ST_Resampling_EmuCallBack(void *unused, Uint8 *stream, int len)
{
    BE_ST_SndSample_T *currSamplePtr = (BE_ST_SndSample_T *)stream;
    uint64_t currNumOfScaledSamples;
    uint64_t scaledSamplesToGenerate = (uint64_t)(len / sizeof(BE_ST_SndSample_T)) * OPL_SAMPLE_RATE; // TODO consider lcm?
    uint64_t scaledSamplesToGenerateNextTime = (uint64_t)g_sdlAudioSpec.freq * OPL_SAMPLE_RATE / 100; // ~10ms
#if SDL_VERSION_ATLEAST(1,3,0)
    memset(stream, 0, len);
#endif
    /////////////////////////////
    BE_ST_LockAudioRecursively(); // RECURSIVE lock
    /////////////////////////////

    while (len)
    {
        uint64_t processedScaledInputSamples = 0;
        // g_sdlMiscOutSamples is used as PC Speaker samples buffer here
        if (g_sdlMiscOutSamplesEnd > 0)
        {
            processedScaledInputSamples = BE_Cross_TypedMin64(scaledSamplesToGenerate, (uint64_t)g_sdlMiscOutSamplesEnd * OPL_SAMPLE_RATE);
            scaledSamplesToGenerate -= processedScaledInputSamples;
        }
        while (scaledSamplesToGenerate)
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
            currNumOfScaledSamples = BE_Cross_TypedMin64(scaledSamplesToGenerate, g_sdlScaledSamplesInCurrentPart - g_sdlScaledSampleOffsetInSound);
            processedScaledInputSamples += currNumOfScaledSamples;
            /*** FIXME - Scaling back to the original rates may be a bit inaccurate (due to divisions) ***/

            // PC Speaker
            uint32_t targetPCSamples = processedScaledInputSamples / OPL_SAMPLE_RATE;
            if (targetPCSamples > g_sdlMiscOutNumOfSamples)
            {
                //BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "BEL_ST_Resampling_EmuCallBack PC overflow, want %u, have %u\n", targetPCSamples, g_sdlMiscOutNumOfSamples);
                targetPCSamples = g_sdlMiscOutNumOfSamples;
            }
            if (targetPCSamples > g_sdlMiscOutSamplesEnd)
            {
                if (g_sdlPCSpeakerOn)
                    PCSpeakerUpdateOne(&g_sdlMiscOutSamples[g_sdlMiscOutSamplesEnd], targetPCSamples - g_sdlMiscOutSamplesEnd);
                else
                    memset(&g_sdlMiscOutSamples[g_sdlMiscOutSamplesEnd], 0, sizeof(BE_ST_SndSample_T)*(targetPCSamples - g_sdlMiscOutSamplesEnd));

                g_sdlMiscOutSamplesEnd = targetPCSamples;
            }
            // AdLib:
            //
            // We may have pending AL data ready, but probably less than required
            // for filling the stream buffer, so generate some silence.
            //
            // Make sure we don't overthrow the AL buffer, though.
            uint32_t targetALSamples = processedScaledInputSamples / g_sdlAudioSpec.freq;
            if (targetALSamples > g_sdlALOutNumOfSamples)
            {
                //BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "BEL_ST_Resampling_EmuCallBack AL overflow, want %u, have %u\n", targetALSamples, g_sdlALOutNumOfSamples);
                targetALSamples = g_sdlALOutNumOfSamples;
            }
            if (targetALSamples > g_sdlALOutSamplesEnd)
            {
                YM3812UpdateOne(&oplChip, &g_sdlALOutSamples[g_sdlALOutSamplesEnd], targetALSamples - g_sdlALOutSamplesEnd);
                g_sdlALOutSamplesEnd = targetALSamples;
            }
            // We're done with current part for now
            g_sdlScaledSampleOffsetInSound += currNumOfScaledSamples;
            scaledSamplesToGenerate -= currNumOfScaledSamples;
            // End of part?
            if (g_sdlScaledSampleOffsetInSound >= g_sdlScaledSamplesInCurrentPart)
            {
                g_sdlScaledSampleOffsetInSound = 0;
                if (++g_sdlSamplesPartNum == PC_PIT_RATE)
                    g_sdlSamplesPartNum = 0;
                g_sdlScaledSamplesInCurrentPart = (g_sdlSamplesPartNum + 1) * g_sdlScaledSamplesPerPartsTimesPITRate / PC_PIT_RATE - g_sdlSamplesPartNum * g_sdlScaledSamplesPerPartsTimesPITRate / PC_PIT_RATE;
            }
        }

        // Try to resample the AL data we have (as much as possible)
        uint32_t maxSamplesToOutput = BE_Cross_TypedMin32(len / sizeof(BE_ST_SndSample_T), g_sdlMiscOutSamplesEnd); // Not taking plain (len / sizeof(BE_ST_SndSample_T)), just to make it safer

        uint32_t samples_consumed, samples_produced;
        BEL_ST_DoResample(&samples_consumed, &samples_produced, g_sdlALOutSamples, currSamplePtr, g_sdlALOutSamplesEnd, maxSamplesToOutput);

        len -= sizeof(BE_ST_SndSample_T)*samples_produced;
        // Mix PC Speaker output
        for (uint32_t i = 0; i < BE_Cross_TypedMin32(samples_produced, g_sdlMiscOutSamplesEnd); ++i)
            currSamplePtr[i] = (currSamplePtr[i] + g_sdlMiscOutSamples[i]) / 2;

        currSamplePtr += samples_produced;

        if ((samples_consumed > 0) && (samples_consumed < g_sdlALOutSamplesEnd))
            memmove(g_sdlALOutSamples, &g_sdlALOutSamples[samples_consumed], sizeof(BE_ST_SndSample_T)*(g_sdlALOutSamplesEnd - samples_consumed));
        g_sdlALOutSamplesEnd -= samples_consumed;

        /*** Note: Casting to unsigned for suppression of warnings again ***/
        if ((unsigned)samples_produced >= g_sdlMiscOutSamplesEnd)
            g_sdlMiscOutSamplesEnd = 0;
        else if ((samples_produced > 0) && (samples_produced < g_sdlMiscOutSamplesEnd))
        {
            memmove(g_sdlMiscOutSamples, &g_sdlMiscOutSamples[samples_produced], sizeof(BE_ST_SndSample_T)*(g_sdlMiscOutSamplesEnd - samples_produced));
            g_sdlMiscOutSamplesEnd -= samples_produced;
        }

        // Resampling may add some latency (audible or not), so generate a few more input samples if required.
        // In case we're stuck with no change, again we should try to generate more samples.
        if (!samples_consumed && !samples_produced)
            scaledSamplesToGenerateNextTime += (uint64_t)g_sdlAudioSpec.freq * OPL_SAMPLE_RATE / 1000; // ~1ms
        scaledSamplesToGenerate = scaledSamplesToGenerateNextTime;
    }
    ///////////////////////////////
    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
    ///////////////////////////////
}



/*** Digitized sounds playback ***/

void BE_ST_PlayS16SoundEffect(int16_t *data, int numOfSamples)
{
    BE_ST_LockAudioRecursively();

    g_sdlSoundEffectCurrPtr = data;
    g_sdlSoundEffectSamplesLeft = numOfSamples;

    BE_ST_UnlockAudioRecursively();
}

void BE_ST_StopSoundEffect(void)
{
    BE_ST_LockAudioRecursively();

    g_sdlSoundEffectSamplesLeft = 0;

    BE_ST_UnlockAudioRecursively();
}

// Helper function
static void BEL_ST_ConvertS16SamplesToOutputFormat(int16_t *inPtr, int16_t *outPtr, int samplesToCopy)
{
    memcpy(outPtr, inPtr, 2 * samplesToCopy);
}

/*** Digitized sounds callbacks ***/


static void BEL_ST_Simple_DigiCallBack(void *unused, Uint8 *stream, int len)
{
    if (len <= 0)
        return; // Just in case

    /////////////////////////////
    BE_ST_LockAudioRecursively(); // RECURSIVE lock
    /////////////////////////////

    len /= sizeof(BE_ST_SndSample_T); // Convert to samples

    // A little bit of cheating since we don't actually call any timer handler here
    g_sdlScaledSampleOffsetInSound += (uint64_t)len * PC_PIT_RATE;
    BE_ST_SET_TIMER_INT_COUNTER_ADD(g_sdlScaledSampleOffsetInSound / g_sdlScaledSamplesPerPartsTimesPITRate);
    g_sdlScaledSampleOffsetInSound %= g_sdlScaledSamplesPerPartsTimesPITRate;

    if ((uint32_t)len >= g_sdlSoundEffectSamplesLeft)
    {
        memset((BE_ST_SndSample_T *)stream + g_sdlSoundEffectSamplesLeft, 0, sizeof(BE_ST_SndSample_T) * (len - g_sdlSoundEffectSamplesLeft));
        if (g_sdlSoundEffectSamplesLeft > 0)
        {
            BEL_ST_ConvertS16SamplesToOutputFormat(g_sdlSoundEffectCurrPtr, (BE_ST_SndSample_T *)stream, g_sdlSoundEffectSamplesLeft);
            g_sdlSoundEffectSamplesLeft = 0;
            if (g_sdlTimerIntFuncPtr)
                g_sdlTimerIntFuncPtr();
        }
    }
    else
    {
        BEL_ST_ConvertS16SamplesToOutputFormat(g_sdlSoundEffectCurrPtr, (BE_ST_SndSample_T *)stream, len);
        g_sdlSoundEffectCurrPtr += len;
        g_sdlSoundEffectSamplesLeft -= len;
    }

    ///////////////////////////////
    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
    ///////////////////////////////
}

static void BEL_ST_Resampling_DigiCallBack(void *unused, Uint8 *stream, int len)
{
    /////////////////////////////
    BE_ST_LockAudioRecursively(); // RECURSIVE lock
    /////////////////////////////

    // A little bit of cheating since we don't actually call any timer handler here
    g_sdlScaledSampleOffsetInSound += (len / sizeof(BE_ST_SndSample_T)) * PC_PIT_RATE;
    BE_ST_SET_TIMER_INT_COUNTER_ADD(g_sdlScaledSampleOffsetInSound / g_sdlScaledSamplesPerPartsTimesPITRate);
    g_sdlScaledSampleOffsetInSound %= g_sdlScaledSamplesPerPartsTimesPITRate;

    while (len > 0)
    {
        if (g_sdlSoundEffectSamplesLeft > 0) // Input is always SINT16, output may differ
        {
            int samplesToCopy = BE_Cross_TypedMin32(g_sdlMiscOutNumOfSamples - g_sdlMiscOutSamplesEnd, g_sdlSoundEffectSamplesLeft);
            BEL_ST_ConvertS16SamplesToOutputFormat(g_sdlSoundEffectCurrPtr, &g_sdlMiscOutSamples[g_sdlMiscOutSamplesEnd], samplesToCopy);
            g_sdlMiscOutSamplesEnd += samplesToCopy;
            g_sdlSoundEffectCurrPtr += samplesToCopy;
            g_sdlSoundEffectSamplesLeft -= samplesToCopy;

            if ((g_sdlSoundEffectSamplesLeft == 0) && g_sdlTimerIntFuncPtr)
                g_sdlTimerIntFuncPtr();
        }
        if (g_sdlMiscOutSamplesEnd < g_sdlMiscOutNumOfSamples)
            memset(&g_sdlMiscOutSamples[g_sdlMiscOutSamplesEnd], 0, sizeof(BE_ST_SndSample_T)*(g_sdlMiscOutNumOfSamples - g_sdlMiscOutSamplesEnd));

        // Resample
        uint32_t samples_consumed, samples_produced;
        BEL_ST_DoResample(&samples_consumed, &samples_produced, g_sdlMiscOutSamples, (BE_ST_SndSample_T *)stream, g_sdlMiscOutNumOfSamples, len / sizeof(BE_ST_SndSample_T));
        stream += sizeof(BE_ST_SndSample_T) * samples_produced;
        len -= sizeof(BE_ST_SndSample_T) * samples_produced;
        // Move pending sound data
        g_sdlMiscOutSamplesEnd = g_sdlMiscOutNumOfSamples - samples_consumed;
        if (g_sdlMiscOutSamplesEnd > 0)
            memmove(g_sdlMiscOutSamples, &g_sdlMiscOutSamples[samples_consumed], sizeof(BE_ST_SndSample_T) * g_sdlMiscOutSamplesEnd);
    }

    /////////////////////////////
    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
    /////////////////////////////
}



void BE_ST_SetTimer(uint16_t rateVal)
{
    BE_ST_LockAudioRecursively(); // RECURSIVE lock

    // Note that 0 should be interpreted as 65536
    g_sdlScaledSamplesPerPartsTimesPITRate = (rateVal ? rateVal : 65536) * g_sdlAudioSpec.freq;
    if (g_sdlOurAudioCallback == BEL_ST_Resampling_EmuCallBack)
        g_sdlScaledSamplesPerPartsTimesPITRate *= OPL_SAMPLE_RATE;
    // Since the following division may lead to truncation, g_sdlScaledSamplesInCurrentPart
    // can change during playback by +-1 (otherwise music may be a bit faster than intended).
    g_sdlScaledSamplesInCurrentPart = g_sdlScaledSamplesPerPartsTimesPITRate / PC_PIT_RATE;

    BE_ST_UnlockAudioRecursively(); // RECURSIVE unlock
}

static void BEL_ST_TicksDelayWithOffset(int sdltickstowait);


void BE_ST_WaitForNewVerticalRetraces(int16_t number)
{
    // TODO (REFKEEN) Make a difference based on HW?

    // Simulate waiting while in vertical retrace first, and then
    // waiting while NOT in vertical retrace. In practice, we jump
    // to the very beginning of the next "refresh cycle".
    // This is repeated for a total of 'length' times.

    uint32_t currSdlTicks = SDL_GetTicks();
    uint32_t nextSdlTicks = (int)number * 1000000 / 70086 + currSdlTicks - g_sdlTicksOffset;
    // First iteration takes a bit less time again, so we want
    // the value "mod" about 1000/70.086 (VGA adapter);
    // For the sake of a bit better precision we further multiply by 1000
    nextSdlTicks -= (((uint64_t)1000 * (uint64_t)nextSdlTicks) % ((uint64_t)1000000000 / 70086)) / 1000;
    g_sdlTicksOffset = 0; // Can reset this, taking g_sdlTicksOffset into account above
    BEL_ST_TicksDelayWithOffset(nextSdlTicks - currSdlTicks);
}

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
