#include <stdio.h>
#include <string.h>
#include "..\SDL\include\SDL.h"

#define BE_ST_MAXJOYSTICKS 8
#define BE_ST_EMU_JOYSTICK_RANGEMAX 5000 // id_in.c MaxJoyValue
#define BE_ST_DEFAULT_FARPTRSEGOFFSET 0x14

#if (defined REFKEEN_VER_CATARM) || (defined REFKEEN_VER_CATAPOC)
#define BE_ST_ENABLE_FARPTR_CFG 1
#endif

/*** Last BE_ST_PollEvents time ***/
static uint32_t g_sdlLastPollEventsTime;




extern SDL_Window *g_sdlWindow;

static uint8_t g_sdlLastKeyScanCodeBeforeAnyReset; // May be reset by BE_ST_BiosScanCode

void BE_ST_InitGfx(void);
void BE_ST_InitAudio(void);
void BE_ST_InitTiming(void);
void BE_ST_ShutdownAudio(void);
void BE_ST_ShutdownGfx(void);




SDL_Surface *g_be_sdl_windowIconSurface = NULL;

void BE_ST_InitCommon(void)
{
	if (SDL_Init(SDL_INIT_EVENTS|SDL_INIT_TIMER) < 0)
	{
		//BE_Cross_LogMessage(BE_LOG_MSG_ERROR, "SDL backend initialization failed,\n%s\n", SDL_GetError());
		exit(0);
	}
/*
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		//BE_Cross_LogMessage(BE_LOG_MSG_WARNING, "SDL game controller subsystem initialization (including joystick subsystem) failed, disabled,\n%s\n", SDL_GetError());
	}
    */
}

void BE_ST_PollEvents(void)
{
    //SDL_Event event;
    g_sdlLastPollEventsTime = SDL_GetTicks();


    // HACK - If audio subsystem is disabled we still want to at least
    // make the sound callback run (so e.g., no loop gets stuck waiting
    // for sound playback to complete)
    /*
    extern bool g_sdlAudioSubsystemUp;
    if (! g_sdlAudioSubsystemUp)
    {
        BE_ST_PrepareForManualAudioSDServiceCall();
    }
    */
}

void BE_ST_PrepareForGameStartup(void)
{
	BE_ST_InitAudio();
	BE_ST_InitTiming();
	BE_ST_PollEvents();
}

void BE_ST_ShutdownAll(void)
{
	BE_ST_ShutdownAudio();
	SDL_Quit();
}



