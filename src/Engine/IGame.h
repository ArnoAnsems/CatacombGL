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
#include <map>
#include "Actor.h"

enum DifficultyLevel
{
    Easy,
    Normal,
    Hard
};

class IGame
{
public:
    virtual void SpawnActors(Level* level, const DifficultyLevel difficultyLevel) = NULL;
    virtual const DecorateActor& GetExplosionActor() = NULL;
    virtual const DecorateActor& GetExplodingWallActor() = NULL;
    virtual const DecorateActor& GetPlayerActor() = NULL;
    virtual void DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory) = NULL;
    virtual GameMaps* GetGameMaps() = NULL;
    virtual EgaGraph* GetEgaGraph() = NULL;
    virtual AudioRepository* GetAudioRepository() = NULL;
    virtual AudioPlayer* GetAudioPlayer() = NULL;
    virtual IIntroView* GetIntroView() = NULL;
    virtual const std::map<uint16_t, const DecorateActor>& GetDecorateActors() const = NULL;
    virtual const std::string& GetName() const = NULL;
    virtual const uint8_t GetId() const = NULL;
    virtual const uint16_t GetMenuCursorPic() const = NULL;

protected:
    GameMaps* m_gameMaps;
    EgaGraph* m_egaGraph;
    AudioRepository* m_audioRepository;
    AudioPlayer* m_audioPlayer;
};
