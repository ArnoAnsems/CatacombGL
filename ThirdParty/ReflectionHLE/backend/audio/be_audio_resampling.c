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
#include "be_audio_resampling.h"
#include <stdlib.h>

void BEL_ST_InitResampling(BESDLResamplingContext *context,
                           int outSampleRate, int inSampleRate,
                           BE_ST_SndSample_T *dataInPtr)
{
#ifndef REFKEEN_RESAMPLER_NONE
	if (g_refKeenCfg.useResampler)
	{
#ifndef REFKEEN_RESAMPLER_LIBRESAMPLE
		char errMsg[160];
#endif

#if (defined REFKEEN_RESAMPLER_LIBSWRESAMPLE)
		context->swrContext = swr_alloc_set_opts(
			NULL,                // allocating a new context
			AV_CH_LAYOUT_MONO,   // out channels layout
			AV_SAMPLE_FMT_S16,   // out format
			outSampleRate,       // out rate
			AV_CH_LAYOUT_MONO,   // in channels layout
			AV_SAMPLE_FMT_S16,   // in format
			inSampleRate,        // in rate
			0,
			NULL
		);
		//if (context->swrContext == NULL)
		//	BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: swr_alloc_set_opts failed!");
		int error = swr_init(context->swrContext);
		if (error != 0)
		{
			// av_err2str requires libavutil/libavutil-ffmpeg, so don't convert code to string
			snprintf(errMsg, sizeof(errMsg), "BE_ST_InitAudio: swr_init failed! Error code: %d", error);
			//BE_ST_ExitWithErrorMsg(errMsg);
		}
#elif (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
		context->resampleFactor = (double)outSampleRate/inSampleRate;
		context->resampleHandle = resample_open(0, context->resampleFactor, context->resampleFactor);
		//if (context->resampleHandle == NULL)
		//	BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: resample_open failed!");
#elif (defined REFKEEN_RESAMPLER_LIBSOXR)
		soxr_io_spec_t io_spec = soxr_io_spec(SOXR_INT16, SOXR_INT16);
		soxr_quality_spec_t q_spec = soxr_quality_spec(SOXR_LQ, 0); // Default quality spec adds an audible latency for resampling to 8000Hz
		soxr_error_t error;
		context->soxr = soxr_create(
			inSampleRate, // in rate
			outSampleRate, // out rate
			1, // channels
			&error,
			&io_spec,
			&q_spec,
			NULL // runtime spec
		);
		if (context->soxr == NULL)
		{
			snprintf(errMsg, sizeof(errMsg), "BE_ST_InitAudio: soxr_create failed!\nError: %s", soxr_strerror(error));
			//BE_ST_ExitWithErrorMsg(errMsg);
		}
#elif (defined REFKEEN_RESAMPLER_LIBSPEEXDSP)
		int error;
		context->speexResamplerState = speex_resampler_init(
			1, // channels
			inSampleRate, // in rate
			outSampleRate, // out rate
			0, // quality in the range 0-10 (10 is higher)
			&error
		);
		if (context->speexResamplerState == NULL)
		{
			snprintf(errMsg, sizeof(errMsg), "BE_ST_InitAudio: speex_resampler_init failed! Error code: %d\nError: %s", error, speex_resampler_strerror(error));
			//BE_ST_ExitWithErrorMsg(errMsg);
		}
#elif (defined REFKEEN_RESAMPLER_LIBSAMPLERATE)
		int error;
		context->srcResampler = src_new(SRC_SINC_FASTEST, 1, &error);
		if (context->srcResampler == NULL)
		{
			snprintf(errMsg, sizeof(errMsg), "BE_ST_InitAudio: src_new failed!\nError code: %d", error);
			//BE_ST_ExitWithErrorMsg(errMsg);
		}
		context->srcData.data_in = dataInPtr;
		context->srcData.src_ratio = (double)outSampleRate / inSampleRate;
#endif
	}
	else
#endif // REFKEEN_RESAMPLER_NONE
	{
		// The sum of all entries should be outSamplerate,
		// "uniformly" distributed over context->sdlALSampleRateConvTable
		context->sampleRateConvTable = (int *)malloc(sizeof(int) * inSampleRate);
		//if (context->sampleRateConvTable == NULL)
		//	BE_ST_ExitWithErrorMsg("BE_ST_InitAudio: Failed to allocate memory for sample rate conversion!");
		context->sampleRateConvTableSize = inSampleRate;
		for (int i = 0; i < inSampleRate; ++i)
		{
			// Using uint64_t cause an overflow is possible
			context->sampleRateConvTable[i] = ((uint64_t)(i+1)*(uint64_t)outSampleRate/inSampleRate)-(uint64_t)i*(uint64_t)outSampleRate/inSampleRate;
		}
		context->sampleRateConvCurrIndex = 0;
		context->sampleRateConvCounter = 0;
	}
}

