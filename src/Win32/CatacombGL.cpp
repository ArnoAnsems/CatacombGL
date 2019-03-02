// Copyright (C) 2018 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/ 

//
// This source file is the main entry point of the CatacombGL executable. It contains the WinMain function.
// It creates the OpenGL window and an instance of the EngineCore class. Then it runs the main game loop.
//

// Windows specific includes
#include <windows.h>
#include <gl\gl.h>
#include <Shlwapi.h>
#include <time.h>
#include "RendererOpenGLWin32.h"
#include "SystemWin32.h"

#include "..\Engine\EngineCore.h"
#include "..\Engine\PlayerInput.h"
#include "..\Engine\GameDetection.h"
#include "..\Engine\DefaultFont.h"
#include "..\Engine\GameSelection.h"

#include "..\Abyss\GameAbyss.h"
#include "..\Abyss\GameDetectionAbyss.h"
#include "..\Armageddon\GameArmageddon.h"
#include "..\Armageddon\GameDetectionArmageddon.h"

#include "..\..\ThirdParty\RefKeen\be_st.h"
#include "..\..\ThirdParty\RefKeen\id_sd.h"
#include "..\..\ThirdParty\SDL\include\SDL_mouse.h"
#include "..\..\ThirdParty\SDL\include\SDL.h"

bool	active = true;		// Window Active Flag

SDL_Window* SDLwindow = NULL;
SDL_Renderer* SDLrenderer = NULL;
SDL_GLContext glcontext = NULL;
ScreenMode m_screenMode = Windowed;

EngineCore* engineCore;
IGame* game;
RendererOpenGLWin32 renderer;
PlayerInput playerInput;
SystemWin32 systemWin32;

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	renderer.SetWindowDimensions(width, height);
}

bool GetCatacombsPackGOGPath(std::string& path)
{
    CHAR gog_catacombs_path[256];
    DWORD dwType = 0;
    DWORD dwSize = sizeof(gog_catacombs_path);
    LSTATUS status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GOGCATACOMBSPACK", "PATH", &dwType, gog_catacombs_path, &dwSize);
    const bool isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
    if (isGogCatacombsPathFound)
    {
        path.assign(std::string(gog_catacombs_path));
    }
    return isGogCatacombsPathFound;
}

GLvoid KillGLWindow(GLvoid)
{
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyRenderer(SDLrenderer);
    SDL_DestroyWindow(SDLwindow);
}
 
BOOL CreateGLWindow(int width, int height, int bits)
{
    SDL_Init(SDL_INIT_VIDEO);

    const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo();
    SDLwindow = SDL_CreateWindow(
        windowTitle.c_str(),                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        width,                               // width, in pixels
        height,                               // height, in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE                  // flags - see below
    );

    // Check that the window was successfully created
    if (SDLwindow == NULL) {
    // In the case that the window could not be made...
    printf("Could not create window: %s\n", SDL_GetError());
    return FALSE;
    }

    SDLrenderer = SDL_CreateRenderer(SDLwindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_ShowWindow(SDLwindow);

    glcontext = SDL_GL_CreateContext(SDLwindow);

    renderer.Setup();
    
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

    BE_ST_InitCommon();
    BE_ST_PrepareForGameStartup();

    SD_Startup();

	return TRUE;									// Success
}

void UpdatePlayerInput()
{
    SDL_PumpEvents();
    int numKeys;
    const Uint8 *state = SDL_GetKeyboardState(&numKeys);

    for (int i = 0; i < numKeys; i++)
    {
        playerInput.SetKeyPressed(SDL_GetKeyFromScancode((SDL_Scancode)(i)), state[i]);
    }

    int x, y;
    const uint32_t mouseState = SDL_GetRelativeMouseState(&x, &y);
    playerInput.SetMouseXPos(x);
    playerInput.SetMouseYPos(y);
    playerInput.SetMouseButtonPressed(SDL_BUTTON_LEFT, mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
    playerInput.SetMouseButtonPressed(SDL_BUTTON_MIDDLE, mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE));
    playerInput.SetMouseButtonPressed(SDL_BUTTON_RIGHT, mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));
    playerInput.SetHasFocus(SDL_GetMouseFocus() == SDLwindow);
}

void HandleWindowEvent(const SDL_WindowEvent * event)
{
    switch (event->event) {
    case SDL_WINDOWEVENT_RESIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        int w, h;
        SDL_GetWindowSize(SDLwindow, &w, &h);
        ReSizeGLScene(w, h);
        break;
    case SDL_WINDOWEVENT_CLOSE:
        active = false;
        break;
    }
}

