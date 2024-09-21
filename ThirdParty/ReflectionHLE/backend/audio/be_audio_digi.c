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
#include "refkeen_config.h"
#include "be_audio_mixer.h"
#include <string.h>

static void (*g_sdlDigiAudioIntFuncPtr)(void);

static void *g_sdlSoundEffectCurrPtr;
static uint32_t g_sdlSoundEffectSamplesLeft;
static int g_sdlSoundEffectBits;

// Used for changing input sample rate and checking if emulated SB Pro is "ready"
static BE_ST_AudioMixerSource *g_sdlDigiMixerSource;

void BE_ST_PlaySoundEffect(void *data, int numOfSamples, int bits)
{
	BE_ST_LockMutexRecursively(g_sdlDigiMixerSource->mutex);

	g_sdlSoundEffectCurrPtr = data;
	g_sdlSoundEffectSamplesLeft = numOfSamples;
	g_sdlSoundEffectBits = bits;

	BE_ST_UnlockMutexRecursively(g_sdlDigiMixerSource->mutex);
}

void BE_ST_StopSoundEffect(void)
{
	BE_ST_LockMutexRecursively(g_sdlDigiMixerSource->mutex);

	g_sdlSoundEffectSamplesLeft = 0;

	BE_ST_UnlockMutexRecursively(g_sdlDigiMixerSource->mutex);
}

bool BE_ST_IsEmulatedSBReady(void)
{
	return g_sdlDigiMixerSource;
}

bool BE_ST_IsEmulatedSBProReady(void)
{
	// TODO: Also check mixer channels count?
	return g_sdlDigiMixerSource && (g_refKeenCfg.sb >= SOUNDBLASTER_SBPRO);
}

// Helper function
#ifdef MIXER_SAMPLE_FORMAT_FLOAT
static void BEL_ST_ConvertU8SamplesToOutputFormat(uint8_t *inPtr, float *outPtr, int samplesToCopy)
{
	for (int i = 0; i < samplesToCopy; ++i, ++inPtr, ++outPtr)
		*outPtr = ((float)(*inPtr - 128))/127.0f;
}
static void BEL_ST_ConvertS16SamplesToOutputFormat(int16_t *inPtr, float *outPtr, int samplesToCopy)
{
	for (int i = 0; i < samplesToCopy; ++i, ++inPtr, ++outPtr)
		*outPtr = ((float)(*inPtr))/32767.0f;
}
#elif (defined MIXER_SAMPLE_FORMAT_SINT16)
static void BEL_ST_ConvertU8SamplesToOutputFormat(uint8_t *inPtr, int16_t *outPtr, int samplesToCopy)
{
	for (int i = 0; i < samplesToCopy; ++i, ++inPtr, ++outPtr)
		*outPtr = (*inPtr - 128)*256;
}
static void BEL_ST_ConvertS16SamplesToOutputFormat(int16_t *inPtr, int16_t *outPtr, int samplesToCopy)
{
	memcpy(outPtr, inPtr, 2*samplesToCopy);
}
#endif

void BE_ST_StartDigiAudioInt(void (*funcPtr)(void))
{
	BE_ST_LockMutexRecursively(g_sdlDigiMixerSource->mutex);
	g_sdlDigiAudioIntFuncPtr = funcPtr;
	BE_ST_UnlockMutexRecursively(g_sdlDigiMixerSource->mutex);
}

void BE_ST_StopDigiAudioInt(void)
{
	BE_ST_LockMutexRecursively(g_sdlDigiMixerSource->mutex);
	g_sdlDigiAudioIntFuncPtr = 0;
	BE_ST_UnlockMutexRecursively(g_sdlDigiMixerSource->mutex);
}

void BEL_ST_SetDigiMixerSource(BE_ST_AudioMixerSource *src)
{
	g_sdlDigiMixerSource = src;
}

void BE_ST_SetDigiSoundFreq(int freq)
{
	BE_ST_AudioMixerSetSourceFreq(g_sdlDigiMixerSource, freq);
}

void BE_ST_SetDigiSBProSoundVolumes(uint8_t volBits)
{
	BEL_ST_SetSBProVolumesForSource(g_sdlDigiMixerSource, volBits);
}

uint8_t BE_ST_GetDigiSBProSoundVolumes(void)
{
	return BEL_ST_GetSBProVolumesFromSource(g_sdlDigiMixerSource);
}

void BEL_ST_GenDigiSamples(BE_ST_SndSample_T *stream, int length)
{
	if ((uint32_t)length >= g_sdlSoundEffectSamplesLeft)
	{
		memset(stream + g_sdlSoundEffectSamplesLeft, 0, sizeof(BE_ST_SndSample_T) * (length - g_sdlSoundEffectSamplesLeft));
		if (g_sdlSoundEffectSamplesLeft > 0)
		{
			if (g_sdlSoundEffectBits == 16)
				BEL_ST_ConvertS16SamplesToOutputFormat((int16_t *)g_sdlSoundEffectCurrPtr, stream, g_sdlSoundEffectSamplesLeft);
			else
				BEL_ST_ConvertU8SamplesToOutputFormat((uint8_t *)g_sdlSoundEffectCurrPtr, stream, g_sdlSoundEffectSamplesLeft);
			g_sdlSoundEffectSamplesLeft = 0;
			if (g_sdlDigiAudioIntFuncPtr)
				g_sdlDigiAudioIntFuncPtr();
		}
	}
	else
	{
		if (g_sdlSoundEffectBits == 16)
		{
			BEL_ST_ConvertS16SamplesToOutputFormat((int16_t *)g_sdlSoundEffectCurrPtr, stream, length);
			g_sdlSoundEffectCurrPtr = (int16_t *)g_sdlSoundEffectCurrPtr + length;
		}
		else
		{
			BEL_ST_ConvertU8SamplesToOutputFormat((uint8_t *)g_sdlSoundEffectCurrPtr, stream, length);
			g_sdlSoundEffectCurrPtr = (uint8_t *)g_sdlSoundEffectCurrPtr + length;
		}
		g_sdlSoundEffectSamplesLeft -= length;
	}
}
