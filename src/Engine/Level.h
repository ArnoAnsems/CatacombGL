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
// Level
//
// Represents the single level that is currently being played, including walls, floors and actors.
// When a level is entered, an instance of this class is created. When the level is exited, this instance is destroyed.
//
#pragma once
#include <stdint.h>
#include "EgaColor.h"
#include <string>
#include <vector>
#include "PlayerInventory.h"
#include "Actor.h"
#include "IRenderer.h"
#include "Logging.h"

class EgaGraph;

struct LevelInfo
{
    std::string name;
    egaColor ceilingColor;
    egaColor floorColor;
    bool showLightning;
    bool showDusk;
};

typedef enum WallType
{
    WTOpen,
    WTSolid,
    WTVictory,
    WTDoor,
    WTDoorRedKeyRequired,
    WTDoorYellowKeyRequired,
    WTDoorGreenKeyRequired,
    WTDoorBlueKeyRequired,
    WTInvisibleWall,
    WTDestructable
} WallType;

typedef struct WallInfo
{
    std::vector<uint16_t> textureLight;
    std::vector<uint16_t> textureDark;
    WallType wallType;
} WallInfo;

enum ChaseTarget
{
    ChasePlayer,
    ChaseTopLeftCorner,
    ChaseTopRightCorner,
    ChaseBottomLeftCorner,
    ChaseBottomRightCorner
};

typedef struct LevelCoordinate
{
    float x;
    float y;
} LevelCoordinate;

typedef struct LevelWall
{
    uint16_t x;
    uint16_t y;
    bool isXWall;
} LevelWall;

class Level
{
public:
    Level(
        const uint8_t levelIndex,
        const uint16_t levelWidth,
        const uint16_t levelHeight,
        const uint16_t* plane0,
        const uint16_t* plane2,
        const LevelInfo& mapInfo,
        const std::vector<WallInfo>& wallsInfo);
    bool LoadActorsFromFile(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors);
    ~Level();

    uint16_t GetLevelWidth() const;
    uint16_t GetLevelHeight() const;
    uint16_t GetWallTile(const uint16_t x, const uint16_t y) const;
    uint16_t GetFloorTile(const uint16_t x, const uint16_t y) const;
    void SetWallTile(const uint16_t x, const uint16_t y, const uint16_t wallTile);
    void SetFloorTile(const uint16_t x, const uint16_t y, const uint16_t floorTile);

    bool IsSolidWall(const uint16_t x, const uint16_t y) const;
    bool IsVisibleTile(const uint16_t x, const uint16_t y) const;
    bool IsExplosiveWall(const uint16_t x, const uint16_t y) const;
    bool IsDoor(const uint16_t x, const uint16_t y) const;
    bool IsRemovableDoor(const uint16_t x, const uint16_t y) const;
    bool IsExitDoor(const uint16_t x, const uint16_t y) const;
    bool IsVictoryDoor(const uint16_t x, const uint16_t y) const;
    bool IsBlockedDoor(const uint16_t x, const uint16_t y) const;
    KeyId GetRequiredKeyForDoor(const uint16_t x, const uint16_t y) const;
    const LevelInfo& GetMapInfo() const;
    const char* GetLevelName() const;
    const egaColor GetSkyColor(const uint32_t timeStamp);
    const egaColor GetGroundColor() const;
    uint8_t GetLevelIndex() const;
    void UpdateVisibilityMap();
    bool IsTileVisibleForPlayer(const uint16_t x, const uint16_t y) const;
    Actor* const GetPlayerActor();
    void SetPlayerActor(Actor* const actor);
    Actor** GetBlockingActors();
    Actor** GetNonBlockingActors();
    void SetBlockingActor(const uint16_t x, const uint16_t y, Actor* actor);
    Actor* GetBlockingActor(const uint16_t x, const uint16_t y) const;
    Actor* GetNonBlockingActor(const uint16_t index) const;
    void AddNonBlockingActor(Actor* projectile);
    void RemoveActor(Actor* actor);

    bool Walk(Actor* const actor);
    void ChaseThink (Actor* const actor, const bool diagonal, const ChaseTarget target);
    void RunAwayThink(Actor* const actor);
    void MoveActor(Actor* const actor, const float distance);
    int16_t AngleNearPlayer(const Actor* const actor) const;
    void ExplodeWall(const uint16_t x, const uint16_t y, const uint32_t timestamp, const DecorateActor& explodingWallActor);
    void SpawnExplosion(const float x, const float y, const int16_t delay, const uint32_t timestamp, const DecorateActor& decorateActor);
    void SpawnBigExplosion(const float x, const float y, const uint16_t delay, const uint32_t range, const uint32_t timestamp, const DecorateActor& decorateActor);

    void StoreToFile(std::ofstream& file) const;
    bool IsWaterLevel() const;

    void DrawVisibilityMap(IRenderer& renderer);
    void DrawFloorAndCeiling(IRenderer& renderer, const uint32_t timeStamp);
    void DrawWalls(IRenderer& renderer, EgaGraph* egaGraph, const uint32_t ticks);
    void DrawActors(IRenderer& renderer, EgaGraph* egaGraph);
    void DrawOverheadMap(IRenderer& renderer, EgaGraph& egaGraph);

private:
    uint16_t GetDarkWallPictureIndex(const uint16_t tileIndex, const uint32_t ticks) const;
    uint16_t GetLightWallPictureIndex(const uint16_t tileIndex, const uint32_t ticks) const;
    void BackTraceWalls(const float distanceOnOuterWall, LevelWall& firstWall);
    bool IsActorVisibleForPlayer(const Actor* actor) const;
    void RayTraceWall(const LevelCoordinate& coordinateInView, LevelWall& wallHit);
    LevelCoordinate GetOuterWallCoordinate(const float distance) const;
    float GetDistanceOnOuterWall(const LevelCoordinate& coordinate) const;
    LevelCoordinate GetRightEdgeOfWall(LevelWall& wall) const;
    LevelCoordinate GetLeftEdgeOfWall(LevelWall& wall) const;
    LevelCoordinate GetIntersectionWithOuterWall(const LevelCoordinate& coordinateInView) const;

    const uint16_t m_levelWidth;
    const uint16_t m_levelHeight;
    uint16_t* m_plane0;
    uint16_t* m_plane2;
    const LevelInfo& m_levelInfo;
    const std::vector<WallInfo>& m_wallsInfo;
    uint32_t m_lightningStartTimestamp;
    const uint8_t m_levelIndex;
    bool* m_visibilityMap;
    Actor* m_playerActor;
    Actor** m_blockingActors;
    Actor** m_nonBlockingActors;

    bool* m_wallXVisible;
    bool* m_wallYVisible;
};
