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

#include "be_st.h"
#include "be_audio_mixer.h"

// Changing between 0 and max. possible value - too loud
#ifdef MIXER_SAMPLE_FORMAT_FLOAT
#define MIXER_SAMPLE_PCSPEAKER_TOP_VAL 0.25f
#elif (defined MIXER_SAMPLE_FORMAT_SINT16)
#define MIXER_SAMPLE_PCSPEAKER_TOP_VAL 8192
#endif

static int g_be_pcSpeakerSampleFreq;
// Current PC Speaker status
static bool g_sdlPCSpeakerConnected = false;
static BE_ST_SndSample_T g_sdlCurrentBeepSample;
static uint32_t g_sdlBeepHalfCycleCounter, g_sdlBeepHalfCycleCounterUpperBound;

// Used for locking resources
static BE_ST_AudioMixerSource *g_pcSpeakerMixerSource;

static inline void BEL_ST_PCSpeakerSetBeepSample(bool isUp)
{
	// Changing between 0 and max. possible value - too loud
	g_sdlCurrentBeepSample = isUp ? MIXER_SAMPLE_PCSPEAKER_TOP_VAL : 0;
}

// Frequency is about 1193182Hz/spkVal
void BE_ST_PCSpeakerSetInvFreq(uint16_t spkInvFreq)
{
	BE_ST_LockMutexRecursively(g_pcSpeakerMixerSource->mutex);
	g_sdlPCSpeakerConnected = true;
	BEL_ST_PCSpeakerSetBeepSample(0);
	g_sdlBeepHalfCycleCounter = 0;
	g_sdlBeepHalfCycleCounterUpperBound = g_be_pcSpeakerSampleFreq * spkInvFreq;
	BE_ST_UnlockMutexRecursively(g_pcSpeakerMixerSource->mutex);
}

void BE_ST_PCSpeakerSetConstVal(bool isUp)
{
	BE_ST_LockMutexRecursively(g_pcSpeakerMixerSource->mutex);
	g_sdlPCSpeakerConnected = false;
	BEL_ST_PCSpeakerSetBeepSample(isUp);
	BE_ST_UnlockMutexRecursively(g_pcSpeakerMixerSource->mutex);
}

void BE_ST_BSound(uint16_t frequency)
{
	BE_ST_LockMutexRecursively(g_pcSpeakerMixerSource->mutex);
	BE_ST_PCSpeakerSetInvFreq(PC_PIT_RATE/(uint32_t)frequency);
	BE_ST_UnlockMutexRecursively(g_pcSpeakerMixerSource->mutex);
}

void BE_ST_BNoSound(void)
{
	BE_ST_LockMutexRecursively(g_pcSpeakerMixerSource->mutex);
	BE_ST_PCSpeakerSetConstVal(0);
	BE_ST_UnlockMutexRecursively(g_pcSpeakerMixerSource->mutex);
}

void BEL_ST_SetPCSpeakerMixerSource(BE_ST_AudioMixerSource *src)
{
        g_pcSpeakerMixerSource = src;
}

void BEL_ST_SetPCSpeakerSampleRate(int rate)
{
	g_be_pcSpeakerSampleFreq = rate;
}

/*** PC Speaker: Add audio data to the stream ***/
void BEL_ST_GenPCSpeakerSamples(BE_ST_SndSample_T *stream, int length)
{
	if (!g_sdlPCSpeakerConnected)
	{
		for (int loopVar = 0; loopVar < length; loopVar++, *stream++ = g_sdlCurrentBeepSample)
			;
		return;
	}

	for (int loopVar = 0; loopVar < length; loopVar++, stream++)
	{
		*stream = g_sdlCurrentBeepSample;
		g_sdlBeepHalfCycleCounter += 2 * PC_PIT_RATE;
		if (g_sdlBeepHalfCycleCounter >= g_sdlBeepHalfCycleCounterUpperBound)
		{
			g_sdlBeepHalfCycleCounter %= g_sdlBeepHalfCycleCounterUpperBound;
			BEL_ST_PCSpeakerSetBeepSample(g_sdlCurrentBeepSample < MIXER_SAMPLE_PCSPEAKER_TOP_VAL/2);
		}
	}
}
