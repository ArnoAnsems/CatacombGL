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
#include "../timing/be_timing.h"
#include "be_audio_mixer.h"
#include "be_audio_resampling.h"
#include "be_st_sdl_private.h" // For BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* These tables are based on dB values described here by James-F:
   https://www.vogons.org/viewtopic.php?t=54269 */

static const float g_stSbProVolTable[16] = {
	0.003758/*10^-2.425*/, 0.003758/*10^-2.425*/,
	0.043152/*10^-1.365*/, 0.043152/*10^-1.365*/,
	0.084140/*10^-1.075*/, 0.084140/*10^-1.075*/,
	0.164059/*10^-0.785*/, 0.164059/*10^-0.785*/,
	0.281838/*10^-0.55*/,  0.281838/*10^-0.55*/,
	0.446684/*10^-0.35*/,  0.446684/*10^-0.35*/,
	0.707946/*10^-0.15*/,  0.707946/*10^-0.15*/,
	1,                     1
};

static const float g_stSb16VolTable[16] = {
	0.001334/*10^-2.875*/, 0.001820/*10^-2.74*/,
	0.002818/*10^-2.55*/,  0.004365/*10^-2.36*/,
	0.006918/*10^-2.16*/,  0.010471/*10^-1.98*/,
	0.016596/*10^-1.78*/,  0.025119/*10^-1.6*/,
	0.039811/*10^-1.4*/,   0.063096/*10^-1.2*/,
	0.100000/*10^-1*/,     0.158489/*10^-0.8*/,
	0.251189/*10^-0.6*/,   0.398107/*10^-0.4*/,
	0.630957/*10^-0.2*/,   1
};

enum { BE_ST_AUDIO_MIXER_MAX_SOURCES = 4 };

static struct
{
	BE_ST_AudioMixerSource sources[BE_ST_AUDIO_MIXER_MAX_SOURCES];
	const float *sbVolTable;
	uint64_t samplesPerPartTimesPITRate;
	uint32_t samplesInCurrentPart;
	uint32_t samplesPartNum;
	uint32_t pendingSamples;
	uint32_t offsetInSound;
	int numSources;
	int freq;
	int channels;
} g_stAudioMixer;

static void BEL_ST_AudioMixerFreeSourceBuffers(BE_ST_AudioMixerSource *src)
{
	if (g_stAudioMixer.freq != src->freq)
		BEL_ST_ShutdownResampling(&src->resamplingContext);
	free(src->out.buffer);
	free(src->in.buffer);
}

void BE_ST_AudioMixerSetSourceFreq(BE_ST_AudioMixerSource *src, int freq)
{
	BE_ST_LockMutexRecursively(src->mutex);
	src->skip = true;
	BE_ST_UnlockMutexRecursively(src->mutex);

	if (src->in.buffer)
		BEL_ST_AudioMixerFreeSourceBuffers(src);

	src->in.num = 0;
	src->out.num = 0;
	src->numScaledSamplesToGenNextTime = 0;
	// src->out.size shall not change, and should've been set earlier
	src->in.size = src->out.size * freq / g_stAudioMixer.freq;
	src->freq = freq;

	src->in.buffer = (BE_ST_SndSample_T *)malloc(src->in.size * sizeof(BE_ST_SndSample_T));
//	if (!src->in.buffer)
//		BE_ST_ExitWithErrorMsg("BE_ST_AudioMixerSetSourceFreq: Out of memory!");

	if (g_stAudioMixer.freq != freq)
		BEL_ST_InitResampling(&src->resamplingContext,
		                      g_stAudioMixer.freq, freq, src->in.buffer);

	src->out.buffer = (BE_ST_SndSample_T *)malloc(src->out.size * sizeof(BE_ST_SndSample_T));
//	if (!src->out.buffer)
//		BE_ST_ExitWithErrorMsg("BE_ST_AudioMixerSetSourceFreq: Out of memory!");
finish:
	BE_ST_LockMutexRecursively(src->mutex);
	src->skip = false;
	BE_ST_UnlockMutexRecursively(src->mutex);
}

