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
#include <Shlwapi.h>
#include <time.h>
#include "RendererOpenGLWin32.h"
#include "SystemWin32.h"

#include "..\Engine\EngineCore.h"
#include "..\Engine\PlayerInput.h"
#include "..\Engine\GameDetection.h"
#include "..\Engine\DefaultFont.h"
#include "..\Engine\GameSelection.h"
#include "..\Engine\ConfigurationSettings.h"
#include "..\Engine\Logging.h"
#include "..\Engine\Console.h"

#include "..\Abyss\GameAbyss.h"
#include "..\Abyss\GameDetectionAbyss.h"
#include "..\Armageddon\GameArmageddon.h"
#include "..\Armageddon\GameDetectionArmageddon.h"
#include "..\Apocalypse\GameApocalypse.h"
#include "..\Apocalypse\GameDetectionApocalypse.h"
#include "..\Catacomb3D\GameCatacomb3D.h"
#include "..\Catacomb3D\GameDetectionCatacomb3D.h"

#include "..\..\ThirdParty\RefKeen\be_st.h"
#include "..\..\ThirdParty\RefKeen\id_sd.h"
#include "..\..\ThirdParty\SDL\include\SDL_mouse.h"
#include "..\..\ThirdParty\SDL\include\SDL.h"

bool	active = true;		// Window Active Flag

SDL_Window* SDLwindow = nullptr;
SDL_GLContext glcontext = nullptr;
ScreenMode m_screenMode = Windowed;

EngineCore* engineCore = nullptr;
IGame* game = nullptr;
RendererOpenGLWin32* m_renderer;
PlayerInput playerInput;
SystemWin32 systemWin32;
ConfigurationSettings m_configurationSettings;
Console* m_console;

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	m_renderer->SetWindowDimensions(width, height);
}

bool GetCatacombsPackGOGPath(std::string& path)
{
    CHAR gog_catacombs_path[256];
    memset(gog_catacombs_path, 0, 256);
    DWORD dwType = 0;
    DWORD dwSize = sizeof(gog_catacombs_path);
    // Check GOG Catacombs Pack - Legacy path
    LSTATUS status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GOGCATACOMBSPACK", "PATH", &dwType, gog_catacombs_path, &dwSize);
    bool isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
    if (isGogCatacombsPathFound)
    {
        path.assign(std::string(gog_catacombs_path));
    }
    else
    {
        // Check GOG Catacombs Pack - Product ID 1207659189
        status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GAMES\\1207659189", "PATH", &dwType, gog_catacombs_path, &dwSize);
        isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
        if (isGogCatacombsPathFound)
        {
            path.assign(std::string(gog_catacombs_path));
        }
    }

    if (path.length() > 0)
    {
        if (path.at(path.length() - 1) != '\\')
        {
            path.append("\\");
        }
    }
    return isGogCatacombsPathFound;
}

