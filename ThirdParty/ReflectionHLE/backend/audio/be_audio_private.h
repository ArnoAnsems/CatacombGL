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

#ifndef BE_AUDIO_PRIVATE_H
#define BE_AUDIO_PRIVATE_H

#include "refkeen_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "be_cross.h"

#define PC_PIT_RATE 1193182

#define OPL_SAMPLE_RATE 49716

#if (defined REFKEEN_RESAMPLER_LIBSAMPLERATE) || (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
#define MIXER_SAMPLE_FORMAT_FLOAT
#define MIXER_SILENCE_SAMPLE 0.f
#else
#define MIXER_SAMPLE_FORMAT_SINT16
#define MIXER_SILENCE_SAMPLE 0
#endif

#ifdef MIXER_SAMPLE_FORMAT_FLOAT
typedef float BE_ST_SndSample_T;
typedef float BE_ST_SndSamplesSum_T;
static inline BE_ST_SndSample_T BEL_ST_SamplesSumClamp(BE_ST_SndSamplesSum_T sum)
{
	return BE_Cross_TypedClamp(float, sum, -1.f, 1.f);
}
#elif (defined MIXER_SAMPLE_FORMAT_SINT16)
typedef int16_t BE_ST_SndSample_T;
typedef int32_t BE_ST_SndSamplesSum_T;
static inline BE_ST_SndSample_T BEL_ST_SamplesSumClamp(BE_ST_SndSamplesSum_T sum)
{
	return BE_Cross_TypedClamp(int32_t, sum, -32768, 32767);
}
#endif

#define MIXER_DEFAULT_CHANNELS_COUNT 2

extern bool g_sdlEmulatedOPLChipReady;

void BEL_ST_ResetOPLChip(void);

bool BEL_ST_InitAudioSubsystem(int *freq, int *channels, int *bufferLen);
void BEL_ST_ShutdownAudioSubsystem(void);
void BEL_ST_StartAudioSubsystem(void);

/*** Use these functions for adding audio data to the stream ***/
void BEL_ST_GenPCSpeakerSamples(BE_ST_SndSample_T *stream, int length);
void BEL_ST_GenOPLSamples(BE_ST_SndSample_T *stream, int length);
void BEL_ST_GenDigiSamples(BE_ST_SndSample_T *stream, int length);

void BEL_ST_SetPCSpeakerSampleRate(int rate);

struct BE_ST_AudioMixerSource;
void BEL_ST_SetPCSpeakerMixerSource(struct BE_ST_AudioMixerSource *src);
void BEL_ST_SetOPLMixerSource(struct BE_ST_AudioMixerSource *src);
void BEL_ST_SetDigiMixerSource(struct BE_ST_AudioMixerSource *src);

#endif
