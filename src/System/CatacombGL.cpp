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

#include "../Engine/CommandLineParser.h"
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

extern "C" {
#include "../../ThirdParty/ReflectionHLE/be_st.h"
};
#include "../../ThirdParty/ReflectionHLE/id_sd.h"

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

namespace fs = std::filesystem;

GameId selectedGame = GameId::NotDetected;

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
    CommandLineParser commandLineParser;
    commandLineParser.parse(argc, argv);
    selectedGame = commandLineParser.gameIdToStart();

	const fs::path configPath = system.GetConfigurationFilePath();
	system.CreatePath(configPath);

	const fs::path logFilename = commandLineParser.getFilenameLog().empty() ? (fs::path) configPath / "CatacombGL_log.txt" : (fs::path) commandLineParser.getFilenameLog();
	Logging::Instance().SetLogFile(logFilename);
	Logging::Instance().AddLogMessage("Configuration file used .... " + logFilename.string());

	const std::string buildBitInfo(system.isBuiltIn64Bit() ? " (64 bit)" : " (32 bit)");
	Logging::Instance().AddLogMessage("Initializing CatacombGL " + EngineCore::GetVersionInfo() + buildBitInfo);

	Logging::Instance().AddLogMessage("Running on " + system.GetOSVersion());

	const fs::path configFilename = commandLineParser.getFilenameIni().empty() ? (fs::path) configPath / "CatacombGL.ini" : (fs::path) commandLineParser.getFilenameIni();
	Logging::Instance().AddLogMessage("Loading CatacombGL.ini");
	config.LoadFromFile(configFilename);

	InitializeSDL();

	Finder finder = Finder(config);

	for (GameId gameId : getAllGameIds())
	{
		if (finder.GetGameScore(gameId) == 0)
		{
			Logging::Instance().AddLogMessage(finder.GetGameName(gameId) + " detected at " + finder.GetGameFolder(gameId).string());
		}
		else
		{
			Logging::Instance().AddLogMessage(finder.GetGameName(gameId) + " not detected");
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

	BEL_ST_SetConfig();
	BE_ST_InitTiming();
	BE_ST_InitAudio();
	SD_Startup();

	fs::path initialSearchFolder = fs::current_path();
	GameSelection gameSelection(*renderer);

	GameSelectionPresentation gameSelectionPresentation;
	gameSelectionPresentation.searchFolder = initialSearchFolder;
	gameSelectionPresentation.selectedSubFolder = 0;
	gameSelectionPresentation.subFolderOffset = 0;

	system.GetSubFolders(initialSearchFolder, gameSelectionPresentation.subFolders);

	if (selectedGame == GameId::NotDetected)
	{
		while (selectedGame == GameId::NotDetected && active)
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
				const GameDetectionState catacomb3Dv122DetectionState = (finder.GetGameScore(GameId::Catacomb3Dv122) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("1. Catacomb 3-D: The Descent v1.22", catacomb3Dv122DetectionState));
				const GameDetectionState abyssv124DetectionState = (finder.GetGameScore(GameId::CatacombAbyssv124) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("2. Catacomb Abyss v1.24", abyssv124DetectionState));
				const GameDetectionState armageddonDetectionState = (finder.GetGameScore(GameId::CatacombArmageddonv102) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("3. Catacomb Armageddon v1.02", armageddonDetectionState));
				const GameDetectionState apocalypseDetectionState = (finder.GetGameScore(GameId::CatacombApocalypsev101) == 0) ? Detected : NotDetected;
				gameSelectionPresentation.gameListCatacombsPack.push_back(std::make_pair("4. Catacomb Apocalypse v1.01", apocalypseDetectionState));
			}

			if (gameSelectionPresentation.gameListShareware.empty())
			{
				gameSelectionPresentation.gameListShareware.push_back(std::make_pair("5. Catacomb Abyss v1.12", NotSupported));
				const GameDetectionState abyssv133DetectionState = (finder.GetGameScore(GameId::CatacombAbyssv113) == 0) ? Detected : NotDetected;
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
				selectedGame = GameId::Catacomb3Dv122;
			}

			if (input.IsKeyPressed(SDLK_2))
			{
				selectedGame = GameId::CatacombAbyssv124;
			}

			if (input.IsKeyPressed(SDLK_3))
			{
				selectedGame = GameId::CatacombArmageddonv102;
			}

			if (input.IsKeyPressed(SDLK_4))
			{
				selectedGame = GameId::CatacombApocalypsev101;
			}

			if (input.IsKeyPressed(SDLK_6))
			{
				selectedGame = GameId::CatacombAbyssv113;
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
			case GameId::Catacomb3Dv122:
				game = new GameCatacomb3D(report.folder, system.GetConfigurationFilePath(), *renderer);
				break;
			case GameId::CatacombAbyssv124:
				game = new GameAbyss(GameId::CatacombAbyssv124, report.folder, *renderer);
				break;
			case GameId::CatacombArmageddonv102:
				game = new GameArmageddon(report.folder, *renderer);
				break;
			case GameId::CatacombApocalypsev101:
				game = new GameApocalypse(report.folder, *renderer);
				break;
			default:
				// Shareware
				game = new GameAbyss(GameId::CatacombAbyssv113, report.folder, *renderer);

			}
			Logging::Instance().AddLogMessage("Initializing game " + game->GetName());
		}
		else
		{
			const std::string errorMessage = "Failed to detect game files! Please make sure the Catacombs Pack [GOG.com] is correctly installed. Alternatively, the (shareware) game files can be placed in the same folder as CatacombGL.exe. Detailed info: " + report.infoString;
			Logging::Instance().FatalError(errorMessage);
		}

		if (active && game != nullptr)
		{
			engine = new EngineCore(*game, system, input, config);

			// Update the window title with the selected game info.
			const std::string windowTitle = "CatacombGL " + EngineCore::GetVersionInfo() + " [" + game->GetName() + "]";
			SDL_SetWindowTitle(window, windowTitle.c_str());
		}
	}

	// Loop That Runs While done=FALSE
	while (active && game != nullptr)
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