GLvoid KillGLWindow(GLvoid)
{
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(SDLwindow);
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
 
void CreateGLWindow(int width, int height, int bits)
{
    Logging::Instance().AddLogMessage("Initializing OpenGL renderer");

    if (SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1) != 0)
    {
        Logging::Instance().AddLogMessage("WARNING: call to SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,1) failed: " + std::string(SDL_GetError()));
    }

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
    if (SDLwindow == nullptr)
    {
        // In the case that the window could not be made...
        Logging::Instance().FatalError("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    SDL_ShowWindow(SDLwindow);

    glcontext = SDL_GL_CreateContext(SDLwindow);
    if (glcontext == nullptr)
    {
        Logging::Instance().FatalError("SDL_GL_CreateContext failed: " + std::string(SDL_GetError()));
    }

    m_renderer = new RendererOpenGLWin32();
    m_renderer->Setup();
    SetScreenMode(m_configurationSettings.GetScreenMode());
    
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

    Logging::Instance().AddLogMessage("Running on graphics adapter model " + m_renderer->GetGraphicsAdapterModel());
    Logging::Instance().AddLogMessage("Running on " + m_renderer->GetGraphicsApiVersion());

    BE_ST_InitAudio();

    SD_Startup();
}

void UpdatePlayerInput()
{
    SDL_PumpEvents();
    int numKeys = 0;
    const Uint8 *state = SDL_GetKeyboardState(&numKeys);

    for (int i = 0; i < numKeys; i++)
    {
        playerInput.SetKeyPressed(SDL_GetKeyFromScancode((SDL_Scancode)(i)), state[i]);
    }

    int x = 0;
    int y = 0;
    const uint32_t mouseState = SDL_GetRelativeMouseState(&x, &y);
    playerInput.SetMouseXPos(x);
    playerInput.SetMouseYPos(y);
    playerInput.SetMouseButtonPressed(SDL_BUTTON_LEFT, mouseState & SDL_BUTTON_LMASK);
    playerInput.SetMouseButtonPressed(SDL_BUTTON_MIDDLE, mouseState & SDL_BUTTON_MMASK);
    playerInput.SetMouseButtonPressed(SDL_BUTTON_RIGHT, mouseState & SDL_BUTTON_RMASK);
    playerInput.SetMouseButtonPressed(SDL_BUTTON_X1, mouseState & SDL_BUTTON_X1MASK);
    playerInput.SetMouseButtonPressed(SDL_BUTTON_X2, mouseState & SDL_BUTTON_X2MASK);
    playerInput.SetHasFocus(SDL_GetMouseFocus() == SDLwindow);
}

void HandleWindowEvent(const SDL_WindowEvent * event)
{
    switch (event->event) {
    case SDL_WINDOWEVENT_RESIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
    {
        int w = 0;
        int h = 0;
        SDL_GetWindowSize(SDLwindow, &w, &h);
        ReSizeGLScene(w, h);
        break;
    }
    case SDL_WINDOWEVENT_CLOSE:
        active = false;
        break;
    }
}

void GetSubFolders(const std::string selectedFolder, std::vector<std::string>& subFolders)
{
    subFolders.clear();
    if (selectedFolder.empty())
    {
        // No search folder. List all the logical drives instead.
        uint32_t logicalDrives = GetLogicalDrives();
        for (uint32_t driveIndex = 0; driveIndex < 26; driveIndex++)
        {
            const uint32_t logicalDriveBit = 1 << driveIndex;
            if (logicalDrives & logicalDriveBit)
            {
                char logicalDriveName[3];
                logicalDriveName[0] = 'A' + driveIndex;
                logicalDriveName[1] = ':';
                logicalDriveName[2] = 0;
                subFolders.push_back(logicalDriveName);
            }
        }
    }
    else
    {
        subFolders.push_back(".."); // Make sure the "return to parent" folder ends up at the top of the list.

        std::string ffsearchFolder = selectedFolder + '*';
        WIN32_FIND_DATA findData;
        memset(&findData,0, sizeof(findData));
        HANDLE searchHandle = FindFirstFileEx(ffsearchFolder.c_str(), FindExInfoBasic, &findData, FindExSearchLimitToDirectories, nullptr, 0);

        if (searchHandle != nullptr)
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
            {
                subFolders.push_back(findData.cFileName);
            }

            while (FindNextFile(searchHandle, &findData))
            {
                if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
                {
                    subFolders.push_back(findData.cFileName);
                }
            }
            FindClose(searchHandle);
        }
    }
}

void InitializeSDL()
{
    SDL_version sdlVersion;
    memset(&sdlVersion, 0, sizeof(sdlVersion));
    SDL_GetVersion(&sdlVersion);
    const std::string sdlLogMessage =
        "Initializing SDL " +
        std::to_string(sdlVersion.major) + "." +
        std::to_string(sdlVersion.minor) + "." +
        std::to_string(sdlVersion.patch);
    Logging::Instance().AddLogMessage(sdlLogMessage);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        Logging::Instance().FatalError("SDL_Init failed: " + std::string(SDL_GetError()));
    }
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
    /* initialize random seed: */
    srand ((unsigned int)time(nullptr));

    const std::string filenamePath = systemWin32.GetConfigurationFilePath();
    systemWin32.CreatePath(filenamePath);

    const std::string logFilename = filenamePath + "CatacombGL_log.txt";
    Logging::Instance().SetLogFile(logFilename);
    m_console = new Console("CatacombGL " + EngineCore::GetVersionInfo());

    const std::string buildBitInfo(systemWin32.isBuiltIn64Bit() ? " (64 bit)" : " (32 bit)");
    Logging::Instance().AddLogMessage("Initializing CatacombGL " + EngineCore::GetVersionInfo() + buildBitInfo);

    Logging::Instance().AddLogMessage("Running on " + systemWin32.GetOSVersion());

    const std::string configFilename = filenamePath + "CatacombGL.ini";
    Logging::Instance().AddLogMessage("Loading CatacombGL.ini");
    m_configurationSettings.LoadFromFile(configFilename);

    InitializeSDL();

    const uint8_t GameIdNotDetected = 0;
    const uint8_t GameIdCatacombAbyssv113 = 1;
    const uint8_t GameIdCatacombAbyssv124 = 2;
    const uint8_t GameIdCatacombArmageddonv102 = 3;
    const uint8_t GameIdCatacombApocalypsev101 = 4;
    const uint8_t GameIdCatacomb3Dv122 = 5;
    GameDetection gameDetectionAbyssV113;
    GameDetection gameDetectionAbyssV124;
    GameDetection gameDetectionArmageddonv102;
    GameDetection gameDetectionApocalypsev101;
    GameDetection gameDetectionCatacomb3Dv122;

    uint8_t selectedGame = GameIdNotDetected;

    // Try to find shareware game files in local path
    gameDetectionAbyssV113.GetDetectionReport(GameIdCatacombAbyssv113, ".\\", abyssFilesv113);

    // Try to find registered game files in GOG path
    std::string gogPath;
    if (GetCatacombsPackGOGPath(gogPath))
    {
        Logging::Instance().AddLogMessage("Catacombs Pack is present in Windows registry: " + gogPath);
        const std::string gogArmageddonPath = gogPath + "Armageddon\\";
        gameDetectionArmageddonv102.GetDetectionReport(GameIdCatacombArmageddonv102, gogArmageddonPath, armageddonFiles);

        const std::string gogApocalypsePath = gogPath + "Apocalypse\\";
        gameDetectionApocalypsev101.GetDetectionReport(GameIdCatacombApocalypsev101, gogApocalypsePath, apocalypseFiles);

        const std::string gogAbyssPath = gogPath + "Abyss\\";
        gameDetectionAbyssV124.GetDetectionReport(GameIdCatacombAbyssv124, gogAbyssPath, abyssFilesv124);

        const std::string gogCatacomb3DPath = gogPath + "Cat3D\\";
        gameDetectionCatacomb3Dv122.GetDetectionReport(GameIdCatacomb3Dv122, gogCatacomb3DPath, catacomb3DFiles);
    }
    else
    {
        Logging::Instance().AddLogMessage("Catacombs Pack is not present in Windows registry");
    }

    // Try to find game files via configuration file
    if (gameDetectionAbyssV113.GetBestMatch().score != 0 && !m_configurationSettings.GetPathAbyssv113().empty())
    {
        gameDetectionAbyssV113.GetDetectionReport(GameIdCatacombAbyssv113, m_configurationSettings.GetPathAbyssv113(), abyssFilesv113);
    }

    if (gameDetectionAbyssV124.GetBestMatch().score != 0 && !m_configurationSettings.GetPathAbyssv124().empty())
    {
        gameDetectionAbyssV124.GetDetectionReport(GameIdCatacombAbyssv124, m_configurationSettings.GetPathAbyssv124(), abyssFilesv124);
    }

    if (gameDetectionArmageddonv102.GetBestMatch().score != 0 && !m_configurationSettings.GetPathArmageddonv102().empty())
    {
        gameDetectionArmageddonv102.GetDetectionReport(GameIdCatacombArmageddonv102, m_configurationSettings.GetPathArmageddonv102(), armageddonFiles);
    }

    if (gameDetectionApocalypsev101.GetBestMatch().score != 0 && !m_configurationSettings.GetPathApocalypsev101().empty())
    {
        gameDetectionApocalypsev101.GetDetectionReport(GameIdCatacombApocalypsev101, m_configurationSettings.GetPathApocalypsev101(), apocalypseFiles);
    }

    if (gameDetectionCatacomb3Dv122.GetBestMatch().score != 0 && !m_configurationSettings.GetPathCatacomb3Dv122().empty())
    {
        gameDetectionCatacomb3Dv122.GetDetectionReport(GameIdCatacomb3Dv122, m_configurationSettings.GetPathCatacomb3Dv122(), catacomb3DFiles);
    }
 
    // Try to find registered game files in local path
    if (gameDetectionAbyssV124.GetBestMatch().score != 0)
    {
        gameDetectionAbyssV124.GetDetectionReport(GameIdCatacombAbyssv124, ".\\", abyssFilesv124);
    }
    if (gameDetectionArmageddonv102.GetBestMatch().score != 0)
    {
        gameDetectionArmageddonv102.GetDetectionReport(GameIdCatacombArmageddonv102, ".\\", armageddonFiles);
    }

    if (gameDetectionApocalypsev101.GetBestMatch().score != 0)
    {
        gameDetectionApocalypsev101.GetDetectionReport(GameIdCatacombApocalypsev101, ".\\", apocalypseFiles);
    }

    if (gameDetectionCatacomb3Dv122.GetBestMatch().score != 0)
    {
        gameDetectionCatacomb3Dv122.GetDetectionReport(GameIdCatacomb3Dv122, ".\\", catacomb3DFiles);
    }

    if (gameDetectionAbyssV113.GetBestMatch().score == 0)
    {
        Logging::Instance().AddLogMessage("Catacomb Abyss v1.13 detected at " + gameDetectionAbyssV113.GetBestMatch().folder);
    }
    else
    {
        Logging::Instance().AddLogMessage("Catacomb Abyss v1.13 not detected");
    }
    
    if (gameDetectionAbyssV124.GetBestMatch().score == 0)
    {
        Logging::Instance().AddLogMessage("Catacomb Abyss v1.24 detected at " + gameDetectionAbyssV124.GetBestMatch().folder);
    }
    else
    {
        Logging::Instance().AddLogMessage("Catacomb Abyss v1.24 not detected");
    }

    if (gameDetectionArmageddonv102.GetBestMatch().score == 0)
    {
        Logging::Instance().AddLogMessage("Catacomb Armageddon v1.02 detected at " + gameDetectionArmageddonv102.GetBestMatch().folder);
    }
    else
    {
        Logging::Instance().AddLogMessage("Catacomb Armageddon v1.02 not detected");
    }

    if (gameDetectionApocalypsev101.GetBestMatch().score == 0)
    {
        Logging::Instance().AddLogMessage("Catacomb Apocalypse v1.01 detected at " + gameDetectionApocalypsev101.GetBestMatch().folder);
    }
    else
    {
        Logging::Instance().AddLogMessage("Catacomb Apocalypse v1.01 not detected");
    }

    if (gameDetectionCatacomb3Dv122.GetBestMatch().score == 0)
    {
        Logging::Instance().AddLogMessage("Catacomb 3-D v1.22 detected at " + gameDetectionCatacomb3Dv122.GetBestMatch().folder);
    }
    else
    {
        Logging::Instance().AddLogMessage("Catacomb 3-D v1.22 not detected");
    }

	// Create Our OpenGL Window
    CreateGLWindow(800, 600, 16);

    std::string initialSearchFolder = SDL_GetBasePath();
    GameSelection gameSelection(*m_renderer);

    GameSelectionPresentation gameSelectionPresentation;
    gameSelectionPresentation.searchFolder = initialSearchFolder;
    gameSelectionPresentation.selectedSubFolder = 0;
    gameSelectionPresentation.subFolderOffset = 0;

    GetSubFolders(initialSearchFolder, gameSelectionPresentation.subFolders);

    while (selectedGame == GameIdNotDetected && active)
    {
        SDL_Event event;
        memset(&event, 0, sizeof(event));
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_WINDOWEVENT)
            {
                HandleWindowEvent(&event.window);
            }
        }

        if (gameSelectionPresentation.gameListCatacombsPack.empty())
        {
            const GameDetectionState catacomb3Dv122DetectionState = (gameDetectionCatacomb3Dv122.GetBestMatch().score == 0) ? Detected : NotDetected;
            gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("1. Catacomb 3-D: The Descent v1.22", catacomb3Dv122DetectionState));
            const GameDetectionState abyssv124DetectionState = (gameDetectionAbyssV124.GetBestMatch().score == 0) ? Detected : NotDetected;
            gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("2. Catacomb Abyss v1.24", abyssv124DetectionState));
            const GameDetectionState armageddonDetectionState = (gameDetectionArmageddonv102.GetBestMatch().score == 0) ? Detected : NotDetected;
            gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("3. Catacomb Armageddon v1.02", armageddonDetectionState));
            const GameDetectionState apocalypseDetectionState = (gameDetectionApocalypsev101.GetBestMatch().score == 0) ? Detected : NotDetected;
            gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("4. Catacomb Apocalypse v1.01", apocalypseDetectionState));
        }

        if (gameSelectionPresentation.gameListShareware.empty())
        {
            gameSelectionPresentation.gameListShareware.push_back(std::make_pair("5. Catacomb Abyss v1.12", NotSupported));
            const GameDetectionState abyssv133DetectionState = (gameDetectionAbyssV113.GetBestMatch().score == 0) ? Detected : NotDetected;
            gameSelectionPresentation.gameListShareware.push_back(std::make_pair("6. Catacomb Abyss v1.13", abyssv133DetectionState));
        }

        IRenderer::FrameSettings frameSettings;
        frameSettings.playerAngle = 0.0f;
        frameSettings.playerPosX = 0.0f;
        frameSettings.playerPosY = 0.0f;
        frameSettings.textureFilter = m_configurationSettings.GetTextureFilter();
        frameSettings.vSyncEnabled = m_configurationSettings.GetVSync();
        m_renderer->SetFrameSettings(frameSettings);

        gameSelection.Draw(gameSelectionPresentation);
        m_console->Draw(*m_renderer);
        SDL_GL_SwapWindow(SDLwindow);

        UpdatePlayerInput();

        if (playerInput.IsKeyPressed(SDLK_1))
        {
            selectedGame = GameIdCatacomb3Dv122;
        }

        if (playerInput.IsKeyPressed(SDLK_2))
        {
            selectedGame = GameIdCatacombAbyssv124;
        }

        if (playerInput.IsKeyPressed(SDLK_3))
        {
            selectedGame = GameIdCatacombArmageddonv102;
        }

        if (playerInput.IsKeyPressed(SDLK_4))
        {
            selectedGame = GameIdCatacombApocalypsev101;
        }

        if (playerInput.IsKeyPressed(SDLK_6))
        {
            selectedGame = GameIdCatacombAbyssv113;
        }

        if (playerInput.IsKeyJustPressed(SDLK_UP))
        {
            if (gameSelectionPresentation.selectedSubFolder == 0)
            {
                gameSelectionPresentation.selectedSubFolder = (uint32_t)gameSelectionPresentation.subFolders.size() - 1;
                gameSelectionPresentation.subFolderOffset = ((uint32_t)gameSelectionPresentation.subFolders.size() > 4) ? (uint32_t)gameSelectionPresentation.subFolders.size() - 4 : 0;
            }
            else
            {
                gameSelectionPresentation.selectedSubFolder--;
                if (gameSelectionPresentation.selectedSubFolder < gameSelectionPresentation.subFolderOffset)
                {
                    gameSelectionPresentation.subFolderOffset = gameSelectionPresentation.selectedSubFolder;
                }
            }
        }

        if (playerInput.IsKeyJustPressed(SDLK_DOWN))
        {
            if (gameSelectionPresentation.selectedSubFolder == (uint32_t)gameSelectionPresentation.subFolders.size() - 1)
            {
                gameSelectionPresentation.selectedSubFolder = 0;
                gameSelectionPresentation.subFolderOffset = 0;
            }
            else
            {
                gameSelectionPresentation.selectedSubFolder++;
                if (gameSelectionPresentation.selectedSubFolder > gameSelectionPresentation.subFolderOffset + 3)
                {
                    gameSelectionPresentation.subFolderOffset = gameSelectionPresentation.selectedSubFolder - 3;
                }
            }
        }

        if (playerInput.IsKeyJustPressed(SDLK_RETURN))
        {
            const std::string subFolder = gameSelectionPresentation.subFolders.at(gameSelectionPresentation.selectedSubFolder);

            if (subFolder != "..")
            {
                gameSelectionPresentation.searchFolder += subFolder;
                gameSelectionPresentation.searchFolder += "\\";
                gameSelectionPresentation.selectedSubFolder = 0;
            }
            else
            {
                size_t backslashPos = gameSelectionPresentation.searchFolder.find_last_of('\\', gameSelectionPresentation.searchFolder.size() - 2);
                if (backslashPos != std::string::npos)
                {
                    gameSelectionPresentation.searchFolder.erase(backslashPos + 1);
                }
                else
                {
                    // We were already in the root of a logical drive.
                    // Empty the search folder. All the logical drives will be listed.
                    gameSelectionPresentation.searchFolder.clear();
                }
            }
            gameSelectionPresentation.subFolderOffset = 0;

            GetSubFolders(gameSelectionPresentation.searchFolder, gameSelectionPresentation.subFolders);

            // Check if games can be detected in the new folder
            if (!gameSelectionPresentation.searchFolder.empty())
            {
                if (gameDetectionAbyssV113.GetBestMatch().score != 0)
                {
                    gameDetectionAbyssV113.GetDetectionReport(GameIdCatacombAbyssv113, gameSelectionPresentation.searchFolder, abyssFilesv113);
                    gameSelectionPresentation.gameListShareware.clear();
                }

                if (gameDetectionAbyssV124.GetBestMatch().score != 0)
                {
                    gameDetectionAbyssV124.GetDetectionReport(GameIdCatacombAbyssv124, gameSelectionPresentation.searchFolder + "Abyss\\", abyssFilesv124);
                    gameSelectionPresentation.gameListCatacombsPack.clear();
                }

                if (gameDetectionArmageddonv102.GetBestMatch().score != 0)
                {
                    gameDetectionArmageddonv102.GetDetectionReport(GameIdCatacombArmageddonv102, gameSelectionPresentation.searchFolder + "Armageddon\\", armageddonFiles);
                    gameSelectionPresentation.gameListCatacombsPack.clear();
                }

                if (gameDetectionApocalypsev101.GetBestMatch().score != 0)
                {
                    gameDetectionApocalypsev101.GetDetectionReport(GameIdCatacombApocalypsev101, gameSelectionPresentation.searchFolder + "Apocalypse\\", apocalypseFiles);
                    gameSelectionPresentation.gameListCatacombsPack.clear();
                }

                if (gameDetectionCatacomb3Dv122.GetBestMatch().score != 0)
                {
                    gameDetectionCatacomb3Dv122.GetDetectionReport(GameIdCatacomb3Dv122, gameSelectionPresentation.searchFolder + "Cat3D\\", catacomb3DFiles);
                    gameSelectionPresentation.gameListCatacombsPack.clear();
                }
            }
        }

        if (playerInput.IsKeyJustPressed(SDLK_ESCAPE))
        {
            // Exit
            active = false;
        }

        m_console->ProcessInput(playerInput);

        playerInput.ClearJustPressed();
    }

    if (active)
    {
        const DetectionReport& report =
            (selectedGame == GameIdCatacombAbyssv113) ? gameDetectionAbyssV113.GetBestMatch() :
            (selectedGame == GameIdCatacombAbyssv124) ? gameDetectionAbyssV124.GetBestMatch() :
            (selectedGame == GameIdCatacombArmageddonv102) ? gameDetectionArmageddonv102.GetBestMatch() :
            (selectedGame == GameIdCatacombApocalypsev101) ? gameDetectionApocalypsev101.GetBestMatch() :
            gameDetectionCatacomb3Dv122.GetBestMatch();

        if (report.score == 0)
        {
            if (report.gameId == GameIdCatacomb3Dv122)
            {
                game = new GameCatacomb3D(report.folder, systemWin32.GetConfigurationFilePath(), *m_renderer);
            }
            else if (report.gameId == GameIdCatacombArmageddonv102)
            {
                game = new GameArmageddon(report.folder, *m_renderer);
            }
            else if(report.gameId == GameIdCatacombApocalypsev101)
            {
                game = new GameApocalypse(report.folder, *m_renderer);
            }
            else
            {
                game = new GameAbyss(report.gameId, report.folder, *m_renderer);
            }
            Logging::Instance().AddLogMessage("Initializing game " + game->GetName());
        }
        else
        {
            const std::string errorMessage = "Failed to detect game files! Please make sure the Catacombs Pack [GOG.com] is correctly installed. Alternatively, the (shareware) game files can be placed in the same folder as CatacombGL.exe. Detailed info: " + report.infoString;
            Logging::Instance().FatalError(errorMessage);
        }

        if (active)
        {
            engineCore = new EngineCore(*game, systemWin32, playerInput, m_configurationSettings);

            // Update the window title with the selected game info.
            const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo() + " [" + game->GetName() + "]";
            SDL_SetWindowTitle(SDLwindow, windowTitle.c_str());
        }
    }

	while(active)									// Loop That Runs While done=FALSE
	{
        SDL_Event event;
        memset(&event, 0, sizeof(event));
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
            SDL_SetRelativeMouseMode(engineCore->RequiresMouseCapture() ? SDL_TRUE : SDL_FALSE);
            UpdatePlayerInput();
            m_console->ProcessInput(playerInput);
            if (m_screenMode != engineCore->GetScreenMode())
            {
                m_screenMode = engineCore->GetScreenMode();
                SetScreenMode(m_screenMode);
            }
            
            engineCore->DrawScene(*m_renderer);
            m_console->Draw(*m_renderer);
            SDL_GL_SwapWindow(SDLwindow);
		}
	}

    // If game data was found, remember the path!
    if (gameDetectionAbyssV113.GetBestMatch().score == 0)
    {
        m_configurationSettings.SetPathAbyssv113(gameDetectionAbyssV113.GetBestMatch().folder);
    }

    if (gameDetectionAbyssV124.GetBestMatch().score == 0)
    {
        m_configurationSettings.SetPathAbyssv124(gameDetectionAbyssV124.GetBestMatch().folder);
    }

    if (gameDetectionArmageddonv102.GetBestMatch().score == 0)
    {
        m_configurationSettings.SetPathArmageddonv102(gameDetectionArmageddonv102.GetBestMatch().folder);
    }

    if (gameDetectionApocalypsev101.GetBestMatch().score == 0)
    {
        m_configurationSettings.SetPathApocalypsev101(gameDetectionApocalypsev101.GetBestMatch().folder);
    }

    if (gameDetectionCatacomb3Dv122.GetBestMatch().score == 0)
    {
        m_configurationSettings.SetPathCatacomb3Dv122(gameDetectionCatacomb3Dv122.GetBestMatch().folder);
    }

    // Kill The Window
    KillGLWindow();

	// Shutdown
    SD_Shutdown();
    BE_ST_ShutdownAudio();

    // Store configuration
    m_configurationSettings.StoreToFile(configFilename);
	
    delete engineCore;
    delete game;

    SDL_Quit();

    delete m_renderer;

    return TRUE;
}
