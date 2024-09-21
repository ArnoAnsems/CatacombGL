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

// The code found here is used if the audio subsystem is
// disabled *or* BE_ST_FILL_AUDIO_IN_MAIN_THREAD is defined

#include "refkeen_config.h"

#include "be_audio_mixer.h"
#include "be_cross.h"
#include "be_st.h"
#include "be_st_sdl_private.h"

// Use this rate, measured in BYTES, if the audio subsystem is disabled
// for most (same units as used in values returned by BEL_ST_GetTicksMS())
#define NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM 1000

extern bool g_sdlAudioSubsystemUp;
extern bool g_sdlAudioInitDone;

static int g_be_audioMainThread_channels;
static int g_be_audioMainThread_freq;
// If enabled, this buffer is actually split into two subbuffers:
// One for main thread use, the other being shared with the audio callback thread.
static BE_ST_SndSample_T *g_be_audioMainThread_samplesBuffer;
// If two sub-buffers are used, this is the size of a single one
static uint32_t g_be_audioMainThread_samplesBufferLenOfPart;
#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
static uint32_t g_be_audioMainThread_samplesRemainingForCallback;
#endif

uint32_t g_be_audioMainThread_lastCallTicks;
static uint32_t g_be_audioMainThread_delayedSamples;

int BEL_ST_PrepareMainThreadForAudio(int *freq, int *channels, int expectedCallbackBufferLen)
{
	g_be_audioMainThread_lastCallTicks = BEL_ST_GetTicksMS();
	g_be_audioMainThread_delayedSamples = 0;

#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
	if (g_sdlAudioSubsystemUp)
	{
		g_be_audioMainThread_samplesBufferLenOfPart = g_refKeenCfg.sndInterThreadBufferRatio * expectedCallbackBufferLen;
		// Allocate TWO parts, hence the multiplication by 2
		g_be_audioMainThread_samplesBuffer = (BE_ST_SndSample_T *)malloc(2*(*channels)*sizeof(BE_ST_SndSample_T)*g_be_audioMainThread_samplesBufferLenOfPart);
//		if (!g_be_audioMainThread_samplesBuffer)
//			BE_ST_ExitWithErrorMsg("BEL_ST_PrepareMainThreadForAudio: Out of memory! (Failed to allocate g_be_audioMainThread_samplesBuffer.)");
		g_be_audioMainThread_samplesRemainingForCallback = 0;
		g_be_audioMainThread_channels = *channels;
		g_be_audioMainThread_freq = *freq;
	}
	else
#endif
	{
		// If the audio subsystem is off, let us simulate a byte rate
		// of 1000Hz (same as BEL_ST_GetTicksMS() time units)
		*channels = g_be_audioMainThread_channels = 1;
		*freq = g_be_audioMainThread_freq = NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM / sizeof(BE_ST_SndSample_T);
		g_be_audioMainThread_samplesBuffer = (BE_ST_SndSample_T *)malloc(NUM_OF_BYTES_FOR_SOUND_CALLBACK_WITH_DISABLED_SUBSYSTEM);
//		if (!g_be_audioMainThread_samplesBuffer)
//			BE_ST_ExitWithErrorMsg("BEL_ST_PrepareMainThreadForAudio: Out of memory! (Failed to allocate g_be_audioMainThread_samplesBuffer.)");
		g_be_audioMainThread_samplesBufferLenOfPart = g_be_audioMainThread_freq;
	}
	return g_be_audioMainThread_samplesBufferLenOfPart;
}

void BEL_ST_ClearMainThreadAudioResources(void)
{
	free(g_be_audioMainThread_samplesBuffer);
	g_be_audioMainThread_samplesBuffer = NULL;
}

