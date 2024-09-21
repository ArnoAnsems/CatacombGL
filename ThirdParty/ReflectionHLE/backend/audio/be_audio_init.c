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

#include "be_cross.h"
#include "be_sound_device_flags.h"
#include "be_st.h"
#include "be_st_sdl_private.h" // For BE_ST_FILL_AUDIO_IN_MAIN_THREAD
#include "be_audio_main_thread.h"
#include "be_audio_mixer.h"

bool g_sdlAudioSubsystemUp;
bool g_sdlAudioInitDone; // Even if audio subsystem isn't brought up

void BE_ST_InitAudio(void)
{
	g_sdlAudioSubsystemUp = false;
	g_sdlEmulatedOPLChipReady = false;
	int samplesForSourceBuffer;
	int audioDeviceFlags = BE_AUDIO_DEVICE_PCSPKR_REQUIRED | BE_AUDIO_DEVICE_OPL;
	int freq = 0, channels = 0, expectedCallbackBufferLen = 0;

	if (g_refKeenCfg.sndSubSystem &&
	    BEL_ST_InitAudioSubsystem(&freq, &channels, &expectedCallbackBufferLen))
	{
		// Must be set before calling BEL_ST_PrepareMainThreadForAudio
		g_sdlAudioSubsystemUp = true;
#ifdef BE_ST_FILL_AUDIO_IN_MAIN_THREAD
		samplesForSourceBuffer =
			BEL_ST_PrepareMainThreadForAudio(&freq, &channels, expectedCallbackBufferLen);
#else
		samplesForSourceBuffer = 2*expectedCallbackBufferLen;
#endif
	}
	else
		samplesForSourceBuffer = BEL_ST_PrepareMainThreadForAudio(&freq, &channels, 0);

	BEL_ST_AudioMixerInit(freq, channels);

	if (((audioDeviceFlags & BE_AUDIO_DEVICE_PCSPKR_REQUIRED) == BE_AUDIO_DEVICE_PCSPKR_REQUIRED) ||
	    (g_sdlAudioSubsystemUp && ((audioDeviceFlags & BE_AUDIO_DEVICE_PCSPKR) == BE_AUDIO_DEVICE_PCSPKR)))
	{
		BEL_ST_SetPCSpeakerSampleRate(freq);
		BEL_ST_SetPCSpeakerMixerSource(
			BEL_ST_AudioMixerAddSource(
				freq,
				samplesForSourceBuffer,
				g_refKeenCfg.pcSpkVol,
				BEL_ST_GenPCSpeakerSamples));
	}

	if (((audioDeviceFlags & BE_AUDIO_DEVICE_DIGI_REQUIRED) == BE_AUDIO_DEVICE_DIGI_REQUIRED) ||
	    (g_sdlAudioSubsystemUp && (g_refKeenCfg.sb != SOUNDBLASTER_OFF) &&
	     ((audioDeviceFlags & BE_AUDIO_DEVICE_DIGI) == BE_AUDIO_DEVICE_DIGI)))
		BEL_ST_SetDigiMixerSource(
			BEL_ST_AudioMixerAddSource(
				8000,
				samplesForSourceBuffer,
				g_refKeenCfg.digiVol,
				BEL_ST_GenDigiSamples));

	if (g_sdlAudioSubsystemUp && g_refKeenCfg.oplEmulation &&
	    ((audioDeviceFlags & BE_AUDIO_DEVICE_OPL) == BE_AUDIO_DEVICE_OPL))
	{
		BEL_ST_ResetOPLChip();
		BEL_ST_SetOPLMixerSource(
			BEL_ST_AudioMixerAddSource(
				OPL_SAMPLE_RATE,
				// Leave some room for calls to BE_ST_OPL2Write
				2*samplesForSourceBuffer,
				g_refKeenCfg.oplVol,
				BEL_ST_GenOPLSamples));
		g_sdlEmulatedOPLChipReady = true;
	}

	// Regardless of the audio subsystem being off or on, have *some*
	// rate set (being ~18.2Hz). In DEMOCAT from The Catacomb Abyss v1.13,
	// BE_ST_BSound may be called, so be ready to generate samples.
	BE_ST_SetTimer(0);

	g_sdlAudioInitDone = true;

	// As stated above, BE_ST_BSound may be called,
	// so better start generation of samples
	if (g_sdlAudioSubsystemUp)
		BEL_ST_StartAudioSubsystem();
}

void BE_ST_ShutdownAudio(void)
{
	g_sdlAudioInitDone = false;

	if (g_sdlAudioSubsystemUp)
		BEL_ST_ShutdownAudioSubsystem();
	g_sdlAudioSubsystemUp = false;

	BEL_ST_ClearMainThreadAudioResources();
}
