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
// IGame
//
// Interface for abstracting game specific properties.
//
#pragma once

#include "Level.h"
#include "PlayerInventory.h"
#include "GameMaps.h"
#include "EgaGraph.h"
#include "AudioRepository.h"
#include "AudioPlayer.h"
#include "IIntroView.h"
#include "IMenu.h"
#include <map>
#include "Actor.h"
#include "PlayerInput.h"
#include "ViewPorts.h"
#include "ConfigurationSettings.h"
#include "ManaBar.h"
#include "../../ThirdParty//SDL/include/SDL_keycode.h"

enum DifficultyLevel
{
    Easy,
    Normal,
    Hard
};

struct CatalogInfo
{
    const std::string label;
    const std::vector<std::string> filenames;
};

class IGame
{
public:
    virtual ~IGame() {};
    virtual void SpawnActors(Level* level, const DifficultyLevel difficultyLevel) = 0;
    virtual const DecorateActor& GetExplosionActor() = 0;
    virtual const DecorateActor& GetExplodingWallActor() = 0;
    virtual const DecorateActor& GetPlayerActor() = 0;
    virtual void DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory, const uint16_t wideScreenMargin, const float playerAngle, const uint8_t levelIndex, const uint16_t shotPower, const uint32_t points) = 0;
    virtual void DrawScroll(const uint8_t scrollIndex) = 0;
    virtual void DrawFinal() = 0;
    virtual GameMaps* GetGameMaps() = 0;
    virtual EgaGraph* GetEgaGraph() = 0;
    virtual AudioRepository* GetAudioRepository() = 0;
    virtual AudioPlayer* GetAudioPlayer() = 0;
    virtual IIntroView* GetIntroView() = 0;
    virtual IMenu* CreateMenu(ConfigurationSettings& configurationSettings, PlayerInput& playerInput, std::vector<std::string>& savedGames) = 0;
    virtual void DrawHelpPage() = 0;
    virtual bool ProcessInputOnHelpPage(PlayerInput& playerInput) = 0;
    virtual const std::map<uint16_t, const DecorateActor>& GetDecorateActors() const = 0;
    virtual const std::string& GetName() const = 0;
    virtual const uint8_t GetId() const = 0;
    virtual const uint16_t GetMenuCursorPic() const = 0;
    virtual const uint16_t GetNorthIconSprite() const = 0;
    virtual const std::string GetSavedGamesPath() const = 0;
    virtual const std::vector<std::vector<uint16_t>>& GetWallSkeletonAnimations() const = 0;
    virtual const SDL_Keycode GetCheatsKeyCode() const = 0;
    virtual void PlaySoundBodyExplode() = 0;
    virtual void PlaySoundHitGate() = 0;
    virtual void PlaySoundPlayerHurt(const int16_t playerHealth) = 0;
    virtual void PlaySoundGetPotion() = 0;
    virtual void PlaySoundUsePotion() = 0;
    virtual void PlaySoundNoItem() = 0;
    virtual void PlaySoundGetBolt() = 0;
    virtual void PlaySoundGetNuke() = 0;
    virtual void PlaySoundGetScroll() = 0;
    virtual void PlaySoundGetGem() = 0;
    virtual void PlaySoundGetKey() = 0;
    virtual void PlaySoundGetPoints() = 0;
    virtual void PlaySoundUseKey() = 0;
    virtual void PlaySoundUseBolt() = 0;
    virtual void PlaySoundUseNuke() = 0;
    virtual void PlaySoundFreezeTime() = 0;
    virtual void PlaySoundFreezeTimeTick(const int32_t freezeTicks) = 0;
    virtual void PlaySoundWalk(const bool walk1) = 0;
    virtual void PlaySoundBossDeath() = 0;
    virtual void PlaySoundShoot() = 0;
    virtual void PlaySoundShootWall() = 0;
    virtual void PlaySoundBoom() = 0;
    virtual void PlaySoundSmallMonster() = 0;
    virtual void PlaySoundLargeMonster() = 0;
    virtual void PlaySoundWarpUpOrDown(const bool up) = 0;
    virtual void PlaySoundWarp() = 0;
    virtual void PlaySoundPortal() = 0;
    virtual const ViewPorts::ViewPortRect3D& GetOriginal3DViewArea() = 0;
    virtual const ManaBar::ManaBarConfig& GetManaBarConfig() = 0;
    virtual const CatalogInfo& GetCatalogInfo() const = 0;


protected:
    GameMaps* m_gameMaps = nullptr;
    EgaGraph* m_egaGraph = nullptr;
    AudioRepository* m_audioRepository = nullptr;
    AudioPlayer* m_audioPlayer = nullptr;
};