// Use this ONLY if audio subsystem isn't properly
// started up if BE_ST_FILL_AUDIO_IN_MAIN_THREAD is not defined
void BE_ST_PrepareForManualAudioCallbackCall(void)
{
	uint32_t currTicks = BEL_ST_GetTicksMS();

	// If e.g., we call this function from BE_ST_PrepareForGameStartupWithoutAudio
	if (!g_sdlAudioInitDone)
		return;

	if (currTicks == g_be_audioMainThread_lastCallTicks)
		return;

	// Using g_be_audioMainThread_freq as the rate, we (generally)
	// lose precision in the following division, so we use
	// g_be_audioMainThread_delayedSamples to accumulate lost samples.
	uint64_t dividend = ((uint64_t)g_be_audioMainThread_freq)*(currTicks-g_be_audioMainThread_lastCallTicks) + g_be_audioMainThread_delayedSamples;
	uint32_t samplesPassed = dividend/1000;
	g_be_audioMainThread_delayedSamples = dividend%1000;

	uint32_t samplesToProcess = samplesPassed;
	// Buffer has some constant size, so loop if required (which may hint at an overflow)
	for (; samplesToProcess >= g_be_audioMainThread_samplesBufferLenOfPart; samplesToProcess -= g_be_audioMainThread_samplesBufferLenOfPart)
		BEL_ST_AudioMixerCallback(g_be_audioMainThread_samplesBuffer, g_be_audioMainThread_samplesBufferLenOfPart);
	if (samplesToProcess > 0)
		BEL_ST_AudioMixerCallback(g_be_audioMainThread_samplesBuffer, samplesToProcess);
	g_be_audioMainThread_lastCallTicks = currTicks;
#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
	if (g_sdlAudioSubsystemUp)
	{
		// Pass samples to audio callback thread (as much as we can)
		BE_ST_LockAudioRecursively();
		// Note that if we filled more than g_be_audioMainThread_samplesBufferLenOfPart,
		// and thus discarded some samples, they won't be covered here.
		int samplesToCopy = BE_Cross_TypedMin(int, samplesPassed, g_be_audioMainThread_samplesBufferLenOfPart - g_be_audioMainThread_samplesRemainingForCallback);
		// NOTE: We copy to the SECOND HALF of the buffer!
		memcpy(g_be_audioMainThread_samplesBuffer + g_be_audioMainThread_channels * (g_be_audioMainThread_samplesBufferLenOfPart + g_be_audioMainThread_samplesRemainingForCallback),
		       g_be_audioMainThread_samplesBuffer, samplesToCopy * sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels);
		g_be_audioMainThread_samplesRemainingForCallback += samplesToCopy;
		BE_ST_UnlockAudioRecursively();
	}
#endif
}


// A (relatively) simple callback, used for copying samples from main thread to SDL audio callback thread
#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
void BEL_ST_InterThread_CallBack(void *unused, Uint8 *stream, int len)
{
	BE_ST_LockAudioRecursively();

	int samplesToCopy = BE_Cross_TypedMin(int, g_be_audioMainThread_samplesRemainingForCallback, len / (sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels));
	memcpy(stream,
	       g_be_audioMainThread_samplesBuffer + g_be_audioMainThread_channels * g_be_audioMainThread_samplesBufferLenOfPart,
	       samplesToCopy * sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels);
	// Shift remaining samples
	memmove(g_be_audioMainThread_samplesBuffer + g_be_audioMainThread_channels * g_be_audioMainThread_samplesBufferLenOfPart,
	        g_be_audioMainThread_samplesBuffer + g_be_audioMainThread_channels * (g_be_audioMainThread_samplesBufferLenOfPart + samplesToCopy),
	        (g_be_audioMainThread_samplesRemainingForCallback - samplesToCopy) * sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels);
	g_be_audioMainThread_samplesRemainingForCallback -= samplesToCopy;

	BE_ST_UnlockAudioRecursively();
	// No need to have lock here
	if (samplesToCopy < len / (sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels))
		memset(stream + samplesToCopy * sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels,
		       0,
		       len - samplesToCopy * sizeof(BE_ST_SndSample_T) * g_be_audioMainThread_channels);
}
#endif