void SetScreenMode(const ScreenMode screenMode)
{
    if (screenMode == Fullscreen)
    {
        // Set the width and height of the fullscreen to the width and height of the desktop area of the
        // display in use.
        const int displayIndex = SDL_GetWindowDisplayIndex(SDLwindow);
        SDL_Rect r;
        SDL_GetDisplayBounds(displayIndex, &r);
        const SDL_DisplayMode displayMode = { SDL_PIXELFORMAT_RGB24, r.w, r.h, 0, 0 };
        SDL_SetWindowDisplayMode(SDLwindow, &displayMode);
        SDL_SetWindowFullscreen(SDLwindow, SDL_WINDOW_FULLSCREEN);
    }
    else if (screenMode == BorderlessWindowed)
    {
        SDL_SetWindowFullscreen(SDLwindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else // Windowed
    {
        SDL_SetWindowFullscreen(SDLwindow, 0);
    }
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
    /* initialize random seed: */
    srand ((unsigned int)time(NULL));

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);

    const uint8_t GameIdNotDetected = 0;
    const uint8_t GameIdCatacombAbyssv113 = 1;
    const uint8_t GameIdCatacombAbyssv124 = 2;
    const uint8_t GameIdCatacombArmageddonv102 = 3;
    GameDetection gameDetectionAbyssV113;
    GameDetection gameDetectionAbyssV124;
    GameDetection gameDetectionArmageddonv102;

    uint8_t selectedGame = GameIdNotDetected;

    // Try to find shareware game files in local path
    gameDetectionAbyssV113.GetDetectionReport(GameIdCatacombAbyssv113, ".\\", abyssFilesv113);

    // Try to find registered game files in GOG path
    std::string gogPath;
    if (GetCatacombsPackGOGPath(gogPath))
    {
        const std::string gogArmageddonPath = gogPath + "Armageddon\\";
        gameDetectionArmageddonv102.GetDetectionReport(GameIdCatacombArmageddonv102, gogArmageddonPath, armageddonFiles);

        const std::string gogAbyssPath = gogPath + "Abyss\\";
        gameDetectionAbyssV124.GetDetectionReport(GameIdCatacombAbyssv124, gogAbyssPath, abyssFilesv124);
    }
 
    // Try to find registered game files in local path
    if (gameDetectionAbyssV124.GetBestMatch().score != 0)
    {
        gameDetectionAbyssV124.GetDetectionReport(GameIdCatacombAbyssv124, ".\\", abyssFilesv124);
    }
    if (gameDetectionArmageddonv102.GetBestMatch().score != 0)
    {
        gameDetectionArmageddonv102.GetDetectionReport(GameIdCatacombAbyssv124, ".\\", armageddonFiles);
    }

	// Create Our OpenGL Window
	if (!CreateGLWindow(800,600,16))
	{
		return 0;									// Quit If Window Was Not Created
	}

    std::string initialSearchFolder = SDL_GetBasePath();
    GameSelection gameSelection(renderer);

    GameSelectionPresentation gameSelectionPresentation;
    gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("1. Catacomb 3-D: The Descent v1.22", NotSupported));
    const GameDetectionState abyssv124DetectionState = (gameDetectionAbyssV124.GetBestMatch().score == 0) ? Detected : NotDetected;
    gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("2. Catacomb Abyss v1.24", abyssv124DetectionState));
    const GameDetectionState armageddonDetectionState = (gameDetectionArmageddonv102.GetBestMatch().score == 0) ? Detected : NotDetected;
    gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("3. Catacomb Armageddon v1.02", armageddonDetectionState));
    gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("4. Catacomb Apocalypse v1.01", NotSupported));

    gameSelectionPresentation.gameListShareware.push_back(std::make_pair("5. Catacomb Abyss v1.12", NotSupported));
    const GameDetectionState abyssv133DetectionState = (gameDetectionAbyssV113.GetBestMatch().score == 0) ? Detected : NotDetected;
    gameSelectionPresentation.gameListShareware.push_back(std::make_pair("6. Catacomb Abyss v1.13", abyssv133DetectionState));

    gameSelectionPresentation.searchFolder = initialSearchFolder;

    while (selectedGame == GameIdNotDetected && active)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                HandleWindowEvent(&event.window);
            }
        }

        gameSelection.Draw(gameSelectionPresentation);
        SDL_GL_SwapWindow(SDLwindow);

        UpdatePlayerInput();
        if (playerInput.IsKeyPressed(SDLK_6))
        {
            selectedGame = GameIdCatacombAbyssv113;
        }

        if (playerInput.IsKeyPressed(SDLK_2))
        {
            selectedGame = GameIdCatacombAbyssv124;
        }

        if (playerInput.IsKeyPressed(SDLK_3))
        {
            selectedGame = GameIdCatacombArmageddonv102;
        }
    }

    const DetectionReport& report =
        (selectedGame == GameIdCatacombAbyssv113) ? gameDetectionAbyssV113.GetBestMatch() :
        (selectedGame == GameIdCatacombAbyssv124) ? gameDetectionAbyssV124.GetBestMatch() :
        gameDetectionArmageddonv102.GetBestMatch();

    if (report.score == 0)
    {
        if (report.gameId == GameIdCatacombArmageddonv102)
        {
            game = new GameArmageddon(report.folder, renderer);
        }
        else
        {
            game = new GameAbyss(report.gameId, report.folder, renderer);
        }
    }
    else
    {
        const std::string errorMessage = "Failed to detect game files! Please make sure the Catacombs Pack [GOG.com] is correctly installed. Alternatively, the (shareware) game files can be placed in the same folder as CatacombGL.exe. Detailed info: " + report.infoString;
        MessageBox(NULL, errorMessage.c_str(), "CatacombGL ERROR", MB_OK | MB_ICONERROR);
        return 0;
    }

    engineCore = new EngineCore(*game, systemWin32, playerInput);

    // Update the window title with the selected game info.
    const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo() + " [" + game->GetName() + "]";
    SDL_SetWindowTitle(SDLwindow, windowTitle.c_str());

	while(active)									// Loop That Runs While done=FALSE
	{
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                HandleWindowEvent(&event.window);
            }
        }

        if (engineCore->Think())
        {
            active = false;
        }
		else								// Not Time To Quit, Update Screen
		{
            UpdatePlayerInput();
            if (m_screenMode != engineCore->GetScreenMode())
            {
                m_screenMode = engineCore->GetScreenMode();
                SetScreenMode(m_screenMode);
            }
            SDL_SetRelativeMouseMode(engineCore->RequiresMouseCapture() ? SDL_TRUE : SDL_FALSE);
            engineCore->DrawScene(renderer);
            SDL_GL_SwapWindow(SDLwindow);
		}
	}

    // Kill The Window
    KillGLWindow();

	// Shutdown
    SD_Shutdown();
    BE_ST_ShutdownAll();
						
    delete engineCore;
    delete game;

    return TRUE;
}