void BEL_ST_AudioMixerInit(int freq, int channels)
{
//	if ((channels != 1) && (channels != 2))
//		BE_ST_ExitWithErrorMsg("BEL_ST_AudioMixerInitource: Unsupported channels count!");
	g_stAudioMixer.offsetInSound = 0;
	g_stAudioMixer.samplesPartNum = 0;
	g_stAudioMixer.numSources = 0;
	g_stAudioMixer.freq = freq;
	g_stAudioMixer.channels = channels;
	g_stAudioMixer.sbVolTable =
		(g_refKeenCfg.sb >= SOUNDBLASTER_SB16) ?
		g_stSb16VolTable : g_stSbProVolTable;
}

void BEL_ST_AudioMixerShutdown(void)
{
	for (int i = 0; i < g_stAudioMixer.numSources; ++i)
	{
		BE_ST_FreeMutex(g_stAudioMixer.sources[i].mutex);
		BEL_ST_AudioMixerFreeSourceBuffers(&g_stAudioMixer.sources[i]);
	}
}

void BEL_ST_AudioMixerUpdateFromPITRateWord(int_fast32_t rateVal)
{
	g_stAudioMixer.samplesPerPartTimesPITRate = rateVal * g_stAudioMixer.freq;
	// Since the following division may lead to truncation,
	// samplesInCurrentPart can change during playback by +-1
	// (otherwise music may be a bit faster than intended).
	g_stAudioMixer.samplesInCurrentPart = g_stAudioMixer.samplesPerPartTimesPITRate / PC_PIT_RATE;
}

BE_ST_AudioMixerSource *BEL_ST_AudioMixerAddSource(
	int freq, int maxNumOfOutSamples, int userVolAsInt,
	void (*genSamples)(BE_ST_SndSample_T *stream, int len))
{
//	if (g_stAudioMixer.numSources == BE_ST_AUDIO_MIXER_MAX_SOURCES)
//		BE_ST_ExitWithErrorMsg("BEL_ST_AudioMixerAddSource: Can't add another source!");
	BE_ST_AudioMixerSource *src = &g_stAudioMixer.sources[g_stAudioMixer.numSources++];

	src->genSamples = genSamples;
	src->out.size = maxNumOfOutSamples;
	src->in.buffer = src->out.buffer = 0;

	if (g_refKeenCfg.sb < SOUNDBLASTER_SBPRO)
		src->vol[0] = src->vol[1] = 1.0;
	else
	{
		src->sbVolBits = (g_refKeenCfg.sb < SOUNDBLASTER_SB16) ? 0x99 : 0xCC;
		src->vol[0] = g_stAudioMixer.sbVolTable[src->sbVolBits>>4];
		src->vol[1] = g_stAudioMixer.sbVolTable[src->sbVolBits&15];
	}

	src->userVol = (userVolAsInt == BE_AUDIO_VOL_MIN) ?
	               0.0 : exp((userVolAsInt - BE_AUDIO_VOL_MAX)/4.0);

	src->mutex = BE_ST_CreateMutex();
	assert(src->mutex);

	BE_ST_AudioMixerSetSourceFreq(src, freq);
	return src;
}

void BEL_ST_SetSBProVolumesForSource(BE_ST_AudioMixerSource *src, uint8_t volBits)
{
//	if (!src)
//		BE_ST_ExitWithErrorMsg("BEL_ST_SetSBProVolumesForSource: Called with null source!");

	if (g_refKeenCfg.sb < SOUNDBLASTER_SBPRO)
	{
		BE_Cross_LogMessage(
			BE_LOG_MSG_NORMAL,
			"BEL_ST_SetSBProVolumesForSource: Called without SB Pro features\n");
		return;
	}

	bool mute = (volBits == 0);
	if (g_refKeenCfg.sb < SOUNDBLASTER_SB16)
		volBits |= 0x11;

	BE_ST_LockMutexRecursively(src->mutex);
	src->sbVolBits = volBits;
	if (mute)
		src->vol[0] = src->vol[1] = 0.0;
	else
	{
		src->vol[0] = g_stAudioMixer.sbVolTable[volBits>>4];
		src->vol[1] = g_stAudioMixer.sbVolTable[volBits&15];
	}
	BE_ST_UnlockMutexRecursively(src->mutex);
}

