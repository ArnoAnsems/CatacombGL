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
#include "../../ThirdParty//SDL/include/SDL_keycode.h"

enum DifficultyLevel
{
    Easy,
    Normal,
    Hard
};

class IGame
{
public:
    virtual ~IGame() {};
    virtual void SpawnActors(Level* level, const DifficultyLevel difficultyLevel) = NULL;
    virtual const DecorateActor& GetExplosionActor() = NULL;
    virtual const DecorateActor& GetExplodingWallActor() = NULL;
    virtual const DecorateActor& GetPlayerActor() = NULL;
    virtual void DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory, const uint16_t wideScreenMargin, const float playerAngle, const uint8_t levelIndex, const uint16_t shotPower) = NULL;
    virtual void DrawScroll(const uint8_t scrollIndex) = NULL;
    virtual void DrawFinal() = NULL;
    virtual GameMaps* GetGameMaps() = NULL;
    virtual EgaGraph* GetEgaGraph() = NULL;
    virtual AudioRepository* GetAudioRepository() = NULL;
    virtual AudioPlayer* GetAudioPlayer() = NULL;
    virtual IIntroView* GetIntroView() = NULL;
    virtual IMenu* CreateMenu(ConfigurationSettings& configurationSettings, std::vector<std::string>& savedGames) = NULL;
    virtual void DrawHelpPage() = NULL;
    virtual bool ProcessInputOnHelpPage(PlayerInput& playerInput) = NULL;
    virtual const std::map<uint16_t, const DecorateActor>& GetDecorateActors() const = NULL;
    virtual const std::string& GetName() const = NULL;
    virtual const uint8_t GetId() const = NULL;
    virtual const uint16_t GetMenuCursorPic() const = NULL;
    virtual const uint16_t GetNorthIconSprite() const = NULL;
    virtual const std::string GetSavedGamesPath() const = NULL;
    virtual const std::vector<std::vector<uint16_t>>& GetWallSkeletonAnimations() const = NULL;
    virtual const SDL_Keycode GetCheatsKeyCode() const = NULL;
    virtual void PlaySoundBodyExplode() = NULL;
    virtual void PlaySoundHitGate() = NULL;
    virtual void PlaySoundPlayerHurt(const int16_t playerHealth) = NULL;
    virtual void PlaySoundGetPotion() = NULL;
    virtual void PlaySoundUsePotion() = NULL;
    virtual void PlaySoundNoItem() = NULL;
    virtual void PlaySoundGetBolt() = NULL;
    virtual void PlaySoundGetNuke() = NULL;
    virtual void PlaySoundGetScroll() = NULL;
    virtual void PlaySoundGetGem() = NULL;
    virtual void PlaySoundGetKey() = NULL;
    virtual void PlaySoundGetPoints() = NULL;
    virtual void PlaySoundUseKey() = NULL;
    virtual void PlaySoundUseBolt() = NULL;
    virtual void PlaySoundUseNuke() = NULL;
    virtual void PlaySoundFreezeTime() = NULL;
    virtual void PlaySoundFreezeTimeTick(const int32_t seconds) = NULL;
    virtual void PlaySoundWalk(const bool walk1) = NULL;
    virtual void PlaySoundBossDeath() = NULL;
    virtual void PlaySoundShoot() = NULL;
    virtual void PlaySoundShootWall() = NULL;
    virtual void PlaySoundBoom() = NULL;
    virtual void PlaySoundSmallMonster() = NULL;
    virtual void PlaySoundLargeMonster() = NULL;
    virtual void PlaySoundWarpUpOrDown(const bool up) = NULL;
    virtual void PlaySoundWarp() = NULL;
    virtual void PlaySoundPortal() = NULL;
    virtual const ViewPorts::ViewPortRect3D& GetOriginal3DViewArea() = NULL;


protected:
    GameMaps* m_gameMaps;
    EgaGraph* m_egaGraph;
    AudioRepository* m_audioRepository;
    AudioPlayer* m_audioPlayer;
};
