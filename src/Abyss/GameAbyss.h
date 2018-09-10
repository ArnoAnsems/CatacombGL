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
// GameAbyss
//
// IGame implementation for the Catacomb Abyss
//
#pragma once

#include "..\Engine\IGame.h"
#include "..\Engine\IRenderer.h"
#include <map>

class GameAbyss: public IGame
{
public:
    GameAbyss(const uint8_t gameId, const std::string gamePath, IRenderer& renderer);
    ~GameAbyss();

    void SpawnActors(Level* level, const DifficultyLevel difficultyLevel);
    void DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory);
    GameMaps* GetGameMaps();
    EgaGraph* GetEgaGraph();
    AudioRepository* GetAudioRepository();
    AudioPlayer* GetAudioPlayer();

    IIntroView* GetIntroView();
    const std::map<uint16_t, const DecorateActor>& GetDecorateActors() const;
    const std::string& GetName() const;
    const uint8_t GetId() const;

private:
    void DrawHealth(const int16_t health);
    void DrawScrolls(const PlayerInventory& playerInventory);
    void DrawKeys(const PlayerInventory& playerInventory);
    void DrawBonus(const PlayerInventory& playerInventory);
    void DrawGems(const PlayerInventory& playerInventory);

    short m_zombie_base_delay;
    IIntroView* m_introView;
    const uint8_t m_gameId;
    const std::string m_gamePath;
    IRenderer& m_renderer;
};
