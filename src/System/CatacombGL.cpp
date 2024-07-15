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

#include "Finder.h"
#include "RendererOpenGL.h"
#include "SystemSDL.h"
#include "WindowSDL.h"

#include "../Engine/ConfigurationSettings.h"
#include "../Engine/Console.h"
#include "../Engine/DefaultFont.h"
#include "../Engine/EngineCore.h"
#include "../Engine/GameDetection.h"
#include "../Engine/GameSelection.h"
#include "../Engine/Logging.h"
#include "../Engine/PlayerInput.h"

#include "../Abyss/GameAbyss.h"
#include "../Abyss/GameDetectionAbyss.h"
#include "../Apocalypse/GameApocalypse.h"
#include "../Apocalypse/GameDetectionApocalypse.h"
#include "../Armageddon/GameArmageddon.h"
#include "../Armageddon/GameDetectionArmageddon.h"
#include "../Catacomb3D/GameCatacomb3D.h"
#include "../Catacomb3D/GameDetectionCatacomb3D.h"

#include "../../ThirdParty/RefKeen/be_st.h"
#include "../../ThirdParty/RefKeen/id_sd.h"

#include <SDL.h>
#include <SDL_mouse.h>
#include <SDL_video.h>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

namespace fs = std::filesystem;

uint8_t selectedGame = GameID::NotDetected;

bool compareStrings(const string& str1, const string& str2)
{
    if (str1.length() != str2.length())
    {
        return false;
    }

	for (int i = 0; i < str1.length(); ++i)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
        {
            return false;
        }
	}

	return true;
}

