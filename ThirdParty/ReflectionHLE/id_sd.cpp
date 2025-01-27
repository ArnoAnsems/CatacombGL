/* Catacomb 3-D Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include "id_sd.h"

extern "C" {
#include "be_st.h"
};

//	Global variables
bool		SoundSourcePresent,SoundBlasterPresent,AdLibPresent;
bool        NeedsMusic;
SDMode		SoundMode;
SMMode		MusicMode;

PCSound		**pcSoundTable;
AdlibSound ** AdlibSoundTable;

//	Internal variables
static	bool			SD_Started;
static	void			(*SoundUserHook)(void);

//	PC Sound variables
static	uint8_t			pcLastSample, *pcSound;
static	uint32_t		pcLengthLeft;
static	uint16_t			pcSoundLookup[255];

//	AdLib variables
static	bool			alNoCheck;
static	uint8_t			*alSound;
static	uint16_t			alBlock;
static	uint32_t		alLengthLeft;
static	uint32_t		alTimeCount;
static	AdlibSound*		alZeroInst = new AdlibSound();

// This table maps channel numbers to carrier and modulator op cells
static const uint8_t carriers[9] = { 3, 4, 5,11,12,13,19,20,21 };
static const uint8_t modifiers[9] = { 0, 1, 2, 8, 9,10,16,17,18 };

//	Sequencer variables
static	bool			sqActive;
static	uint16_t			alFXReg;
static	uint16_t			*sqHack, *sqHackPtr,sqHackLen,sqHackSeqLen;
static	uint32_t			sqHackTime;

//	Internal routines

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SetTimer0() - Sets system timer 0 to the specified speed
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_SetTimer0(uint16_t speed)
{
	BE_ST_SetTimer(speed); // HACK for different timing with music on

}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_SetIntsPerSec() - Uses SDL_SetTimer0() to set the number of
//		interrupts generated by system timer 0 per second
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_SetIntsPerSec(uint16_t ints)
{
	SDL_SetTimer0(1192030 / ints);
}

//
//	PC Sound code
//

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCPlaySound() - Plays the specified sound on the PC speaker
//
///////////////////////////////////////////////////////////////////////////
void
SDL_PCPlaySound(PCSound *sound)
{
	BE_ST_LockAudioRecursively();

	pcLastSample = -1;
	pcLengthLeft = sound->GetLength();
	pcSound = sound->GetData();

	BE_ST_UnlockAudioRecursively();
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCStopSound() - Stops the current sound playing on the PC Speaker
//
///////////////////////////////////////////////////////////////////////////

void
SDL_PCStopSound(void)
{
	BE_ST_LockAudioRecursively();

	pcSound = 0;
	BE_ST_PCSpeakerSetConstVal(0);

	BE_ST_UnlockAudioRecursively();
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_PCService() - Handles playing the next sample in a PC sound
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_PCService(void)
{
	uint8_t	s;
	uint16_t t;

	if (pcSound)
	{
		s = *pcSound++;
		if (s != pcLastSample)
		{
			BE_ST_LockAudioRecursively();

			pcLastSample = s;
			if (s)					// We have a frequency!
			{
				t = pcSoundLookup[s];
				BE_ST_PCSpeakerSetInvFreq(t);

			}
			else					// Time for some silence
			{
				BE_ST_PCSpeakerSetConstVal(0);
			}

			BE_ST_UnlockAudioRecursively();
		}

		if (!(--pcLengthLeft))
		{
			SDL_PCStopSound();
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutPC() - Turns off the pc speaker
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ShutPC(void)
{
	BE_ST_LockAudioRecursively();

	pcSound = 0;

	BE_ST_PCSpeakerSetConstVal(0);

	BE_ST_UnlockAudioRecursively();
}

// 	AdLib Code

///////////////////////////////////////////////////////////////////////////
//
//	alOut(n,b) - Puts b in AdLib card register n
//
///////////////////////////////////////////////////////////////////////////
void
alOut(uint8_t n,uint8_t b)
{
    BE_ST_OPL2Write(n, b);
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ALStopSound() - Turns off any sound effects playing through the
//		AdLib card
//
///////////////////////////////////////////////////////////////////////////

void
SDL_ALStopSound(void)
{
	BE_ST_LockAudioRecursively();

	alSound = 0;
	alOut(alFreqH + 0,0);

	BE_ST_UnlockAudioRecursively();
}

static void
SDL_AlSetFXInst(AdlibSound *inst)
{
	uint8_t		c,m;

	m = modifiers[0];
	c = carriers[0];
	alOut(m + alChar,inst->GetmChar());
	alOut(m + alScale,inst->GetmScale());
	alOut(m + alAttack,inst->GetmAttack());
	alOut(m + alSus,inst->GetmSus());
	alOut(m + alWave,inst->GetmWave());
	alOut(c + alChar,inst->GetcChar());
	alOut(c + alScale,inst->GetcScale());
	alOut(c + alAttack,inst->GetcAttack());
	alOut(c + alSus,inst->GetcSus());
	alOut(c + alWave,inst->GetcWave());
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ALPlaySound() - Plays the specified sound on the AdLib card
//
///////////////////////////////////////////////////////////////////////////
void
SDL_ALPlaySound(AdlibSound *sound)
{
	SDL_ALStopSound();

	BE_ST_LockAudioRecursively();

	alLengthLeft = sound->GetLength();
	alSound = sound->GetData();
	alBlock = ((sound->GetOctave() & 7) << 2) | 0x20;

	if (!(sound->GetmSus() | sound->GetcSus()))
	{
		BE_ST_UnlockAudioRecursively();
		//Quit("SDL_ALPlaySound() - Bad instrument");
	}

	SDL_AlSetFXInst(sound);

	BE_ST_UnlockAudioRecursively();
}

///////////////////////////////////////////////////////////////////////////
//
// 	SDL_ALSoundService() - Plays the next sample out through the AdLib card
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ALSoundService(void)
{
	uint8_t	s;

	if (alSound)
	{
		s = *alSound++;
		if (!s)
			alOut(alFreqH + 0,0);
		else
		{
			alOut(alFreqL + 0,s);
			alOut(alFreqH + 0,(uint8_t)alBlock);
		}

		if (!(--alLengthLeft))
		{
			alSound = 0;
			alOut(alFreqH + 0,0);
		}
	}
}
static void
SDL_ALService(void)
{
	uint8_t 	a,v;
	uint16_t	w;

	if (!sqActive)
		return;

	// REFKEEN - Looks like this the comparison is unsigned in original EXE
	while (sqHackLen && ((uint32_t)sqHackTime <= alTimeCount))
	{
		w = *sqHackPtr++;
		sqHackTime = alTimeCount + *sqHackPtr++;
		// REFKEEN - This is the case on Little and Big Endian altogether
		a = *((uint8_t *)&w);
		v = *((uint8_t *)&w + 1);

		alOut(a,v);
		sqHackLen -= 4;
	}
	alTimeCount++;
	if (!sqHackLen)
	{
		sqHackPtr = (uint16_t*)sqHack;
		sqHackLen = sqHackSeqLen;
		alTimeCount = sqHackTime = 0;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutAL() - Shuts down the AdLib card for sound effects
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_ShutAL(void)
{
	BE_ST_LockAudioRecursively();

	alOut(alEffects,0);
	alOut(alFreqH + 0,0);
	SDL_AlSetFXInst(alZeroInst);
	alSound = 0;

	BE_ST_UnlockAudioRecursively();
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_CleanAL() - Totally shuts down the AdLib card
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_CleanAL(void)
{
	uint8_t	i;

	BE_ST_LockAudioRecursively();

	alOut(alEffects,0);
	for (i = 1;i < 0xf5;i++)
		alOut(i,0);

	BE_ST_UnlockAudioRecursively();
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_StartAL() - Starts up the AdLib card for sound effects
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_StartAL(void)
{
	alFXReg = 0;
	alOut(alEffects, (uint8_t)alFXReg);
	SDL_AlSetFXInst(alZeroInst);
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_DetectAdLib() - Determines if there's an AdLib (or SoundBlaster
//		emulating an AdLib) present
//
///////////////////////////////////////////////////////////////////////////
static bool
SDL_DetectAdLib(void)
{
	// REFKEEN - If there's no emulated OPL chip, just return false
	if (!BE_ST_IsEmulatedOPLChipReady())
	{
		return false;
	}

	alOut(4,0x60);	// Reset T1 & T2
	alOut(4,0x80);	// Reset IRQ
	alOut(2,0xff);	// Set timer 1
	alOut(4,0x21);	// Start timer 1

	// We ALWAYS return true here, but maybe it's good
	// to send some commands to the emulated OPL chip

	alOut(4,0x60);
	alOut(4,0x80);

	{
		for (uint8_t i = 1;i <= 0xf5;i++)	// Zero all the registers
			alOut(i,0);

		alOut(1,0x20);	// Set WSE=1
		alOut(8,0);		// Set CSM=0 & SEL=0

		return(true);
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_t0Service() - My timer 0 ISR which handles the different timings and
//		dispatches to whatever other routines are appropriate
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_t0Service(void)
{
    static	int32_t	count = 1;

	if (MusicMode == smm_AdLib)
	{
		SDL_ALService();
		if (!(++count & 7))
		{
			if (SoundUserHook)
				SoundUserHook();
		}
		if (!(count & 3))
		{
			switch (SoundMode)
			{
			case sdm_PC:
				SDL_PCService();
				break;
			case sdm_AdLib:
				SDL_ALSoundService();
				break;
			}
		}
	}
	else
    {
		if (!(++count & 1))
		{
			if (SoundUserHook)
				SoundUserHook();
		}
		switch (SoundMode)
		{
		case sdm_PC:
			SDL_PCService();
			break;
		case sdm_AdLib:
			SDL_ALSoundService();
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//
//	SDL_ShutDevice() - turns off whatever device was being used for sound fx
//
////////////////////////////////////////////////////////////////////////////
static void
SDL_ShutDevice(void)
{
	switch (SoundMode)
	{
	case sdm_PC:
		SDL_ShutPC();
		break;
	case sdm_AdLib:
		SDL_ShutAL();
		break;
	}
	SoundMode = sdm_Off;
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_CleanDevice() - totally shuts down all sound devices
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_CleanDevice(void)
{
	if ((SoundMode == sdm_AdLib) || (MusicMode == smm_AdLib))
		SDL_CleanAL();
}

///////////////////////////////////////////////////////////////////////////
//
//	SDL_StartDevice() - turns on whatever device is to be used for sound fx
//
///////////////////////////////////////////////////////////////////////////
static void
SDL_StartDevice(void)
{
	switch (SoundMode)
	{
	case sdm_AdLib:
		SDL_StartAL();
		break;
	}
}

static void
SDL_SetTimerSpeed(void)
{
    int16_t rate;
	if (MusicMode == smm_AdLib)
		rate = TickBase * 8;
	else
	    rate = TickBase * 2;
	SDL_SetIntsPerSec(rate);
}

//	Public routines

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetSoundMode() - Sets which sound hardware to use for sound effects
//
///////////////////////////////////////////////////////////////////////////
bool
SD_SetSoundMode(SDMode mode)
{
	SD_StopSound();

	if (mode != SoundMode)
	{
		SDL_ShutDevice();
		SoundMode = mode;
		SDL_StartDevice();
	}

	SDL_SetTimerSpeed();

	return true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetMusicMode() - sets the device to use for background music
//
///////////////////////////////////////////////////////////////////////////
bool
SD_SetMusicMode(SMMode mode)
{
	bool result;

	switch (mode)
	{
	case smm_Off:
		NeedsMusic = false;
		result = true;
		break;
	case smm_AdLib:
		if (AdLibPresent)
		{
			NeedsMusic = true;
			result = true;
		}
		// (REFKEEN) Originally result was not set here to false, or anything, at all - undefined behaviors...
		else
		{
			result = false;
		}
		break;
	default:
		result = false;
		break;
	}

	if (result)
		MusicMode = mode;

	SDL_SetTimerSpeed();

	return(result);
}

SDMode
SD_GetSoundMode()
{
    return SoundMode;
}

SMMode
SD_GetMusicMode()
{
    return MusicMode;
}


///////////////////////////////////////////////////////////////////////////
//
//	SD_Startup() - starts up the Sound Mgr
//		Detects all additional sound hardware and installs my ISR
//
///////////////////////////////////////////////////////////////////////////
void
SD_Startup(void)
{
	if (SD_Started)
		return;

	SoundUserHook = 0;

    BE_ST_StartAudioAndTimerInt(&SDL_t0Service);
	alTimeCount = 0;

	SD_SetSoundMode(sdm_Off);
    SD_SetMusicMode(smm_Off);

    AdLibPresent = SDL_DetectAdLib();

	for (uint16_t i = 0;i < 255;i++)
		pcSoundLookup[i] = i * 60;

	SD_Started = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_Default() - Sets up the default behaviour for the Sound Mgr whether
//		the config file was present or not.
//
///////////////////////////////////////////////////////////////////////////
void
SD_Default(bool gotit,SDMode sd,SMMode sm)
{
	bool	gotsd,gotsm;

	gotsd = gotsm = gotit;

	if (gotsd)	// Make sure requested sound hardware is available
	{
		switch (sd)
		{
		case sdm_AdLib:
			gotsd = AdLibPresent;
			break;
		}
	}
	if (!gotsd)
	{
		if (AdLibPresent)
			sd = sdm_AdLib;
		else
			sd = sdm_PC;
	}
	if (sd != SoundMode)
		SD_SetSoundMode(sd);
	if (gotsm)	// Make sure requested music hardware is available
	{
		switch (sm)
		{
		case sdm_AdLib:
			gotsm = AdLibPresent;
			break;
		}
	}
	if (!gotsm)
	{
		if (AdLibPresent)
			sm = smm_AdLib;
	}

	if (sm != MusicMode)
		SD_SetMusicMode(sm);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_Shutdown() - shuts down the Sound Mgr
//		Removes sound ISR and turns off whatever sound hardware was active
//
///////////////////////////////////////////////////////////////////////////
void
SD_Shutdown(void)
{
	if (!SD_Started)
		return;

    BE_ST_StopAudioAndTimerInt();

    SD_MusicOff();
	SDL_ShutDevice();
	SDL_CleanDevice();

	BE_ST_LockAudioRecursively();

	SDL_SetTimer0(0);

// Do NOT call this here - A deadlock is a possibility (via recursive lock)
//	BE_ST_StopAudioSDService(void);
//	setvect(8,t0OldService);

	BE_ST_UnlockAudioRecursively();

	SD_Started = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetUserHook() - sets the routine that the Sound Mgr calls every 1/70th
//		of a second from its timer 0 ISR
//
///////////////////////////////////////////////////////////////////////////
void
SD_SetUserHook(void (* hook)(void))
{
	SoundUserHook = hook;
}


///////////////////////////////////////////////////////////////////////////
//
//	SD_SoundPlaying() - returns the sound number that's playing, or 0 if
//		no sound is playing
//
///////////////////////////////////////////////////////////////////////////
bool
SD_SoundPlaying(void)
{
	bool	result = false;

	switch (SoundMode)
	{
	case sdm_PC:
		result = pcSound? true : false;
		break;
	case sdm_AdLib:
		result = alSound? true : false;
		break;
	}

    return result;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
void
SD_StopSound(void)
{
	switch (SoundMode)
	{
	case sdm_PC:
		SDL_PCStopSound();
		break;
	case sdm_AdLib:
		SDL_ALStopSound();
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_WaitSoundDone() - waits until the current sound is done playing
//
///////////////////////////////////////////////////////////////////////////
void
SD_WaitSoundDone(void)
{
	while (SD_SoundPlaying())
	{
		BE_ST_ShortSleep();
	}
}


///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOn() - turns on the sequencer
//
///////////////////////////////////////////////////////////////////////////
void
SD_MusicOn(void)
{
	sqActive = true;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOff() - turns off the sequencer and any playing notes
//
///////////////////////////////////////////////////////////////////////////
void
SD_MusicOff(void)
{
	uint16_t	i;


	switch (MusicMode)
	{
	case smm_AdLib:
		alFXReg = 0;
		alOut(alEffects,0);
		for (i = 0;i < sqMaxTracks;i++)
			alOut(alFreqH + i + 1,0);
		break;
	}
	sqActive = false;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StartMusic() - starts playing the music pointed to
//
///////////////////////////////////////////////////////////////////////////
void
SD_StartMusic(FileChunk* music)
{
	SD_MusicOff();
	BE_ST_LockAudioRecursively();

	if (MusicMode == smm_AdLib)
	{
        sqHackPtr = sqHack = (uint16_t*)music->GetChunk();
        sqHackSeqLen = sqHackLen = (uint16_t)music->GetSize();
		sqHackTime = 0;
		alTimeCount = 0;
		SD_MusicOn();
	}

	BE_ST_UnlockAudioRecursively();
}