// Copyright (C) 2019 Arno Ansems
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
// GameAbyss
//
// IGame implementation for the Catacomb 3-D
//
#pragma once

#include "..\Engine\IGame.h"
#include "..\Engine\IRenderer.h"
#include "..\Engine\Logging.h"
#include <map>

class GameCatacomb3D : public IGame
{
public:
    GameCatacomb3D(const std::string gamePath, IRenderer& renderer);
    ~GameCatacomb3D();

    void SpawnActors(Level* level, const DifficultyLevel difficultyLevel) override;
    const DecorateActor& GetExplosionActor() override;
    const DecorateActor& GetExplodingWallActor() override;
    const DecorateActor& GetPlayerActor() override;
    void DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory, const uint16_t wideScreenMargin) override;
    void DrawScroll(const uint8_t scrollIndex) override;
    void DrawFinal() override;
    GameMaps* GetGameMaps() override;
    EgaGraph* GetEgaGraph() override;
    AudioRepository* GetAudioRepository() override;
    AudioPlayer* GetAudioPlayer() override;

    IIntroView* GetIntroView() override;
    void DrawHelpPage() override;
    bool ProcessInputOnHelpPage(PlayerInput& playerInput) override;
    const std::map<uint16_t, const DecorateActor>& GetDecorateActors() const override;
    const std::string& GetName() const override;
    const uint8_t GetId() const override;
    const uint16_t GetMenuCursorPic() const override;
    const uint16_t GetNorthIconSprite() const override;
    const std::string GetSavedGamesPath() const override;
    const std::vector<std::vector<uint16_t>>& GetWallSkeletonAnimations() const override;
    const SDL_Keycode GetCheatsKeyCode() const override;
    void PlaySoundBodyExplode() override;
    void PlaySoundHitGate() override;
    void PlaySoundPlayerHurt(const int16_t playerHealth) override;
    void PlaySoundGetPotion() override;
    void PlaySoundUsePotion() override;
    void PlaySoundNoItem() override;
    void PlaySoundGetBolt() override;
    void PlaySoundGetNuke() override;
    void PlaySoundGetScroll() override;
    void PlaySoundGetGem() override;
    void PlaySoundGetKey() override;
    void PlaySoundGetPoints() override;
    void PlaySoundUseKey() override;
    void PlaySoundUseBolt() override;
    void PlaySoundUseNuke() override;
    void PlaySoundFreezeTime() override;
    void PlaySoundFreezeTimeTick(const int32_t seconds) override;
    void PlaySoundWalk(const bool walk1) override;
    void PlaySoundBossDeath() override;
    void PlaySoundShoot() override;
    void PlaySoundShootWall() override;
    void PlaySoundBoom() override;
    void PlaySoundSmallMonster() override;
    void PlaySoundLargeMonster() override;
    void PlaySoundWarpUpOrDown(const bool up) override;
    void PlaySoundWarp() override;
    void PlaySoundPortal() override;
    const ViewPorts::ViewPortRect3D& GetOriginal3DViewArea() override;

private:
    IIntroView* m_introView;
    const uint8_t m_gameId;
    const std::string m_gamePath;
    IRenderer& m_renderer;
};

