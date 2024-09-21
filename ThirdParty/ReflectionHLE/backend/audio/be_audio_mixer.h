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

#ifndef BE_AUDIO_MIXER_H
#define BE_AUDIO_MIXER_H

#include "be_audio_resampling.h"
#include "../threading/be_mutex.h"

typedef struct BE_ST_AudioMixerSource
{
	void (*genSamples)(BE_ST_SndSample_T *stream, int length);
	/* Deadlock prevention rule: When a thread locks this mutex,
	   it must NOT lock another mutex before unlocking this one. */
	BE_ST_Mutex_T *mutex;
	struct
	{
		BE_ST_SndSample_T *buffer;
		int size, num;
	} in, out;
	BESDLResamplingContext resamplingContext;
	int numScaledSamplesToGenNextTime;
	int freq;
	float vol[2], userVol;
	uint8_t sbVolBits;
	bool skip;
} BE_ST_AudioMixerSource;

void BEL_ST_AudioMixerInit(int freq, int channels);
void BEL_ST_AudioMixerShutdown(void);
void BEL_ST_AudioMixerCallback(BE_ST_SndSample_T *stream, int len);
void BEL_ST_AudioMixerUpdateFromPITRateWord(int_fast32_t rateVal);

// Returns a pointer to a new source, which is invalidated after mixer shutdown
BE_ST_AudioMixerSource *BEL_ST_AudioMixerAddSource(
	int freq, int maxNumOfOutSamples, int userVolAsInt,
	void (*genSamples)(BE_ST_SndSample_T *stream, int len));

void BE_ST_AudioMixerSetSourceFreq(BE_ST_AudioMixerSource *src, int freq);

void BEL_ST_SetSBProVolumesForSource(BE_ST_AudioMixerSource *src, uint8_t volBits);
uint8_t BEL_ST_GetSBProVolumesFromSource(const BE_ST_AudioMixerSource *src);

#endif
