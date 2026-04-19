// Copyright (C) 2026 Arno Ansems
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

#include "GameStub.h"
#include "../Engine/Actor.h"

void GameStub::SpawnActors(Level* /*level*/, const DifficultyLevel /*difficultyLevel*/)
{
};

const DecorateActor& GameStub::GetExplosionActor()
{
    static DecorateActor decorateActor;
    return decorateActor;
}

const DecorateActor& GameStub::GetExplodingWallActor()
{
    static DecorateActor decorateActor;
    return decorateActor;
}

const DecorateActor& GameStub::GetPlayerActor()
{
    static DecorateActor decorateActor;
    return decorateActor;
}

void GameStub::DrawStatusBar(
    const int16_t /*health*/,
    const std::string& /*locationMessage*/,
    const PlayerInventory& /*playerInventory*/,
    const uint16_t /*wideScreenMargin*/,
    const float /*playerAngle*/,
    const uint8_t /*levelIndex*/,
    const uint16_t /*shotPower*/, 
    const uint32_t /*points*/)
{
}

void GameStub::DrawScroll(const uint8_t /*scrollIndex*/)
{
}

void GameStub::DrawFinal()
{
}

GameMaps* GameStub::GetGameMaps()
{
    return nullptr;
}

EgaGraph* GameStub::GetEgaGraph()
{
    return nullptr;
}

AudioRepository* GameStub::GetAudioRepository()
{
    return nullptr;
}

AudioPlayer* GameStub::GetAudioPlayer()
{
    return nullptr;
}

IIntroView* GameStub::GetIntroView()
{
    return nullptr;
}

IMenu* GameStub::CreateMenu(
    ConfigurationSettings& /*configurationSettings*/,
    PlayerInput& /*playerInput*/,
    std::vector<std::string>& /*savedGames*/,
    SavedGamesInDosFormat& /*savedGamesInDosFormat*/)
{
    return nullptr;
}

void GameStub::DrawHelpPage()
{
}

bool GameStub::ProcessInputOnHelpPage(PlayerInput& /*playerInput*/)
{
    return false;
}

const std::map<uint16_t, const DecorateActor>&GameStub::GetDecorateActors() const
{
    static const std::map<uint16_t, const DecorateActor> decorateActors = { {} };
    return decorateActors;
}

const std::string& GameStub::GetName() const
{
    static const std::string gameName = "StubGame";
    return gameName;
}

const GameId GameStub::GetId() const
{
    return GameId::NotDetected;
}

const uint16_t GameStub::GetMenuCursorPic() const
{
    return 0;
}

const uint16_t GameStub::GetNorthIconSprite() const
{
    return 0;
}

const std::string GameStub::GetSavedGamesPath() const
{
    return "";
}

const std::vector<std::vector<uint16_t>>& GameStub::GetWallSkeletonAnimations() const
{
    static const std::vector<std::vector<uint16_t>> animations = {};
    return animations;
}

const SDL_Keycode GameStub::GetCheatsKeyCode() const
{
    return SDLK_UNKNOWN;
}

void GameStub::PlaySoundBodyExplode()
{
}

void GameStub::PlaySoundHitGate()
{
}

void GameStub::PlaySoundPlayerHurt(const int16_t /*playerHealth*/)
{
}

void GameStub::PlaySoundGetPotion()
{
}

void GameStub::PlaySoundUsePotion()
{
}

void GameStub::PlaySoundNoItem()
{
}

void GameStub::PlaySoundGetBolt()
{
}

void GameStub::PlaySoundGetNuke()
{
}

void GameStub::PlaySoundGetScroll()
{
}

void GameStub::PlaySoundGetGem()
{
}

void GameStub::PlaySoundGetKey()
{
}

void GameStub::PlaySoundGetPoints()
{
}

void GameStub::PlaySoundUseKey()
{
}

void GameStub::PlaySoundUseBolt()
{
}

void GameStub::PlaySoundUseNuke()
{
}

void GameStub::PlaySoundFreezeTime()
{
}

void GameStub::PlaySoundFreezeTimeTick(const int32_t /*freezeTicks*/)
{
}

void GameStub::PlaySoundWalk(const bool /*walk1*/)
{
}

void GameStub::PlaySoundBossDeath()
{
}

void GameStub::PlaySoundShoot()
{
}

void GameStub::PlaySoundShootWall()
{
}

void GameStub::PlaySoundBoom()
{
}

void GameStub::PlaySoundSmallMonster()
{
}
void GameStub::PlaySoundLargeMonster()
{
}

void GameStub::PlaySoundWarpUpOrDown(const bool /*up*/)
{
}

void GameStub::PlaySoundWarp()
{
}

void GameStub::PlaySoundPortal()
{
}

const ViewPorts::ViewPortRect3D& GameStub::GetOriginal3DViewArea()
{
    static const ViewPorts::ViewPortRect3D rect3D = { 0, 0, 0, 0 };
    return rect3D;
}

const ManaBar::ManaBarConfig& GameStub::GetManaBarConfig()
{
    static const ManaBar::ManaBarConfig config = { 0, 0, EgaBlack, EgaBlack };
    return config;
}

const CatalogInfo& GameStub::GetCatalogInfo() const
{
    static const CatalogInfo catalogInfo = { "Catalog", {} };
    return catalogInfo;
}

const SavedGameInDosFormatConfig& GameStub::GetSavedGameInDosFormatConfig() const
{
    static const SavedGameInDosFormatConfig config = { "StubGame", "0.0", {}, {} };
    return config;
}

ISavedGameConverter& GameStub::GetSavedGameConverter()
{
    static ISavedGameConverter* savedGameConverter = nullptr;
    return *savedGameConverter;
}

bool GameStub::IsCatacomb3D() const
{
    return false;
}

bool GameStub::IsCatacombAdventureSeries() const
{
    return false;
}
