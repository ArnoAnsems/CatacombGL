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

#include "refkeen_config.h"

#include "SDL.h"

#include "be_cross.h"
#include "be_st.h"
#include "be_st_sdl_private.h"
#include "be_audio_mixer.h"
#include "be_audio_private.h"

#ifdef REFKEEN_CONFIG_THREADS
static SDL_mutex* g_sdlCallbackMutex = NULL;
#endif
static int g_sdlAudioChannels;
/*static*/ SDL_AudioDeviceID g_sdlAudioDevice;

extern bool g_sdlAudioSubsystemUp;

#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
void BEL_ST_InterThread_CallBack(void *unused, Uint8 *stream, int len);
#endif

static void BEL_ST_MixerCallback(void *unused, Uint8 *stream, int len)
{
	BEL_ST_AudioMixerCallback((BE_ST_SndSample_T *)stream,
	                          len / (g_sdlAudioChannels * sizeof(BE_ST_SndSample_T)));
}

bool BEL_ST_InitAudioSubsystem(int *freq, int *channels, int *bufferLen)
{
	SDL_AudioSpec desiredSpec, obtainedSpec;
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "SDL audio system initialization failed,\n%s\n", SDL_GetError());
		return false;
	}
	desiredSpec.freq = g_refKeenCfg.sndSampleRate;
#ifdef MIXER_SAMPLE_FORMAT_FLOAT
	desiredSpec.format = AUDIO_F32SYS;
#elif (defined MIXER_SAMPLE_FORMAT_SINT16)
	desiredSpec.format = AUDIO_S16SYS;
#endif
	desiredSpec.channels = MIXER_DEFAULT_CHANNELS_COUNT;
	// Should be some power-of-two roughly proportional to the sample rate; Using 1024 for 48000Hz.
	for (desiredSpec.samples = 1; desiredSpec.samples < g_refKeenCfg.sndSampleRate/64; desiredSpec.samples *= 2)
		;

#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
	desiredSpec.callback = BEL_ST_InterThread_CallBack;
#else
	desiredSpec.callback = BEL_ST_MixerCallback;
#endif

	desiredSpec.userdata = NULL;
	BE_Cross_LogMessage(BE_LOG_MSG_NORMAL, "Initializing audio subsystem, requested spec: freq %d, format %u, channels %d, samples %u\n", (int)desiredSpec.freq, (unsigned int)desiredSpec.format, (int)desiredSpec.channels, (unsigned int)desiredSpec.samples);
	g_sdlAudioDevice = SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	if (g_sdlAudioDevice <= 0)
	{
		BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "Cannot open SDL audio device,\n%s\n", SDL_GetError());
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return false;
	}
#ifdef REFKEEN_CONFIG_THREADS
	g_sdlCallbackMutex = SDL_CreateMutex();
	if (!g_sdlCallbackMutex)
	{
		BE_Cross_LogMessage(BE_LOG_MSG_ERROR, "Cannot create recursive mutex for SDL audio callback,\n%s\nClosing SDL audio subsystem\n", SDL_GetError());
		SDL_CloseAudioDevice(g_sdlAudioDevice);
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return false;
	}
#endif
	BE_Cross_LogMessage(BE_LOG_MSG_NORMAL, "Audio subsystem initialized, received spec: freq %d, format %u, channels %d, samples %u, size %u\n", (int)obtainedSpec.freq, (unsigned int)obtainedSpec.format, (int)obtainedSpec.channels, (unsigned int)obtainedSpec.samples, (unsigned int)obtainedSpec.size);

	// Size may be reported as "0" on Android
	*freq = obtainedSpec.freq;
	*channels = g_sdlAudioChannels = obtainedSpec.channels;
	*bufferLen = obtainedSpec.size ?
	             (obtainedSpec.size / sizeof(BE_ST_SndSample_T)) : obtainedSpec.samples;
	return true;
}

void BEL_ST_ShutdownAudioSubsystem(void)
{
	SDL_PauseAudioDevice(g_sdlAudioDevice, 1);
	BEL_ST_AudioMixerShutdown();
#ifdef REFKEEN_CONFIG_THREADS
	SDL_DestroyMutex(g_sdlCallbackMutex);
	g_sdlCallbackMutex = NULL;
#endif
	SDL_CloseAudioDevice(g_sdlAudioDevice);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void BEL_ST_StartAudioSubsystem(void)
{
	SDL_PauseAudioDevice(g_sdlAudioDevice, 0);
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