void BEL_ST_ShutdownResampling(BESDLResamplingContext *context)
{
#ifndef REFKEEN_RESAMPLER_NONE
	if (g_refKeenCfg.useResampler)
	{
#if (defined REFKEEN_RESAMPLER_LIBSWRESAMPLE)
		swr_free(&context->swrContext);
#elif (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
		resample_close(context->resampleHandle);
#elif (defined REFKEEN_RESAMPLER_LIBSOXR)
		soxr_delete(context->soxr);
#elif (defined REFKEEN_RESAMPLER_LIBSPEEXDSP)
		speex_resampler_destroy(context->speexResamplerState);
#elif (defined REFKEEN_RESAMPLER_LIBSAMPLERATE)
		src_delete(context->srcResampler);
#endif
	}
	else
#endif // REFKEEN_RESAMPLER_NONE
		free(context->sampleRateConvTable);
}

void BEL_ST_DoResample(BESDLResamplingContext *context,
                       uint32_t *outConsumed, uint32_t *outProduced,
                       BE_ST_SndSample_T *inPtr, BE_ST_SndSample_T *outPtr,
                       uint32_t numOfAvailInputSamples, uint32_t maxSamplesToOutput)
{
	// Just some type differences
#ifdef REFKEEN_RESAMPLER_LIBSOXR
	size_t samples_consumed, samples_produced;
#elif (defined REFKEEN_RESAMPLER_NONE) || (defined REFKEEN_RESAMPLER_LIBSPEEXDSP) || (defined REFKEEN_RESAMPLER_LIBSAMPLERATE)
	uint32_t samples_consumed, samples_produced;
#elif (defined REFKEEN_RESAMPLER_LIBSWRESAMPLE) || (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
	int samples_consumed, samples_produced;
#endif

#ifndef REFKEEN_RESAMPLER_NONE
	if (g_refKeenCfg.useResampler)
	{
#if (defined REFKEEN_RESAMPLER_LIBSWRESAMPLE)
		samples_consumed = numOfAvailInputSamples;
		const uint8_t * inPtrs[] = { inPtr, NULL };
		uint8_t *outPtrs[] =  { outPtr, NULL };
		samples_produced = swr_convert(context->swrContext, outPtrs, maxSamplesToOutput, inPtrs, numOfAvailInputSamples);
#elif (defined REFKEEN_RESAMPLER_LIBRESAMPLE)
		samples_consumed = 0;
		samples_produced = resample_process(context->resampleHandle, context->resampleFactor, (float *)inPtr, numOfAvailInputSamples, 0, &samples_consumed, (float *)outPtr, maxSamplesToOutput);
#elif (defined REFKEEN_RESAMPLER_LIBSOXR)
		samples_consumed = samples_produced = 0;
		soxr_process(context->soxr, inPtr, numOfAvailInputSamples, &samples_consumed, outPtr, maxSamplesToOutput, &samples_produced);
#elif (defined REFKEEN_RESAMPLER_LIBSPEEXDSP)
		samples_consumed = numOfAvailInputSamples;
		samples_produced = maxSamplesToOutput;
		speex_resampler_process_int(context->speexResamplerState, 0, (spx_int16_t *)inPtr, &samples_consumed, (spx_int16_t *)outPtr, &samples_produced);
#elif (defined REFKEEN_RESAMPLER_LIBSAMPLERATE)
		context->srcData.data_out = (float *)outPtr;
		context->srcData.input_frames = numOfAvailInputSamples;
		context->srcData.output_frames = maxSamplesToOutput;
		src_process(context->srcResampler, &context->srcData);
		samples_consumed = context->srcData.input_frames_used;
		samples_produced = context->srcData.output_frames_gen;
#endif
	}
	else
#endif // REFKEEN_RESAMPLER_NONE
	{
		samples_consumed = samples_produced = 0;
		/*** Note: Casting to unsigned in order to suppress comparison-related warnings ***/
		while (((unsigned)samples_consumed < numOfAvailInputSamples) && ((unsigned)samples_produced < maxSamplesToOutput))
		{
			if (context->sampleRateConvCounter)
			{
				outPtr[samples_produced++] = context->sampleRateConvLastValue;
				--context->sampleRateConvCounter;
			}
			else
			{
				context->sampleRateConvLastValue = inPtr[samples_consumed++];
				context->sampleRateConvCounter = context->sampleRateConvTable[context->sampleRateConvCurrIndex];
				context->sampleRateConvCurrIndex = (context->sampleRateConvCurrIndex+1) % context->sampleRateConvTableSize;
			}
		}
	}
	*outConsumed = samples_consumed;
	*outProduced = samples_produced;
}
