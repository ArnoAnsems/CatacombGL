/* Copyright (C) 2014-2026 NY00123
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

#include "refkeen_config.h"

#include <SDL3/SDL.h>

#include "be_cross.h"
#include "be_st.h"
#include "be_st_sdl_private.h"
#include "be_audio_mixer.h"
#include "be_audio_private.h"

#ifdef REFKEEN_CONFIG_THREADS
static SDL_Mutex* g_sdlCallbackMutex = NULL;
#endif
static int g_sdlAudioChannels;
/*static*/ SDL_AudioStream *g_sdlAudioStream;

extern bool g_sdlAudioSubsystemUp;

#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
void BEL_ST_InterThread_CallBack(void *unused, Uint8 *stream, int len);
#endif

static void BEL_ST_MixerCallback(void *unused_userdata, SDL_AudioStream *stream,
                                 int additional_amount, int unused_total_amount)
{
	if (additional_amount > 0)
	{
		Uint8 *data = SDL_stack_alloc(Uint8, additional_amount);
		if (data)
		{
#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
			BEL_ST_InterThread_CallBack(unused_userdata, data,
			                            additional_amount);
#else
			BEL_ST_AudioMixerCallback((BE_ST_SndSample_T *)data,
			                          additional_amount / (g_sdlAudioChannels * sizeof(BE_ST_SndSample_T)));
#endif
			SDL_PutAudioStreamData(stream, data, additional_amount);
			SDL_stack_free(data);
		}
	}
}

static void BEL_ST_LowLatencySetup(const SDL_AudioSpec *spec)
{
	// As of SDL 3.4 the device sample frames hint doesn't scale based on
	// sample rate. SDL also at the time of writing has a minimum device
	// sample rate of 44.1kHz. We could assume the current logic, but we can
	// also open the device and query what sample rate we got. We do need to
	// re-open it with adjusted latency.
	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, spec);
	if (dev == 0)
	{
		BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "Cannot temporarily open SDL audio device for low latency probe,\n%s\n", SDL_GetError());
		return;
	}

	int sampleRate;
	SDL_AudioSpec devSpec;
	if (SDL_GetAudioDeviceFormat(dev, &devSpec, NULL))
	{
		sampleRate = devSpec.freq;
	}
	else
	{
		// If we can't query the actual frequency then we can try just assuming
		// we will get what we requested.
		sampleRate = spec->freq;
	}

	SDL_CloseAudioDevice(dev);

	// Hint that we want 700Hz callback timing
	char* sampleFramesStr = NULL;
	if (SDL_asprintf(&sampleFramesStr, "%d", sampleRate/700) != -1)
	{
		SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, sampleFramesStr);
		SDL_free(sampleFramesStr);
	}
}

bool BEL_ST_InitAudioSubsystem(int *freq, int *channels, int *bufferLen)
{
	SDL_AudioSpec spec;
	if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
	{
		BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "SDL audio system initialization failed,\n%s\n", SDL_GetError());
		return false;
	}
	spec.freq = g_refKeenCfg.sndSampleRate;
#ifdef MIXER_SAMPLE_FORMAT_FLOAT
	spec.format = SDL_AUDIO_F32;
#elif (defined MIXER_SAMPLE_FORMAT_SINT16)
	spec.format = SDL_AUDIO_S16;
#endif
	spec.channels = MIXER_DEFAULT_CHANNELS_COUNT;
	if (g_refKeenCfg.sndLowLatency)
	{
		BEL_ST_LowLatencySetup(&spec);
	}
	BE_Cross_LogMessage(BE_LOG_MSG_NORMAL, "Initializing audio subsystem, requested spec: freq %d, format %u, channels %d\n", (int)spec.freq, (unsigned int)spec.format, (int)spec.channels);
	g_sdlAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, BEL_ST_MixerCallback, 0);
	if (!g_sdlAudioStream)
	{
		BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "Cannot open SDL audio stream,\n%s\n", SDL_GetError());
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return false;
	}
#ifdef REFKEEN_CONFIG_THREADS
	g_sdlCallbackMutex = SDL_CreateMutex();
	if (!g_sdlCallbackMutex)
	{
		BE_Cross_LogMessage(BE_LOG_MSG_ERROR, "Cannot create recursive mutex for SDL audio callback,\n%s\nClosing SDL audio subsystem\n", SDL_GetError());
		SDL_DestroyAudioStream(g_sdlAudioStream);
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return false;
	}
#endif
	SDL_AudioSpec devSpec;
	int sampleFrames;
	if (!SDL_GetAudioDeviceFormat(SDL_GetAudioStreamDevice(g_sdlAudioStream), &devSpec, &sampleFrames))
	{
		devSpec = spec;
		sampleFrames = g_refKeenCfg.sndSampleRate / 64; // An approximation
	}
	BE_Cross_LogMessage(BE_LOG_MSG_NORMAL, "Audio subsystem initialized, audio driver: %s, service rate: ~%fHz\n", SDL_GetCurrentAudioDriver(), (double)devSpec.freq / sampleFrames);

	*freq = spec.freq;
	*channels = g_sdlAudioChannels = spec.channels;
	*bufferLen = sampleFrames;

	return true;
}

void BEL_ST_ShutdownAudioSubsystem(void)
{
	SDL_PauseAudioDevice(SDL_GetAudioStreamDevice(g_sdlAudioStream));
	BEL_ST_AudioMixerShutdown();
#ifdef REFKEEN_CONFIG_THREADS
	SDL_DestroyMutex(g_sdlCallbackMutex);
	g_sdlCallbackMutex = NULL;
#endif
	SDL_DestroyAudioStream(g_sdlAudioStream);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void BEL_ST_StartAudioSubsystem(void)
{
	SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(g_sdlAudioStream));
}

void BE_ST_LockAudioRecursively(void)
{
#ifdef REFKEEN_CONFIG_THREADS
	if (g_sdlAudioSubsystemUp)
		SDL_LockMutex(g_sdlCallbackMutex);
#endif
}

void BE_ST_UnlockAudioRecursively(void)
{
#ifdef REFKEEN_CONFIG_THREADS
	if (g_sdlAudioSubsystemUp)
		SDL_UnlockMutex(g_sdlCallbackMutex);
#endif
}
