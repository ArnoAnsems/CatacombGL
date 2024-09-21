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

#ifndef BE_AUDIO_RESAMPLING_H
#define BE_AUDIO_RESAMPLING_H

#include "be_audio_private.h"

#ifdef REFKEEN_RESAMPLER_NONE
// Nothing to add here
#elif (defined REFKEEN_RESAMPLER_LIBSWRESAMPLE)
#include <libavutil/channel_layout.h>
//#include <libavutil/error.h> // av_err2str requires libavutil/libavutil-ffmpeg
#include <libswresample/swresample.h>
#elif (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
#include <libresample.h>
#elif (defined REFKEEN_RESAMPLER_LIBSOXR)
#include <soxr.h>
#elif (defined REFKEEN_RESAMPLER_LIBSPEEXDSP)
#include <speex/speex_resampler.h>
#elif (defined REFKEEN_RESAMPLER_LIBSAMPLERATE)
#include <samplerate.h>
#else
#error "FATAL ERROR: No recognized ReflectionHLE resampler macro is defined!"
#endif

typedef struct
{
	// Nearest-neighborhood sample rate conversion, used as
	// a simplistic alternative to any resampling library
	int *sampleRateConvTable;
	int sampleRateConvTableSize;
	// Current location in the conversion process;
	int sampleRateConvCurrIndex; // Index to sampleRateConvTable
	int sampleRateConvCounter; // Counter for current cell of g_sdlALSampleRateConvTable
	BE_ST_SndSample_T sampleRateConvLastValue; // Last input sample
#ifdef REFKEEN_RESAMPLER_NONE
	// Nothing to add here
#elif (defined REFKEEN_RESAMPLER_LIBSWRESAMPLE)
	struct SwrContext *swrContext;
#elif (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
	void *resampleHandle;
	double resampleFactor;
#elif (defined REFKEEN_RESAMPLER_LIBSOXR)
	soxr_t soxr;
#elif (defined REFKEEN_RESAMPLER_LIBSPEEXDSP)
	SpeexResamplerState *speexResamplerState;
#elif (defined REFKEEN_RESAMPLER_LIBSAMPLERATE)
	SRC_STATE *srcResampler;
	SRC_DATA srcData;
#endif
} BESDLResamplingContext;

void BEL_ST_InitResampling(BESDLResamplingContext *context,
                           int outSampleRate, int inSampleRate,
                           BE_ST_SndSample_T *dataInPtr);

void BEL_ST_ShutdownResampling(BESDLResamplingContext *context);

void BEL_ST_DoResample(BESDLResamplingContext *context,
                       uint32_t *outConsumed, uint32_t *outProduced,
                       BE_ST_SndSample_T *inPtr, BE_ST_SndSample_T *outPtr,
                       uint32_t numOfAvailInputSamples, uint32_t maxSamplesToOutput);

#endif