void UpdatePlayerInput(const SDL_Window* const window, PlayerInput& input)
{
	SDL_PumpEvents();
	int numKeys = 0;
	const Uint8* state = SDL_GetKeyboardState(&numKeys);

	for (int i = 0; i < numKeys; i++)
	{
		input.SetKeyPressed(SDL_GetKeyFromScancode((SDL_Scancode)(i)), state[i]);
	}

	const uint32_t timestamp = SDL_GetTicks();
	const uint32_t minimumTimeBetweenMouseUpdates = 10u; // in milliseconds
	if (timestamp >= input.GetMouseUpdateTick() + minimumTimeBetweenMouseUpdates)
	{
		int x = 0;
		int y = 0;
		const uint32_t mouseState = SDL_GetRelativeMouseState(&x, &y);
		input.SetMouseXPos(x);
		input.SetMouseYPos(y);
		input.SetMouseButtonPressed(SDL_BUTTON_LEFT, mouseState & SDL_BUTTON_LMASK);
		input.SetMouseButtonPressed(SDL_BUTTON_MIDDLE, mouseState & SDL_BUTTON_MMASK);
		input.SetMouseButtonPressed(SDL_BUTTON_RIGHT, mouseState & SDL_BUTTON_RMASK);
		input.SetMouseButtonPressed(SDL_BUTTON_X1, mouseState & SDL_BUTTON_X1MASK);
		input.SetMouseButtonPressed(SDL_BUTTON_X2, mouseState & SDL_BUTTON_X2MASK);
		input.SetHasFocus(SDL_GetMouseFocus() == window);
		input.SetMouseUpdateTick(timestamp);
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

const std::string paramDescent = "--descent";
const std::string paramAbyss = "--abyss";
const std::string paramArmageddon = "--armageddon";
const std::string paramApocalypse = "--apocalypse";
const std::string paramAbyssSW13 = "--abyss_sw13";

const std::string paramIniFile = "--ini";
std::string FilenameIni = "";
const std::string paramLogFile = "--log";
std::string FilenameLog = "";

void parseCommandLine(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		std::string prev = argv[i - 1];
		std::string current = argv[i];

        if (compareStrings(current, paramDescent))
        {
            selectedGame = GameID::Catacomb3Dv122;
        }
        if (compareStrings(current, paramAbyss))
        {
            selectedGame = GameID::CatacombAbyssv124;
        }
        if (compareStrings(current, paramApocalypse))
        {
            selectedGame = GameID::CatacombApocalypsev101;
        }
        if (compareStrings(current, paramArmageddon))
        {
            selectedGame = GameID::CatacombArmageddonv102;
        }
        if (compareStrings(current, paramAbyssSW13))
        {
            selectedGame = GameID::CatacombAbyssv113;
        }

        if (compareStrings(prev, paramIniFile))
        {
            FilenameIni = current;
        }

        if (compareStrings(prev, paramLogFile))
        {
            FilenameLog = current;
        }
	};
}

int main(int argc, char* argv[])
{
	ConfigurationSettings config;
	PlayerInput input;
	SystemSDL system;

	Console* console = new Console("CatacombGL " + EngineCore::GetVersionInfo());
	IRenderer* renderer = new RendererOpenGL();

	EngineCore* engine = nullptr;
	IGame* game = nullptr;
	SDL_Window* window = nullptr;
	SDL_GLContext context = nullptr;

	bool active = true;
	uint8_t screenMode = CVarItemIdScreenModeWindowed;

	/* initialize random seed: */
	srand((unsigned int)time(nullptr));

	/* Check command line parameters */
    if (argc > 1)
    {
        parseCommandLine(argc, argv);
    }

	const fs::path configPath = system.GetConfigurationFilePath();
	system.CreatePath(configPath);

	const fs::path logFilename = FilenameLog.empty() ? configPath / "CatacombGL_log.txt" : FilenameLog;
	Logging::Instance().SetLogFile(logFilename);
	Logging::Instance().AddLogMessage("Configuration file used .... " + logFilename.string());

	const std::string buildBitInfo(system.isBuiltIn64Bit() ? " (64 bit)" : " (32 bit)");
	Logging::Instance().AddLogMessage("Initializing CatacombGL " + EngineCore::GetVersionInfo() + buildBitInfo);

	Logging::Instance().AddLogMessage("Running on " + system.GetOSVersion());

	const fs::path configFilename = FilenameIni.empty() ? configPath / "CatacombGL.ini" : FilenameIni;
	Logging::Instance().AddLogMessage("Loading CatacombGL.ini");
	config.LoadFromFile(configFilename);

	InitializeSDL();

	Finder finder = Finder(config);

	for (uint8_t i = 0; i < GameID::Count; ++i)
	{
		if (finder.GetGameScore(i) == 0)
		{
			Logging::Instance().AddLogMessage(finder.GetGameName(i) + " detected at " + finder.GetGameFolder(i).string());
		}
		else
		{
			Logging::Instance().AddLogMessage(finder.GetGameName(i) + " not detected");
		}
	}

	// Create Our OpenGL Window
	CreateGLWindow(800, 600, 16, window, context);

	renderer = new RendererOpenGL();
	renderer->Setup();
	SetScreenMode(config.GetCVarEnum(CVarIdScreenMode).GetItemIndex(), window);

	// Set Up Our Perspective GL Screen
	ReSizeGLScene(renderer, 800, 600);

	Logging::Instance().AddLogMessage("Running on graphics adapter model " + renderer->GetGraphicsAdapterModel());
	Logging::Instance().AddLogMessage("Running on " + renderer->GetGraphicsApiVersion());

	BE_ST_InitAudio();
	SD_Startup();

	fs::path initialSearchFolder = fs::current_path();
	GameSelection gameSelection(*renderer);

	GameSelectionPresentation gameSelectionPresentation;
	gameSelectionPresentation.searchFolder = initialSearchFolder;
	gameSelectionPresentation.selectedSubFolder = 0;
	gameSelectionPresentation.subFolderOffset = 0;

	system.GetSubFolders(initialSearchFolder, gameSelectionPresentation.subFolders);

	if (selectedGame == GameID::NotDetected)
	{
		while (selectedGame == GameID::NotDetected && active)
		{
			SDL_Event event;
			memset(&event, 0, sizeof(event));
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_WINDOWEVENT)
				{
					active = HandleWindowEvent(&event.window, window, renderer);
				}
			}

			if (gameSelectionPresentation.gameListCatacombsPack.empty())
			{
				const GameDetectionState catacomb3Dv122DetectionState = (finder.GetGameScore(GameID::Catacomb3Dv122) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("1. Catacomb 3-D: The Descent v1.22", catacomb3Dv122DetectionState));
				const GameDetectionState abyssv124DetectionState = (finder.GetGameScore(GameID::CatacombAbyssv124) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("2. Catacomb Abyss v1.24", abyssv124DetectionState));
				const GameDetectionState armageddonDetectionState = (finder.GetGameScore(GameID::CatacombArmageddonv102) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("3. Catacomb Armageddon v1.02", armageddonDetectionState));
				const GameDetectionState apocalypseDetectionState = (finder.GetGameScore(GameID::CatacombApocalypsev101) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("4. Catacomb Apocalypse v1.01", apocalypseDetectionState));
			}

			if (gameSelectionPresentation.gameListShareware.empty())
			{
				gameSelectionPresentation.gameListShareware.push_back(std::make_pair("5. Catacomb Abyss v1.12", NotSupported));
				const GameDetectionState abyssv133DetectionState = (finder.GetGameScore(GameID::CatacombAbyssv113) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListShareware.push_back(std::make_pair("6. Catacomb Abyss v1.13", abyssv133DetectionState));
			}

			IRenderer::FrameSettings frameSettings;
			frameSettings.textureFilter = (config.GetCVarEnum(CVarIdTextureFilter).GetItemIndex() == CVarItemIdTextureFilterNearest) ? IRenderer::Nearest : IRenderer::Linear;
			frameSettings.vSyncEnabled = config.GetCVarBool(CVarIdVSync).IsEnabled();
			renderer->SetFrameSettings(frameSettings);

			gameSelection.Draw(gameSelectionPresentation);
			console->Draw(*renderer);
			SDL_GL_SwapWindow(window);

			UpdatePlayerInput(window, input);

			if (input.IsKeyPressed(SDLK_1))
			{
				selectedGame = GameID::Catacomb3Dv122;
			}

			if (input.IsKeyPressed(SDLK_2))
			{
				selectedGame = GameID::CatacombAbyssv124;
			}

			if (input.IsKeyPressed(SDLK_3))
			{
				selectedGame = GameID::CatacombArmageddonv102;
			}

			if (input.IsKeyPressed(SDLK_4))
			{
				selectedGame = GameID::CatacombApocalypsev101;
			}

			if (input.IsKeyPressed(SDLK_6))
			{
				selectedGame = GameID::CatacombAbyssv113;
			}

			if (input.IsKeyJustPressed(SDLK_UP))
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

			if (input.IsKeyJustPressed(SDLK_DOWN))
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

			if (input.IsKeyJustPressed(SDLK_RETURN))
			{
				const fs::path subFolder = gameSelectionPresentation.subFolders.at(gameSelectionPresentation.selectedSubFolder);

				if (subFolder != "..")
				{
					gameSelectionPresentation.searchFolder = subFolder;
					gameSelectionPresentation.selectedSubFolder = 0;
				}
				else
				{
					auto searchFolder = gameSelectionPresentation.searchFolder;
					if (searchFolder.has_parent_path() && searchFolder != searchFolder.root_path())
					{
						gameSelectionPresentation.searchFolder = searchFolder.parent_path();
					}
#ifdef _WIN32
					else
					{
						// We were already in the root of a logical drive.
						// Empty the search folder. All the logical drives will be listed.
						gameSelectionPresentation.searchFolder.clear();
					}
#endif
				}
				gameSelectionPresentation.subFolderOffset = 0;

				system.GetSubFolders(gameSelectionPresentation.searchFolder, gameSelectionPresentation.subFolders);

				// Check if games can be detected in the new folder
				if (!gameSelectionPresentation.searchFolder.empty())
				{
					finder.FindInPath(gameSelectionPresentation.searchFolder);
					gameSelectionPresentation.gameListShareware.clear();
					gameSelectionPresentation.gameListCatacombsPack.clear();
				}
			}

			if (input.IsKeyJustPressed(SDLK_ESCAPE))
			{
				// Exit
				active = false;
			}

			console->ProcessInput(input);
			input.ClearJustPressed();
		}
	}

	if (active)
	{
		const DetectionReport report = finder.GetGameReport(selectedGame);

		if (report.score == 0)
		{
			switch (report.gameId)
			{
			case GameID::Catacomb3Dv122:
				game = new GameCatacomb3D(report.folder, system.GetConfigurationFilePath(), *renderer);
				break;
			case GameID::CatacombAbyssv124:
				game = new GameAbyss(2, report.folder, *renderer);
				break;
			case GameID::CatacombArmageddonv102:
				game = new GameArmageddon(report.folder, *renderer);
				break;
			case GameID::CatacombApocalypsev101:
				game = new GameApocalypse(report.folder, *renderer);
				break;
			default:
				// DEMO
				game = new GameAbyss(1, report.folder, *renderer);

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
			engine = new EngineCore(*game, system, input, config);

			// Update the window title with the selected game info.
			const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo() + " [" + game->GetName() + "]";
			SDL_SetWindowTitle(window, windowTitle.c_str());
		}
	}

	// Loop That Runs While done=FALSE
	while (active)
	{
		SDL_Event event;
		memset(&event, 0, sizeof(event));
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_WINDOWEVENT)
			{
				active = HandleWindowEvent(&event.window, window, renderer);
			}
		}

		if (engine->Think())
		{
			active = false;
		}
		else                                // Not Time To Quit, Update Screen
		{
			SDL_SetRelativeMouseMode(engine->RequiresMouseCapture() ? SDL_TRUE : SDL_FALSE);
			UpdatePlayerInput(window, input);
			console->ProcessInput(input);
			if (screenMode != engine->GetScreenMode())
			{
				screenMode = engine->GetScreenMode();
				SetScreenMode(screenMode, window);
			}

			engine->DrawScene(*renderer);
			console->Draw(*renderer);
			SDL_GL_SwapWindow(window);
		}
	}

	finder.SafePaths(config);

	// Kill The Window
	KillGLWindow(window, context);

	// Shutdown
	SD_Shutdown();
	BE_ST_ShutdownAudio();

	// Store configuration
	config.StoreToFile(configFilename);

	delete engine;
	delete game;

	SDL_Quit();

	delete renderer;
	delete console;

	return 0;
}