uint8_t BEL_ST_GetSBProVolumesFromSource(const BE_ST_AudioMixerSource *src)
{
	BE_ST_LockMutexRecursively(src->mutex);
	uint8_t volBits = src->sbVolBits;
	BE_ST_UnlockMutexRecursively(src->mutex);
	return volBits;
}

void BEL_ST_AudioMixerCallback(BE_ST_SndSample_T *stream, int len)
{
	int samplesToGenerate = len;
	int samplesToGenerateNextTime = g_stAudioMixer.freq / 100; // ~10ms
	int i, j, k;
	BE_ST_AudioMixerSource *src;

	while (len)
	{
		uint32_t processedInputSamples = g_stAudioMixer.pendingSamples;
		samplesToGenerate -= processedInputSamples;
		while (samplesToGenerate > 0)
		{
			BE_ST_LockAudioRecursively();
			// Optionally make an inner callback back, possibly in game code
			if (!g_stAudioMixer.offsetInSound && g_sdlTimerIntFuncPtr)
			{
				g_sdlTimerIntFuncPtr();
#ifndef BE_ST_MANAGE_INT_CALLS_SEPARATELY_FROM_AUDIO
				BE_ST_SET_TIMER_INT_COUNTER_INC();
#endif
			}
			BE_ST_UnlockAudioRecursively();
			// Now generate sound
			uint32_t currNumOfSamples =
				BE_Cross_TypedMin(
					uint32_t,
					samplesToGenerate,
					g_stAudioMixer.samplesInCurrentPart-g_stAudioMixer.offsetInSound);
			processedInputSamples += currNumOfSamples;
			g_stAudioMixer.pendingSamples = processedInputSamples;
			for (i = 0; i < g_stAudioMixer.numSources; ++i)
			{
				src = &g_stAudioMixer.sources[i];
				BE_ST_LockMutexRecursively(src->mutex);
				if (!src->skip)
				{
					uint32_t srcSamplesToGen = processedInputSamples;
					srcSamplesToGen = srcSamplesToGen * src->freq + src->numScaledSamplesToGenNextTime;
					src->numScaledSamplesToGenNextTime = srcSamplesToGen % g_stAudioMixer.freq;
					srcSamplesToGen /= g_stAudioMixer.freq;
					if (srcSamplesToGen > src->in.size)
					{
						BE_Cross_LogMessage(
							BE_LOG_MSG_NORMAL,
							"BEL_ST_AudioMixerCallback: Overflow for source %d; Want %u, have %u\n",
							i, srcSamplesToGen, src->in.size);
						srcSamplesToGen = src->in.size;
					}
					if (srcSamplesToGen > src->in.num)
					{
						src->genSamples(&src->in.buffer[src->in.num],
						                srcSamplesToGen - src->in.num);
						src->in.num = srcSamplesToGen;
					}
				}
				BE_ST_UnlockMutexRecursively(src->mutex);
			}
			// We're done with current part for now
			g_stAudioMixer.offsetInSound += currNumOfSamples;
			samplesToGenerate -= currNumOfSamples;
			// End of part?
			if (g_stAudioMixer.offsetInSound >= g_stAudioMixer.samplesInCurrentPart)
			{
				g_stAudioMixer.offsetInSound = 0;
				if (++g_stAudioMixer.samplesPartNum == PC_PIT_RATE)
					g_stAudioMixer.samplesPartNum = 0;
				g_stAudioMixer.samplesInCurrentPart =
					(g_stAudioMixer.samplesPartNum + 1) * g_stAudioMixer.samplesPerPartTimesPITRate / PC_PIT_RATE -
					g_stAudioMixer.samplesPartNum * g_stAudioMixer.samplesPerPartTimesPITRate / PC_PIT_RATE;
			}
		}

		// Try to resample the data that we have
		uint32_t samplesToOutput = len;
		for (i = 0; i < g_stAudioMixer.numSources; ++i)
		{
			src = &g_stAudioMixer.sources[i];
			BE_ST_LockMutexRecursively(src->mutex);
			if (!src->skip)
			{
				uint32_t consumed, produced;
				uint32_t maxSamplesToOutput = BE_Cross_TypedMin(
					uint32_t, len,
					src->out.size - src->out.num);
				if (g_stAudioMixer.freq != src->freq)
					BEL_ST_DoResample(&src->resamplingContext,
					                  &consumed, &produced,
					                  src->in.buffer,
					                  &src->out.buffer[src->out.num],
					                  src->in.num, maxSamplesToOutput);
				else
				{
					consumed = produced = BE_Cross_TypedMin(
						uint32_t, maxSamplesToOutput, src->in.num);
					memcpy(&src->out.buffer[src->out.num], src->in.buffer,
					       sizeof(BE_ST_SndSample_T) * consumed);
				}
				if ((consumed > 0) && (consumed < src->in.num))
					memmove(src->in.buffer, &src->in.buffer[consumed],
					        sizeof(BE_ST_SndSample_T) * (src->in.num - consumed));

				src->in.num -= consumed;
				src->out.num += produced;

				samplesToOutput = BE_Cross_TypedMin(uint32_t, samplesToOutput, src->out.num);
			}
			BE_ST_UnlockMutexRecursively(src->mutex);
		}

		// Mix
		if (samplesToOutput > 0)
		{
			int channels = g_stAudioMixer.channels;

			for (i = 0; i < samplesToOutput; ++i, stream += channels)
			{
				BE_ST_SndSamplesSum_T sums[2] = {
					MIXER_SILENCE_SAMPLE,
					MIXER_SILENCE_SAMPLE
				};
				for (j = 0; j < g_stAudioMixer.numSources; ++j)
				{
					src = &g_stAudioMixer.sources[j];
					BE_ST_LockMutexRecursively(src->mutex);
					if (!src->skip)
					{
						if (channels == 1)
							sums[0] += src->out.buffer[i] * src->userVol *
							           ((src->vol[0] + src->vol[1]) / 2.0f);
						else
						{
							sums[0] += src->out.buffer[i] * src->userVol * src->vol[0];
							sums[1] += src->out.buffer[i] * src->userVol * src->vol[1];
						}
					}
					BE_ST_UnlockMutexRecursively(src->mutex);
				}
				stream[0] = BEL_ST_SamplesSumClamp(sums[0]);
				if (channels != 1)
					stream[1] = BEL_ST_SamplesSumClamp(sums[1]);
			}

			for (j = 0; j < g_stAudioMixer.numSources; ++j)
			{
				src = &g_stAudioMixer.sources[j];
				BE_ST_LockMutexRecursively(src->mutex);
				if (!src->skip)
				{
					if (samplesToOutput < src->out.num)
					{
						memmove(src->out.buffer,
						        &src->out.buffer[samplesToOutput],
						        sizeof(BE_ST_SndSample_T) * (src->out.num - samplesToOutput));
						src->out.num -= samplesToOutput;
					}
					else
						src->out.num = 0;
				}
				BE_ST_UnlockMutexRecursively(src->mutex);
			}
			len -= samplesToOutput;
			if (g_stAudioMixer.pendingSamples < samplesToOutput)
				g_stAudioMixer.pendingSamples = 0;
			else
				g_stAudioMixer.pendingSamples -= samplesToOutput;
		}
		else
		{
			// Resampling may add some latency (audible or not),
			// so generate a few more input samples if required.
			// In case we're stuck with no change,
			// again we should try to generate more samples.
			samplesToGenerateNextTime += (uint64_t)g_stAudioMixer.freq / 1000; // ~1ms
		}
		samplesToGenerate = samplesToGenerateNextTime;
	}
}
