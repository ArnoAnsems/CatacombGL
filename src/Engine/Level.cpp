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

#include "Level.h"
#include "PlayerInventory.h"
#include "EgaGraph.h"
#include "RenderableSprites.h"
#include "Renderable3DWalls.h"
#include "RenderableAutoMapTopDown.h"
#include "RenderableAutoMapIso.h"
#include "Renderable3DScene.h"
#include "LevelLocationNames.h"

Level::Level(
    const uint8_t mapIndex,
    const uint16_t mapWidth,
    const uint16_t mapHeight,
    const uint16_t* plane0,
    const uint16_t* plane2,
    const LevelInfo& mapInfo,
    const std::vector<WallInfo>& wallsInfo):
    m_levelWidth (mapWidth),
    m_levelHeight (mapHeight),
    m_levelInfo (mapInfo),
    m_wallsInfo (wallsInfo),
    m_lightningStartTimestamp(0),
    m_levelIndex(mapIndex),
    m_visibilityMap(nullptr),
    m_fogOfWarMap(nullptr),
    m_playerActor(nullptr),
    m_blockingActors(nullptr),
    m_nonBlockingActors(nullptr),
    m_wallXVisible(nullptr),
    m_wallYVisible(nullptr)
{
    const uint16_t mapSize = m_levelWidth * m_levelHeight;
    m_plane0 = new uint16_t[mapSize];
    m_plane2 = new uint16_t[mapSize];
    for ( uint16_t i = 0; i < mapSize; i++)
    {
        m_plane0[i] = plane0[i];
        m_plane2[i] = plane2[i];
    }

    m_visibilityMap = new bool[mapSize];
    m_fogOfWarMap = new bool[mapSize];

    m_blockingActors = new Actor*[mapSize];
    for ( uint16_t i = 0; i < mapSize; i++)
    {
        m_blockingActors[i] = nullptr;
    }

    m_nonBlockingActors = new Actor*[m_maxNonBlockingActors];
    for ( uint16_t i = 0; i < m_maxNonBlockingActors; i++)
    {
        m_nonBlockingActors[i] = nullptr;
    }

    m_wallXVisible = new bool[m_levelWidth * m_levelHeight];
    m_wallYVisible = new bool[m_levelWidth * m_levelHeight];

    for (uint32_t i = 0u; i < (uint32_t)(m_levelWidth * m_levelHeight); i++)
    {
        m_visibilityMap[i] = false;
        m_fogOfWarMap[i] = false;
        m_wallXVisible[i] = false;
        m_wallYVisible[i] = false;
    }

    UpdateLocationNamesBestPositions();
}

bool Level::LoadActorsFromFile(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors)
{
    m_playerActor = new Actor(file, decorateActors);
    uint16_t numberOfBlockingActors = 0;
    file.read((char*)&numberOfBlockingActors, sizeof(numberOfBlockingActors));
    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to read number of blocking actors from saved game");
    }
    for (uint16_t i = 0; i < numberOfBlockingActors; i++)
    {
        Actor* blockingActor = new Actor(file, decorateActors);
        SetBlockingActor(blockingActor->GetTileX(), blockingActor->GetTileY(), blockingActor);
    }

    uint16_t numberOfNonBlockingActors = 0;
    file.read((char*)&numberOfNonBlockingActors, sizeof(numberOfNonBlockingActors));
    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to read number of non-blocking actors from saved game");
    }
    if (numberOfNonBlockingActors > m_maxNonBlockingActors)
    {
        Logging::Instance().FatalError("Saved game contains " + std::to_string(numberOfNonBlockingActors) + " non-blocking actors, while " + std::to_string(m_maxNonBlockingActors) + " is the maximum");
    }
    for (uint16_t i = 0; i < numberOfNonBlockingActors; i++)
    {
        Actor* nonBlockingActor = new Actor(file, decorateActors);
        AddNonBlockingActor(nonBlockingActor);
    }

    return true;
}

bool Level::LoadFogOfWarFromFile(std::ifstream& file)
{
    file.read((char*)m_fogOfWarMap, m_levelWidth * m_levelHeight * sizeof(m_fogOfWarMap[0]));
    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to read fog of war map from saved game");
        return false;
    }

    return true;
}

Level::~Level()
{
    delete[] m_plane0;
    delete[] m_plane2;

    delete[] m_visibilityMap;
    m_visibilityMap = nullptr;

    delete[] m_fogOfWarMap;
    m_fogOfWarMap = nullptr;

    delete[] m_wallXVisible;
    delete[] m_wallYVisible;

    delete m_playerActor;

    if (m_blockingActors != nullptr)
    {
        for ( uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
        {
            delete m_blockingActors[i];
            m_blockingActors[i] = nullptr;
        }

        delete[] m_blockingActors;
        m_blockingActors = nullptr;
    }

    if (m_nonBlockingActors != nullptr)
    {
        for ( uint16_t i = 0; i < m_maxNonBlockingActors; i++)
        {
            delete m_nonBlockingActors[i];
            m_nonBlockingActors[i] = nullptr;
        }
        delete[] m_nonBlockingActors;
        m_nonBlockingActors = nullptr;
    }
}

void Level::StoreToFile(std::ofstream& file) const
{
    file.write((const char*)&m_levelIndex, sizeof(m_levelIndex));
    file.write((const char*)&m_levelWidth, sizeof(m_levelWidth));
    file.write((const char*)&m_levelHeight, sizeof(m_levelHeight));
    file.write((const char*)m_plane0, m_levelWidth * m_levelHeight * sizeof(m_plane0[0]));
    file.write((const char*)m_plane2, m_levelWidth * m_levelHeight * sizeof(m_plane2[0]));
    file.write((const char*)&m_lightningStartTimestamp, sizeof(m_lightningStartTimestamp));
    m_playerActor->StoreToFile(file);
    uint16_t numberOfBlockingActors = 0;
    for (uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
    {
        if (m_blockingActors[i] != nullptr)
        {
            numberOfBlockingActors++;
        }
    }
    file.write((const char*)&numberOfBlockingActors, sizeof(numberOfBlockingActors));
    for (uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
    {
        if (m_blockingActors[i] != nullptr)
        {
            m_blockingActors[i]->StoreToFile(file);
        }
    }
    uint16_t numberOfNonBlockingActors = 0;
    for (uint16_t i = 0; i < m_maxNonBlockingActors; i++)
    {
        if (m_nonBlockingActors[i] != nullptr)
        {
            numberOfNonBlockingActors++;
        }
    }
    file.write((const char*)&numberOfNonBlockingActors, sizeof(numberOfNonBlockingActors));
    for (uint16_t i = 0; i < m_maxNonBlockingActors; i++)
    {
        if (m_nonBlockingActors[i] != nullptr)
        {
            m_nonBlockingActors[i]->StoreToFile(file);
        }
    }
    file.write((const char*)m_fogOfWarMap, m_levelWidth * m_levelHeight * sizeof(m_fogOfWarMap[0]));
}

uint16_t Level::GetLevelWidth() const
{
    return m_levelWidth;
}

uint16_t Level::GetLevelHeight() const
{
    return m_levelHeight;
}

uint16_t Level::GetWallTile(const uint16_t x, const uint16_t y) const
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("GetWallTile(" + std::to_string(x) + "," + std::to_string(y) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    return m_plane0[(y * m_levelWidth) + x];
}

uint16_t Level::GetFloorTile(const uint16_t x, const uint16_t y) const
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("GetFloorTile(" + std::to_string(x) + "," + std::to_string(y) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    return m_plane2[(y * m_levelWidth) + x];
}

void Level::SetWallTile(const uint16_t x, const uint16_t y, const uint16_t wallTile)
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("SetWallTile(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(wallTile) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    m_plane0[(y * m_levelWidth) + x] = wallTile;
}

void Level::SetFloorTile(const uint16_t x, const uint16_t y, const uint16_t floorTile)
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("SetFloorTile(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(floorTile) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    m_plane2[(y * m_levelWidth) + x] = floorTile;
}

bool Level::IsSolidWall(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    return ((wallTile < m_wallsInfo.size()) && (m_wallsInfo.at(wallTile).wallType != WTOpen));
}

bool Level::IsVisibleTile(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    const WallType wallType = (wallTile < m_wallsInfo.size()) ? m_wallsInfo.at(wallTile).wallType : WTOpen;
    return (wallType == WTOpen || wallType == WTInvisibleWall);
}

bool Level::IsExplosiveWall(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    if (wallTile < m_wallsInfo.size() && m_wallsInfo.at(wallTile).wallType == WTDestructable)
    {
        return true;
    }
    const uint16_t spot = (GetFloorTile(x,y) >> 8);
    return (spot == 0xfc);
}

bool Level::IsDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    const WallType wallType = (wallTile < m_wallsInfo.size()) ? m_wallsInfo.at(wallTile).wallType : WTOpen;
    return
        (wallType == WTDoor ||
         wallType == WTDoorRedKeyRequired ||
         wallType == WTDoorYellowKeyRequired ||
         wallType == WTDoorGreenKeyRequired ||
         wallType == WTDoorBlueKeyRequired);
}

bool Level::IsRemovableDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t spot = (GetFloorTile(x,y) >> 8);
    return (spot == 0xfe);
}

bool Level::IsExitDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t spot = (GetFloorTile(x,y) >> 8);
    return (spot == 0xff);
}

bool Level::IsVictoryDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    return ((wallTile < m_wallsInfo.size()) && (m_wallsInfo.at(wallTile).wallType == WTVictory));
}

bool Level::IsBlockedDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t spot = (GetFloorTile(x,y) >> 8);
    return (spot == 0xfd);
}

bool Level::IsFakeWall(const uint16_t x, const uint16_t y) const
{
    const Actor* actor = GetBlockingActor(x, y);
    return (actor == nullptr) ? false : (actor->GetAction() == ActionFakeWall);
}

KeyId Level::GetRequiredKeyForDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t tile = GetWallTile(x, y);
    if (tile >= m_wallsInfo.size())
    {
        // This is a floor tile
        return NoKey;
    }

    const WallType wallType = m_wallsInfo.at(tile).wallType;
    if (wallType == WTDoorRedKeyRequired)
    {
        return RedKey;
    }
    if (wallType == WTDoorYellowKeyRequired)
    {
        return YellowKey;
    }
    if (wallType == WTDoorGreenKeyRequired)
    {
        return GreenKey;
    }
    if (wallType == WTDoorBlueKeyRequired)
    {
        return BlueKey;
    }
    const uint16_t spot = (y < m_levelHeight - 1) ? (GetFloorTile(x, y + 1) >> 8) : 0;
    return (spot > 0 && spot < 5) ? (KeyId)(spot - 1) : NoKey;
}

const LevelInfo& Level::GetMapInfo() const
{
    return m_levelInfo;
}

const char* Level::GetLevelName() const
{
    return m_levelInfo.name.c_str();
}

const egaColor Level::GetSkyColor(const uint32_t timeStamp)
{
    if (m_levelInfo.showDusk && timeStamp < 2500 * 4)
    {
        const egaColor sky_daytonight[]={EgaBrightBlue,EgaBlue,EgaDarkGray,EgaBlack};
        return sky_daytonight[timeStamp / 2500];
    }
    else if (m_levelInfo.showLightning)
    {
        if (m_lightningStartTimestamp < timeStamp)
        {
            // Time to show the lightning
            const egaColor sky_lightning[]={EgaBlue,EgaBrightBlue,EgaBrightWhite,EgaDarkGray,EgaBlack};
            const uint32_t lightningFrameDuration = 14; //ms (70 Hz)
            const int8_t lightningNoFrames = sizeof(sky_lightning) / sizeof(egaColor);
            const uint32_t lightningDuration = lightningFrameDuration * lightningNoFrames;
            const uint32_t timeSinceStartLightning = timeStamp - m_lightningStartTimestamp;
            const uint32_t lightningFrame = timeSinceStartLightning / lightningFrameDuration;
            if (lightningFrame < lightningNoFrames)
            {
                // Show lightning
                return sky_lightning[lightningFrame];
            }
            else
            {
                // All lightning frames were shown.
                // Set the next lightning to strike between 1 and 10 seconds later.
                const uint32_t timeToNextLightning = (1000 + rand() % 9000);
                m_lightningStartTimestamp += timeToNextLightning;
            }
        }
    }

    return m_levelInfo.ceilingColor;
}

const egaColor Level::GetGroundColor() const
{
    return m_levelInfo.floorColor;
}

uint8_t Level::GetLevelIndex() const
{
    return m_levelIndex;
}

void Level::UpdateVisibilityMap()
{
    for (uint32_t i = 0u; i < (uint32_t)(m_levelWidth * m_levelHeight); i++)
    {
        m_visibilityMap[i] = false;
        m_wallXVisible[i] = false;
        m_wallYVisible[i] = false;
    }

    LevelCoordinate coordinateOnOuterWall = { 0.0f, 0.0f };
    bool done = false;
    LevelWall firstWallBackTraced = { 0, 0, true };
    LevelWall firstWallHit = { 0 ,0, true };
    float previousDistance = -1.0f;
    bool retryDistance = false;
    while (!done)
    {
        LevelWall wallHit;
        RayTraceWall(coordinateOnOuterWall, wallHit);
        if (firstWallHit.x == 0 && firstWallHit.y == 0)
        {
            firstWallHit = wallHit;
        }
        const uint16_t wallArrayIndex = (wallHit.y * m_levelWidth) + wallHit.x;
        done = (!(firstWallBackTraced.x == wallHit.x && firstWallBackTraced.y == wallHit.y && firstWallBackTraced.isXWall == wallHit.isXWall)) &&
            ((wallHit.isXWall && m_wallXVisible[wallArrayIndex]) ||
            (!wallHit.isXWall && m_wallYVisible[wallArrayIndex]));
        if (!done)
        {
            if (wallHit.isXWall)
            {
                m_wallXVisible[wallArrayIndex] = true;
            }
            else
            {
                m_wallYVisible[wallArrayIndex] = true;
            }
            const LevelCoordinate wallEdge = GetRightEdgeOfWall(wallHit);
            const LevelCoordinate intersection = GetIntersectionWithOuterWall(wallEdge);
            const float distance = GetDistanceOnOuterWall(intersection);
            if (abs(distance - previousDistance) < 0.0000001f)
            {
                // Distance did not change; we must have hit the same wall again.
                if (retryDistance)
                {
                    // Already retried once; prevent infinite loop
                    done = true;
                }
                else
                {
                    // Try one more time with additional distance.
                    retryDistance = true;
                }
            }
            else
            {
                // Distance changed as expected
                previousDistance = distance;
                retryDistance = false;
            }
            BackTraceWalls(distance, firstWallBackTraced);
            const float additionalDistance = (retryDistance) ? 0.01f : 0.001f;
            coordinateOnOuterWall = GetOuterWallCoordinate(distance + additionalDistance);
        }
    }

    const LevelCoordinate wallEdgeLeft = GetLeftEdgeOfWall(firstWallHit);
    const LevelCoordinate intersection2 = GetIntersectionWithOuterWall(wallEdgeLeft);
    const float distanceForBackTracing = GetDistanceOnOuterWall(intersection2) - 0.001f;
    BackTraceWalls(distanceForBackTracing, firstWallBackTraced);

    for (uint16_t x = 1; x < m_levelWidth - 1; x++)
    {
        for (uint16_t y = 1; y < m_levelHeight - 1; y++)
        {
            const uint16_t tileIndex = (y * m_levelWidth) + x;
            if (IsVisibleTile(x, y) &&
                (m_wallXVisible[tileIndex] ||
                m_wallXVisible[tileIndex + 1] ||
                m_wallYVisible[tileIndex] ||
                m_wallYVisible[tileIndex + m_levelWidth]))
            {
                m_visibilityMap[(y * m_levelWidth) + x] = true;
            }
        }
    }

    for (uint16_t x = 0; x < m_levelWidth; x++)
    {
        for (uint16_t y = 0; y < m_levelHeight; y++)
        {
            const uint16_t tileIndex = (y * m_levelWidth) + x;
            m_fogOfWarMap[tileIndex] |=
                (m_wallXVisible[tileIndex] ||
                 m_wallYVisible[tileIndex] ||
                 (y < m_levelHeight - 1 && m_wallYVisible[tileIndex + m_levelWidth]) ||
                 (x < m_levelWidth - 1 && m_wallXVisible[tileIndex + 1]));
        }
    }
}

void Level::BackTraceWalls(const float distanceOnOuterWall, LevelWall& firstWall)
{
    firstWall = { 0, 0, true };
    float distanceForBackTracing = distanceOnOuterWall;
    bool doneBackTracing = false;
    float previousDistance = distanceOnOuterWall + 1.0f;
    bool retryDistance = false;
    while (!doneBackTracing)
    {
        if (abs(distanceForBackTracing - previousDistance) < 0.0000001f)
        {
            // Distance did not change; we must have hit the same wall again.
            if (retryDistance)
            {
                // Already retried once; prevent infinite loop
                doneBackTracing = true;
            }
            else
            {
                // Try one more time with additional distance.
                retryDistance = true;
            }
        }
        else
        {
            // Distance changed as expected
            previousDistance = distanceForBackTracing;
            retryDistance = false;
        }
        const float additionalDistance = (retryDistance) ? 0.01f : 0.001f;
        const LevelCoordinate leftCoordinate = GetOuterWallCoordinate(distanceForBackTracing - additionalDistance);
        LevelWall wallHit;
        RayTraceWall(leftCoordinate, wallHit);
        const uint16_t wallHitIndex = (wallHit.y * m_levelWidth) + wallHit.x;
        if (firstWall.x == 0 && firstWall.y == 0)
        {
            firstWall = wallHit;
        }
        doneBackTracing =
            ((wallHit.isXWall && m_wallXVisible[wallHitIndex]) ||
            (!wallHit.isXWall && m_wallYVisible[wallHitIndex]));

        if (!doneBackTracing)
        {
            if (wallHit.isXWall)
            {
                m_wallXVisible[wallHitIndex] = true;
            }
            else
            {
                m_wallYVisible[wallHitIndex] = true;
            }

            const LevelCoordinate wallEdgeLeft = GetLeftEdgeOfWall(wallHit);
            const LevelCoordinate intersection = GetIntersectionWithOuterWall(wallEdgeLeft);
            distanceForBackTracing = GetDistanceOnOuterWall(intersection);
        }
    }
}

LevelCoordinate Level::GetOuterWallCoordinate(const float distance) const
{
    LevelCoordinate coordinate = { 1.0f, 1.0f };
    float remainingDistance = distance;

    if (remainingDistance < 0.0f)
    {
        // West wall
        coordinate = { 0.0f, -remainingDistance };
        return coordinate;
    }

    // North wall
    if (remainingDistance < (float)m_levelWidth)
    {
        coordinate = { distance, 0.0f };
        return coordinate;
    }
    else
    {
        remainingDistance -= (float)m_levelWidth;
    }

    // East wall
    if (remainingDistance < (float)m_levelHeight)
    {
        coordinate = { (float)m_levelWidth, remainingDistance };
        return coordinate;
    }
    else
    {
        remainingDistance -= (float)m_levelHeight;
    }

    // South wall
    if (remainingDistance < (float)m_levelWidth)
    {
        coordinate = { (float)m_levelWidth - remainingDistance, (float)m_levelHeight };
        return coordinate;
    }
    else
    {
        remainingDistance -= (float)m_levelWidth;
    }

    // West wall
    if (remainingDistance < (float)m_levelHeight)
    {
        coordinate = { 0.0f, (float)m_levelHeight - remainingDistance };
        return coordinate;
    }
    else
    {
        remainingDistance -= (float)m_levelHeight;
    }

    coordinate = { remainingDistance, 0.0f };
    return coordinate;
}

float Level::GetDistanceOnOuterWall(const LevelCoordinate& coordinate) const
{
    // North wall
    if (abs(coordinate.y) < 0.000000001f)
    {
        return coordinate.x;
    }

    // East wall
    if (abs(coordinate.x - (float)m_levelWidth) < 0.000000001f)
    {
        return (float)m_levelWidth + coordinate.y;
    }

    // South wall
    if (abs(coordinate.y - (float)m_levelHeight) < 0.00000001f)
    {
        return (float)(2 * m_levelWidth) + (float)m_levelHeight - coordinate.x;
    }

    // West wall
    if (abs(coordinate.x) < 0.000000001f)
    {
        return (float)(2 * m_levelWidth) + (float)(2 * m_levelHeight) - coordinate.y;
    }

    return 0.0f;
}

LevelCoordinate Level::GetRightEdgeOfWall(LevelWall& wall) const
{
    const float x = m_playerActor->GetX();
    const float y = m_playerActor->GetY();
    const float levelX = (!wall.isXWall && y > wall.y) ? (float)wall.x + 1.0f : (float)wall.x;
    const float levelY = (wall.isXWall && x <= wall.x) ? (float)wall.y + 1.0f : (float)wall.y;
    return { levelX, levelY };
}

LevelCoordinate Level::GetLeftEdgeOfWall(LevelWall& wall) const
{
    const float x = m_playerActor->GetX();
    const float y = m_playerActor->GetY();
    const float levelX = (!wall.isXWall && y < wall.y) ? (float)wall.x + 1.0f : (float)wall.x;
    const float levelY = (wall.isXWall && x >= wall.x) ? (float)wall.y + 1.0f : (float)wall.y;
    return { levelX, levelY };
}

LevelCoordinate Level::GetIntersectionWithOuterWall(const LevelCoordinate& coordinateInView) const
{
    const float x = m_playerActor->GetX();
    const float y = m_playerActor->GetY();
    bool hitFound = false;
    LevelCoordinate intersection = { 0.0f, 0.0f };
    if (abs(x - coordinateInView.x) < 0.000001)
    {
        if (y > coordinateInView.y)
        {
            intersection = { x, 0.0f };
        }
        else
        {
            intersection = { x, (float)m_levelHeight };
        }
    }
    else if (abs(y - coordinateInView.y) < 0.000001)
    {
        if (x > coordinateInView.x)
        {
            intersection = { 0.0f, y};
        }
        else
        {
            intersection = { (float)m_levelWidth, y };
        }
    }
    else 
    {
        if (coordinateInView.x < x)
        {
            // Test for hit on West wall
            const float normalY = (coordinateInView.y - y) / (x - coordinateInView.x);
            const float hitWally = ((x * normalY) + y);
            if (hitWally >= 0.0f && hitWally <= (float)m_levelHeight)
            {
                intersection = { 0.0f, hitWally };
                hitFound = true;
            }
        }
        else
        {
            // Test for hit on East wall
            const float normalY = (coordinateInView.y - y) / (coordinateInView.x - x);
            const float hitWally = ((((float)m_levelWidth - x) * normalY) + y);
            if (hitWally >= 0.0f && hitWally <= (float)m_levelHeight)
            {
                intersection = { (float)m_levelWidth, hitWally };
                hitFound = true;
            }
        }

        if (!hitFound)
        {
            if (coordinateInView.y < y)
            {
                // Test for hit on North wall
                const float normalX = (coordinateInView.x - x) / (y - coordinateInView.y);
                const float hitWallx = ((y * normalX) + x);
                if (hitWallx >= 0.0f && hitWallx <= (float)m_levelWidth)
                {
                    intersection = { hitWallx, 0.0f };
                }
            }
            else
            {
                // Test for hit on South wall
                const float normalX = (coordinateInView.x - x) / (coordinateInView.y - y);
                const float hitWallx = ((((float)m_levelHeight - y) * normalX) + x);
                if (hitWallx >= 0.0f && hitWallx <= (float)m_levelWidth)
                {
                    intersection = { hitWallx, (float)m_levelHeight };
                }
            }
        }
    }

    return intersection;
}

void Level::RayTraceWall(const LevelCoordinate& coordinateInView, LevelWall& wallHit)
{
    const float x = coordinateInView.x;
    const float y = coordinateInView.y;
    enum TraceState
    {
        LookingForWall,
        NoWallFound,
        WallFound,
        WallIsFurtherAway
    };

    TraceState traceStateX = (abs(m_playerActor->GetX() - x) < 0.000001 ||
        ((x > m_playerActor->GetX()) && x <= (float)(int32_t)(m_playerActor->GetX() + 1.0f)) ||
        ((x < m_playerActor->GetX()) && x >= (float)(int32_t)m_playerActor->GetX())) ? NoWallFound : LookingForWall;

    TraceState traceStateY = (abs(m_playerActor->GetY() - y) < 0.000001 ||
        ((y > m_playerActor->GetY()) && y <= (float)(int32_t)(m_playerActor->GetY() + 1.0f)) ||
        ((y < m_playerActor->GetY()) && y >= (float)(int32_t)m_playerActor->GetY())) ? NoWallFound : LookingForWall;

    uint16_t hitWallX_x = 0;
    float hitWallX_y = 0.0f;
    uint16_t hitWallY_y = 0;
    float hitWallY_x = 0.0f;
    int32_t tileX = (x >= m_playerActor->GetX()) ? (int32_t)(m_playerActor->GetX() + 1.0f) : (int32_t)m_playerActor->GetX();
    int32_t tileY = (y >= m_playerActor->GetY()) ? (int32_t)(m_playerActor->GetY() + 1.0f) : (int32_t)m_playerActor->GetY();

    float squareDistanceX = 0.0f;
    float squareDistanceY = 0.0f;

    const float normalX = (abs(m_playerActor->GetY() - y) < 0.000001) ? 100000.0f : ((float)x - m_playerActor->GetX()) / ((float)y - m_playerActor->GetY());
    const float normalY = (abs(m_playerActor->GetX() - x) < 0.000001) ? 100000.0f : ((float)y - m_playerActor->GetY()) / ((float)x - m_playerActor->GetX());

    while (traceStateX == LookingForWall || traceStateY == LookingForWall)
    {
        if ((traceStateX == LookingForWall) &&
           ((traceStateY == LookingForWall && squareDistanceX <= squareDistanceY) ||
            (traceStateY != LookingForWall)))
        {
            const bool traceEast = (x > m_playerActor->GetX() && (float)tileX < x);
            const bool traceWest = (x <= m_playerActor->GetX() && (float)tileX > x);
            if (traceWest || traceEast)
            {
                hitWallX_x = (uint16_t)tileX;
                hitWallX_y = ((((float)tileX - m_playerActor->GetX()) * normalY) + m_playerActor->GetY());

                const float deltaX_x = abs(m_playerActor->GetX() - (float)hitWallX_x);
                const float deltaX_y = abs(m_playerActor->GetY() - hitWallX_y);
                squareDistanceX = (deltaX_x * deltaX_x) + (deltaX_y * deltaX_y);

                const uint16_t nextTileX = (traceEast) ? hitWallX_x : hitWallX_x - 1;
                if (squareDistanceX > squareDistanceY && traceStateY == WallFound)
                {
                    traceStateX = WallIsFurtherAway;
                }
                else if (!IsVisibleTile(nextTileX, (uint16_t)hitWallX_y))
                {
                    traceStateX = WallFound;
                }
                else if (squareDistanceX > squareDistanceY && traceStateY == LookingForWall && hitWallY_y > 0)
                {
                    m_wallYVisible[(hitWallY_y * m_levelWidth) + (uint16_t)hitWallY_x] = true;
                }
                else if (squareDistanceX <= squareDistanceY || traceStateY == NoWallFound)
                {
                    m_wallXVisible[((uint16_t)hitWallX_y * m_levelWidth) + hitWallX_x] = true;
                }
                tileX += (traceEast) ? 1 : -1;
            }
            else
            {
                traceStateX = NoWallFound;
            }
        }
        else
        {
            const bool traceNorth = (y > m_playerActor->GetY() && (float)tileY < y);
            const bool traceSouth = (y <= m_playerActor->GetY() && (float)tileY > y);
            if (traceNorth || traceSouth)
            {
                hitWallY_x = ((((float)tileY - m_playerActor->GetY()) * normalX) + m_playerActor->GetX());
                hitWallY_y = (uint16_t)tileY;

                const float deltaY_x = abs(m_playerActor->GetX() - hitWallY_x);
                const float deltaY_y = abs(m_playerActor->GetY() - (float)hitWallY_y);
                squareDistanceY = (deltaY_x * deltaY_x) + (deltaY_y * deltaY_y);

                const uint16_t nextTileY = (traceNorth) ? hitWallY_y : hitWallY_y - 1;
                if (squareDistanceY >= squareDistanceX && traceStateX == WallFound)
                {
                    traceStateY = WallIsFurtherAway;
                }
                else if (!IsVisibleTile((uint16_t)hitWallY_x, nextTileY))
                {
                    traceStateY = WallFound;
                }
                else if (squareDistanceY > squareDistanceX && traceStateX == LookingForWall && hitWallX_x > 0)
                {
                    m_wallXVisible[((uint16_t)hitWallX_y * m_levelWidth) + hitWallX_x] = true;
                }
                else if (squareDistanceY <= squareDistanceX || traceStateX == NoWallFound)
                {
                    m_wallYVisible[(hitWallY_y * m_levelWidth) + (uint16_t)hitWallY_x] = true;
                }
                tileY += (traceNorth) ? 1 : -1;
            }
            else
            {
                traceStateY = NoWallFound;
            }
        }
    }

    wallHit.isXWall = (traceStateX == WallFound && squareDistanceX <= squareDistanceY) || traceStateY != WallFound;
    wallHit.x = wallHit.isXWall ? hitWallX_x : (uint16_t)hitWallY_x;
    wallHit.y = wallHit.isXWall ? (uint16_t)hitWallX_y : hitWallY_y;
}

bool Level::IsTileVisibleForPlayer(const uint16_t x, const uint16_t y) const
{
    return m_visibilityMap[y * m_levelWidth + x];
}

bool Level::IsTileClearFromFogOfWar(const uint16_t x, const uint16_t y) const
{
    return m_fogOfWarMap[y * m_levelWidth + x];
}

bool Level::IsActorVisibleForPlayer(const Actor* actor) const
{
    const float actorSize = actor->GetDecorateActor().size;
    const uint16_t xl = (uint16_t)(actor->GetX() - actorSize);
    const uint16_t xh = (uint16_t)(actor->GetX() + actorSize);
    const uint16_t yl = (uint16_t)(actor->GetY() - actorSize);
    const uint16_t yh = (uint16_t)(actor->GetY() + actorSize);

    bool visible = false;
    for (uint16_t y = yl; y <= yh; y++)
        for (uint16_t x = xl; x <= xh; x++)
        {
            visible |= m_visibilityMap[y * m_levelWidth + x];
        }

    return visible;
}

Actor* const Level::GetPlayerActor()
{
    return m_playerActor;
}

void Level::SetPlayerActor(Actor* const actor)
{
    m_playerActor = actor;
}

Actor** Level::GetBlockingActors()
{
    return m_blockingActors;
}

Actor** Level::GetNonBlockingActors()
{
    return m_nonBlockingActors;
}

uint16_t Level::GetMaxNonBlockingActors() const
{
    return m_maxNonBlockingActors;
}

void Level::SetBlockingActor(const uint16_t x, const uint16_t y, Actor* actor)
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("SetBlockingActor(" + std::to_string(x) + "," + std::to_string(y) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    m_blockingActors[(y * m_levelWidth) + x] = actor;
}

Actor* Level::GetBlockingActor(const uint16_t x, const uint16_t y) const
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("GetBlockingActor(" + std::to_string(x) + "," + std::to_string(y) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    return m_blockingActors[(y * m_levelWidth) + x];
}

Actor* Level::GetNonBlockingActor(const uint16_t index) const
{
    return m_nonBlockingActors[index];
}

void Level::AddNonBlockingActor(Actor* projectile)
{
    uint16_t i = 0;

    // To ensure that there is always a free index available when Nemesis needs to drop a key,
    // the last five indices can only be occupied by bonus items.
    // To ensure that the corpses of fallen monsters can spawn after crystal hour glass usages, the
    // ten indices before that cannot be occupied by projectiles.
    const uint16_t maxNonBonusItems = m_maxNonBlockingActors - 5;
    const uint16_t maxProjectiles = maxNonBonusItems - 10;
    const bool isBonusItem = (projectile->GetDecorateActor().initialState == StateIdWaitForPickup);
    const bool isProjectile = (projectile->GetDecorateActor().initialState == StateIdProjectileFly);
    const uint8_t maxActorIndex = isBonusItem ? m_maxNonBlockingActors : isProjectile ? maxProjectiles : maxNonBonusItems;
    
    while (m_nonBlockingActors[i] != nullptr && i < maxActorIndex)
    {
        i++;
    }
    if (i < maxActorIndex)
    {
        m_nonBlockingActors[i] = projectile;
    }
}

bool Level::Walk(Actor* const actor)
{
    const uint8_t tileX = actor->GetTileX();
    const uint8_t tileY = actor->GetTileY();
    switch (actor->GetDirection())
    {
    case north:
        if ((GetBlockingActor(tileX, tileY - 1) != nullptr) || IsSolidWall(tileX, tileY - 1))
        {
            return false;
        }
        actor->SetTile(tileX, tileY - 1);
        return true;

    case northeast:
        if ((GetBlockingActor(tileX + 1, tileY - 1) != nullptr) || IsSolidWall(tileX + 1, tileY - 1))
        {
            return false;
        }
        actor->SetTile(tileX + 1, tileY - 1);
        return true;

    case east:
        if ((GetBlockingActor(tileX + 1, tileY) != nullptr) || IsSolidWall(tileX + 1, tileY))
        {
            return false;
        }
        actor->SetTile(tileX + 1, tileY);
        return true;

    case southeast:
        if ((GetBlockingActor(tileX + 1, tileY + 1) != nullptr) || IsSolidWall(tileX + 1, tileY + 1))
        {
            return false;
        }
        actor->SetTile(tileX + 1, tileY + 1);
        return true;

    case south:
        if ((GetBlockingActor(tileX, tileY + 1) != nullptr) || IsSolidWall(tileX, tileY + 1))
        {
            return false;
        }
        actor->SetTile(tileX, tileY + 1);
        return true;

    case southwest:
        if ((GetBlockingActor(tileX - 1, tileY + 1) != nullptr) || IsSolidWall(tileX - 1, tileY + 1))
        {
            return false;
        }
        actor->SetTile(tileX - 1, tileY + 1);
        return true;

    case west:
        if ((GetBlockingActor(tileX - 1, tileY) != nullptr) || IsSolidWall(tileX - 1, tileY))
        {
            return false;
        }
        actor->SetTile(tileX - 1, tileY);
        return true;

    case northwest:
        if ((GetBlockingActor(tileX - 1, tileY - 1) != nullptr) || IsSolidWall(tileX - 1, tileY - 1))
        {
            return false;
        }
        actor->SetTile(tileX - 1, tileY - 1);
        return true;

    case nodir:
        return false;
    }

    //Quit ("Walk: Bad dir");
    return false;
}

void Level::ChaseThink(Actor* const actor, const bool diagonal, const ChaseTarget target)
{
    const actorDirection dirtable[9] = {northwest,north,northeast,west,nodir,east,southwest,south,southeast};
    const actorDirection opposite[9] = {south,west,north,east,southwest,northwest,northeast,southeast,nodir};

    actorDirection tdir = nodir;
    actorDirection d[3];

    const actorDirection olddir = actor->GetDirection();
    const actorDirection turnaround = opposite[olddir];

    float targetX = 0.0f;
    float targetY = 0.0f;

    switch (target)
    {
    case ChaseTopLeftCorner:
        targetX = 0.0f;
        targetY = 0.0f;
        break;
    case ChaseTopRightCorner:
        targetX = 0.0f;
        targetY = (float)m_levelWidth;
        break;
    case ChaseBottomLeftCorner:
        targetX = (float)m_levelHeight;
        targetY = 0.0f;
        break;
    case ChaseBottomRightCorner:
        targetX = (float)m_levelHeight;
        targetY = (float)m_levelWidth;
        break;
    default:
        targetX = m_playerActor->GetX();
        targetY = m_playerActor->GetY();
        break;
    }

    const int8_t deltax = (int8_t)targetX - actor->GetTileX();
    const int8_t deltay = (int8_t)targetY - actor->GetTileY();

    d[1]=nodir;
    d[2]=nodir;

    if (deltax>0)
        d[1]= east;
    if (deltax<0)
        d[1]= west;
    if (deltay>0)
        d[2]=south;
    if (deltay<0)
        d[2]=north;

    if (abs(deltay)>abs(deltax))
    {
        tdir=d[1];
        d[1]=d[2];
        d[2]=tdir;
    }

    if (d[1]==turnaround)
        d[1]=nodir;
    if (d[2]==turnaround)
        d[2]=nodir;


    if (diagonal)
    {                           //ramdiagonals try the best dir first
        if (d[1]!=nodir)
        {
            actor->SetDirection(d[1]);
            if (Walk(actor))
                return;     //either moved forward or attacked
        }

        if (d[2]!=nodir)
        {
            actor->SetDirection(d[2]);
            if (Walk(actor))
                return;
        }
    }
    else
    {                  //ramstraights try the second best dir first
        if (d[2]!=nodir)
        {
            actor->SetDirection(d[2]);
            if (Walk(actor))
                return;
        }

        if (d[1]!=nodir)
        {
            actor->SetDirection(d[1]);
            if (Walk(actor))
                return;
        }
    }

    // there is no direct path to the player, so pick another direction

    actor->SetDirection(olddir);
    if (Walk(actor))
        return;

    if ((rand() % 2) == 0) 	//randomly determine direction of search
    {
        for (int8_t i = (int8_t)north; i <= (int8_t)west;i++)
        {
            tdir = (actorDirection)i;
            if (tdir!=turnaround)
            {
                actor->SetDirection(tdir);
                if (Walk(actor))
                    return;
            }
        }
    }
    else
    {
        for (int8_t i = (int8_t)west;tdir>=(int8_t)north;i--)
        {
            
            tdir = (actorDirection)i;
            if (tdir!=turnaround)
            {
                actor->SetDirection(tdir);
                if (Walk(actor))
                    return;
            }
        }
    }

    actor->SetDirection(turnaround);
    Walk(actor);		//last chance, don't worry about returned value
}

void Level::MoveActor(Actor* const actor, const float distance)
{
    switch (actor->GetDirection())
    {
    case north:
        actor->SetY(actor->GetY() - distance);
        return;
    case northeast:
        actor->SetX(actor->GetX() + distance);
        actor->SetY(actor->GetY() - distance);
        return;
    case east:
        actor->SetX(actor->GetX() + distance);
        return;
    case southeast:
        actor->SetX(actor->GetX() + distance);
        actor->SetY(actor->GetY() + distance);
        return;
    case south:
        actor->SetY(actor->GetY() + distance);
        return;
    case southwest:
        actor->SetX(actor->GetX() - distance);
        actor->SetY(actor->GetY() + distance);
        return;
    case west:
        actor->SetX(actor->GetX() - distance);
        return;
    case northwest:
        actor->SetX(actor->GetX() - distance);
        actor->SetY(actor->GetY() - distance);
        return;

    case nodir:
        return;
    }
}

void Level::RunAwayThink(Actor* const actor)
{
    const actorDirection dirtable[9] = { northwest,north,northeast,west,nodir,east,southwest,south,southeast };

    const actorDirection oldDir = actor->GetDirection();
    actorDirection tdir = nodir;
    actorDirection d[3];

    const float targetX = m_playerActor->GetX();
    const float targetY = m_playerActor->GetY();

    const int8_t deltax = (int8_t)targetX - actor->GetTileX();
    const int8_t deltay = (int8_t)targetY - actor->GetTileY();

    d[1] = nodir;
    d[2] = nodir;

    if (deltax>0)
        d[1] = west;
    if (deltax<0)
        d[1] = east;
    if (deltay>0)
        d[2] = north;
    if (deltay<0)
        d[2] = south;

    if (abs(deltay)>abs(deltax))
    {
        tdir = d[1];
        d[1] = d[2];
        d[2] = tdir;
    }

    actor->SetDirection(d[1]);
    if (Walk(actor))
        return;     //either moved forward or attacked

    actor->SetDirection(d[2]);
    if (Walk(actor))
        return;

    // there is no direct path away from the player, so pick another direction

    actor->SetDirection(oldDir);
    if (Walk(actor))
        return;

    if ((rand() % 2) == 0) 	//randomly determine direction of search
    {
        for (int8_t i = (int8_t)north; i <= (int8_t)west; i++)
        {
            tdir = (actorDirection)i;
            actor->SetDirection(tdir);
            if (Walk(actor))
                return;
        }
    }
    else
    {
        for (int8_t i = (int8_t)west; tdir >= (int8_t)north; i--)
        {

            tdir = (actorDirection)i;
            actor->SetDirection(tdir);
            if (Walk(actor))
                return;
        }
    }
}

int16_t Level::AngleNearPlayer(const Actor* const actor) const
{
    int16_t angle = -1;
    const float dx =  m_playerActor->GetX() - actor->GetX();
    const float dy =  m_playerActor->GetY() - actor->GetY();
    if (abs(dx) < 0.5)
    {
        if (dy < 0)
        {
            angle = 0;
        }
        else
        {
            angle = 180;
        }
    }
    else if (abs(dy) < 0.5)
    {
        if (dx < 0)
        {
            angle = 270;
        }
        else
        {
            angle = 90;
        }
    }
    else if (abs(abs(dx) - abs(dy)) < 1.0)
    {
        if (dx > 0)
        {
            if (dy > 0)
            {
                angle = 135;
            }
            else
            {
                angle = 45;
            }
        }
        else // dx <= 0
        {
            if (dy > 0)
            {
                angle = 225;
            }
            else
            {
                angle = 315;
            }
        }
    }

     return(angle);
}

void Level::ExplodeWall(const uint16_t x, const uint16_t y, const uint32_t timestamp, const DecorateActor& explodingWallActor)
{
    if (x >= m_levelWidth || y >= m_levelHeight)
    {
        Logging::Instance().FatalError("ExplodeWall(" + std::to_string(x) + "," + std::to_string(y) + ") is outside of bounds (" + std::to_string(m_levelWidth) + "," + std::to_string(m_levelHeight) + ")");
    }

    if (m_blockingActors[(y * m_levelWidth) + x] == nullptr)
    {
        m_blockingActors[(y * m_levelWidth) + x] = new Actor(x, y, timestamp, explodingWallActor);
        m_blockingActors[(y * m_levelWidth) + x]->SetActive(true);
    }
}

void Level::SpawnExplosion(const float x, const float y, const int16_t delay, const uint32_t timestamp, const DecorateActor& decorateActor)
{
    Actor* actor = new Actor(x, y, timestamp, decorateActor);
    actor->SetTemp2(delay);
    AddNonBlockingActor(actor);
}

void Level::SpawnBigExplosion(const float x, const float y, const uint16_t delay, const uint32_t range, const uint32_t timestamp, const DecorateActor& decorateActor)
{
    SpawnExplosion(x-((rand() % range) / 65536.0f),y+((rand() % range) / 65536.0f),rand() % delay, timestamp, decorateActor);
    SpawnExplosion(x+((rand() % range) / 65536.0f),y-((rand() % range) / 65536.0f),rand() % delay, timestamp, decorateActor);
    SpawnExplosion(x-((rand() % range) / 65536.0f),y-((rand() % range) / 65536.0f),rand() % delay, timestamp, decorateActor);
    SpawnExplosion(x+((rand() % range) / 65536.0f),y+((rand() % range) / 65536.0f),rand() % delay, timestamp, decorateActor);
}

bool Level::IsWaterLevel() const
{
    return GetGroundColor() == EgaBlue;
}
void Level::Setup3DScene(
    EgaGraph& egaGraph,
    Renderable3DScene& renderable3DScene,
    const uint32_t timeStamp,
    const uint32_t ticks)
{
    Renderable3DTiles& renderable3DTiles = renderable3DScene.Get3DTilesMutable();
    for (int16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (int16_t x = 1; x < m_levelWidth - 1; x++)
        {
            if (IsTileVisibleForPlayer(x, y))
            {
                renderable3DTiles.AddTile(Renderable3DTiles::tileCoordinate{ x, y });
            }
        }
    }
    renderable3DTiles.SetOnlyFloor(false);
    renderable3DTiles.SetFloorColor(GetGroundColor());
    renderable3DTiles.SetCeilingColor(GetSkyColor(timeStamp));

    const float x1 = m_playerActor->GetX();
    const float y1 = m_playerActor->GetY();
    const float x2 = x1 - 1.0f * (float)sin((m_playerActor->GetAngle() + 270.0f) * 3.14159265f / 180.0f);
    const float y2 = y1 + 1.0f * (float)cos((m_playerActor->GetAngle() + 270.0f) * 3.14159265f / 180.0f);

    for (uint16_t y = 1; y < m_levelHeight; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth; x++)
        {
            if (m_wallYVisible[(y * m_levelWidth) + x])
            {
                if (IsPointVisible((float)x, (float)y, x1, y1, x2, y2) ||
                    IsPointVisible((float)x + 1, (float)y, x1, y1, x2, y2))
                {
                    const uint16_t northwallIndex = GetWallTile(x, y - 1);
                    const uint16_t northWall = GetDarkWallPictureIndex(northwallIndex, ticks);
                    if (northWall != 1)
                    {
                        const Picture* northPicture = egaGraph.GetPicture(northWall);
                        if (northPicture != nullptr)
                        {
                            const unsigned int textureId = northPicture->GetTextureId();
                            renderable3DScene.AddNorthWall(x, y, textureId);
                        }
                    }
                    const uint16_t southwallIndex = GetWallTile(x, y);
                    const uint16_t southWall = GetDarkWallPictureIndex(southwallIndex, ticks);
                    if (southWall != 1)
                    {
                        const Picture* southPicture = egaGraph.GetPicture(southWall);
                        if (southPicture != nullptr)
                        {
                            const unsigned int textureId = southPicture->GetTextureId();
                            renderable3DScene.AddSouthWall(x, y, textureId);
                        }
                    }
                }
            }

            if (m_wallXVisible[(y * m_levelWidth) + x])
            {
                if (IsPointVisible((float)x, (float)y, x1, y1, x2, y2) ||
                    IsPointVisible((float)x, (float)y + 1, x1, y1, x2, y2))
                {
                    const uint16_t eastwallIndex = GetWallTile(x, y);
                    const uint16_t eastWall = GetLightWallPictureIndex(eastwallIndex, ticks);
                    if (eastWall != 1)
                    {
                        const Picture* eastPicture = egaGraph.GetPicture(eastWall);
                        if (eastPicture != nullptr)
                        {
                            const unsigned int textureId = eastPicture->GetTextureId();
                            renderable3DScene.AddEastWall(x, y, textureId);
                        }
                    }
                    const uint16_t westwallIndex = GetWallTile(x - 1, y);
                    const uint16_t westWall = GetLightWallPictureIndex(westwallIndex, ticks);
                    if (westWall != 1)
                    {
                        const Picture* westPicture = egaGraph.GetPicture(westWall);
                        if (westPicture != nullptr)
                        {
                            const unsigned int textureId = westPicture->GetTextureId();
                            renderable3DScene.AddWestWall(x, y, textureId);
                        }
                    }
                }
            }
        }
    }

    RenderableSprites& renderableSprites = renderable3DScene.GetSpritesMutable();
    renderableSprites.Reset(m_playerActor->GetX(), m_playerActor->GetY(), m_playerActor->GetAngle());
    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            // Actors
            Actor* actor = GetBlockingActor(x, y);
            if (actor != nullptr)
            {
                Picture* actorPicture = egaGraph.GetPicture(actor->GetPictureIndex());
                if (actorPicture != nullptr)
                {
                    RenderableSprites::SpriteOrientation orientation = RenderableSprites::SpriteOrientation::RotatedTowardsPlayer;

                    if (actor->GetState() == StateIdArch)
                    {
                        int16_t storedOrientation = actor->GetTemp1();
                        if (storedOrientation == 0)
                        {
                            actor->SetTemp1(IsSolidWall(x - 1, y) && !IsExplosiveWall(x - 1, y) && !IsDoor(x - 1, y) ||
                                IsSolidWall(x + 1, y) && !IsExplosiveWall(x + 1, y) && !IsDoor(x + 1, y) ||
                                (GetBlockingActor(x - 1, y) != nullptr && GetBlockingActor(x - 1, y)->GetState() == StateIdArch) ||
                                (GetBlockingActor(x + 1, y) != nullptr && GetBlockingActor(x + 1, y)->GetState() == StateIdArch)
                                ? RenderableSprites::SpriteOrientation::AlongXAxis : RenderableSprites::SpriteOrientation::AlongYAxis);
                            storedOrientation = actor->GetTemp1();
                        }
                        orientation = (RenderableSprites::SpriteOrientation)storedOrientation;
                    }

                    const float actorSize = actor->GetDecorateActor().size;
                    const float actorX = actor->GetX();
                    const float actorY = actor->GetY();
                    if (IsActorVisibleForPlayer(actor) &&
                        (IsPointVisible(actorX - actorSize, actorY - actorSize, x1, y1, x2, y2) ||
                         IsPointVisible(actorX - actorSize, actorY + actorSize, x1, y1, x2, y2) ||
                         IsPointVisible(actorX + actorSize, actorY - actorSize, x1, y1, x2, y2) ||
                         IsPointVisible(actorX + actorSize, actorY + actorSize, x1, y1, x2, y2)))
                    {
                        renderableSprites.AddSprite(actorPicture, actor->GetX(), actor->GetY(), orientation);
                    }
                }
            }
        }
    }

    for (uint16_t i = 0; i < m_maxNonBlockingActors; i++)
    {
        // Projectiles
        Actor* projectile = GetNonBlockingActor(i);
        if (projectile != nullptr)
        {
            Picture* actorPicture = egaGraph.GetPicture(projectile->GetPictureIndex());
            if (actorPicture != nullptr)
            {
                if (IsActorVisibleForPlayer(projectile))
                {
                    renderableSprites.AddSprite(actorPicture, projectile->GetX(), projectile->GetY(), RenderableSprites::SpriteOrientation::RotatedTowardsPlayer);
                }
            }
        }
    }
}

void Level::DrawAutoMap(
    IRenderer& renderer,
    EgaGraph& egaGraph,
    const uint16_t additionalMargin,
    const uint16_t originX,
    const uint16_t originY,
    const AutoMapType autoMapType,
    const bool cheat)
{
    const int16_t tileWidth = 16;
    const int16_t additionalTilesInMargin = (additionalMargin == 0) ? 0 : (additionalMargin / tileWidth) + 1;
    const int16_t firstTileX = -additionalTilesInMargin + originX;
    const int16_t lastTileX = (320 / 16) + additionalTilesInMargin + originX;
    const int16_t firstTileY = originY;
    const int16_t lastTileY = originY + 9;
    RenderableTiles tiles(*egaGraph.GetTilesSize16());
    RenderableTiles numbers(*egaGraph.GetTilesSize8());
    for (int16_t y = firstTileY; y < lastTileY; y++)
    {
        for (int16_t x = firstTileX; x < lastTileX; x++)
        {
            const int16_t sx = (x - (int16_t)originX) * tileWidth;
            const int16_t sy = (y - (int16_t)originY) * tileWidth;
            if (x >= 0 && x < m_levelWidth && y >= 0 && y < m_levelHeight &&
                (cheat || IsTileClearFromFogOfWar(x, y)))
            {
                switch (autoMapType)
                {
                case MapView:
                {
                    const uint16_t wallTile = GetWallTile(x, y);
                    const uint16_t wallIndex = (wallTile < egaGraph.GetNumberOfTilesSize16(false)) ? wallTile : 0;
                    tiles.Add(sx, sy, wallIndex);
                    break;
                }
                case TileMapView:
                {
                    const uint16_t wallTile = GetWallTile(x, y);
                    // Only display wall tiles; skip floor tiles.
                    const uint16_t wallIndex = (wallTile < m_wallsInfo.size()) ? wallTile : 0;
                    tiles.Add(sx, sy, wallIndex);
                    break;
                }
                case ActorAtView:
                {
                    const uint16_t wallIndex = GetWallTile(x, y);
                    if (wallIndex < m_wallsInfo.size())
                    {
                        // This is a wall tile
                        tiles.Add(sx, sy, wallIndex);
                    }
                    else
                    {
                        // No wall; check for presence of actor
                        uint16_t tileIndex = 0;
                        const Actor* blockingActor = GetBlockingActor(x, y);
                        if (blockingActor != nullptr)
                        {
                            // In the original Catacomb 3-D, an actor is displayed here as 4 hexadecimal numbers,
                            // representing the first 2 bytes of its objtype struct, see C3_DEF.H.
                            // Since that struct does not exist in CatacombGL, the floor tile index of the
                            // actor is displayed instead.
                            const uint16_t tileIndex = blockingActor->GetDecorateActor().spawnOnAllDifficulties;
                            numbers.Add(sx, sy, RenderableTiles::TileIdFirstNumber + ((tileIndex & 0xf000) >> 12));
                            numbers.Add(sx + 8, sy, RenderableTiles::TileIdFirstNumber + ((tileIndex & 0x0f00) >> 8));
                            numbers.Add(sx, sy + 8, RenderableTiles::TileIdFirstNumber + ((tileIndex & 0x00f0) >> 4));
                            numbers.Add(sx + 8, sy + 8, RenderableTiles::TileIdFirstNumber + (tileIndex & 0x000f));
                        }
                        else
                        {
                            // No actor present; draw a black tile
                            tiles.Add(sx, sy, 0);
                        }
                    }
                    break;
                }
                case VisView:
                {
                    const uint16_t tileIndex = (m_visibilityMap[(y * m_levelWidth) + x]) ? 1 : 0;
                    tiles.Add(sx, sy, tileIndex);
                    break;
                }
                }
            }
            else
            {
                // Outside the map boundaries; draw a black tile
                tiles.Add(sx, sy, 0);
            }
        }
    }

    renderer.RenderTiles(tiles);
    renderer.RenderTiles(numbers);
}

void Level::SetupAutoMapIso(
    RenderableAutoMapIso& renderableAutoMapIso,
    EgaGraph& egaGraph,
    const float aspectRatio,
    const float originX,
    const float originY,
    const bool cheat)
{
    renderableAutoMapIso.PrepareFrame(aspectRatio, originX, originY);
    renderableAutoMapIso.SetPlayerPosition(m_playerActor->GetX(), m_playerActor->GetY(), m_playerActor->GetAngle());

    Renderable3DTiles& renderable3DTiles = renderableAutoMapIso.GetFloorTilesMutable();

    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            if (!IsSolidWall(x, y) && (cheat || IsTileClearFromFogOfWar(x, y)))
            {
                // Add floor and ceiling tile
                renderable3DTiles.AddTile(Renderable3DTiles::tileCoordinate{ (int16_t)x, (int16_t)y });

                // Add walls
                const uint16_t northwallIndex = GetWallTile(x, y - 1);
                const uint16_t northWall = GetDarkWallPictureIndex(northwallIndex, 0);
                if (northWall != 1)
                {
                    const Picture* northPicture = egaGraph.GetPicture(northWall);
                    if (northPicture != nullptr)
                    {
                        renderableAutoMapIso.AddNorthWall(x, y, northPicture->GetTextureId());
                    }
                }

                const uint16_t westwallIndex = GetWallTile(x - 1, y);
                const uint16_t westWall = GetLightWallPictureIndex(westwallIndex, 0);
                if (westWall != 1)
                {
                    const Picture* westPicture = egaGraph.GetPicture(westWall);
                    if (westPicture != nullptr)
                    {
                        renderableAutoMapIso.AddWestWall(x, y, westPicture->GetTextureId());
                    }
                }
            }
        }
    }

    renderable3DTiles.SetOnlyFloor(true);
    renderable3DTiles.SetFloorColor(GetGroundColor());

    const egaColor wallCapMainColor = GetWallCapMainColor();
    for (uint16_t y = 0; y < m_levelHeight; y++)
    {
        for (uint16_t x = 0; x < m_levelWidth; x++)
        {
            const bool isTileClearFromFogOfWar = IsTileClearFromFogOfWar(x, y);
            if (IsSolidWall(x, y) && (cheat || isTileClearFromFogOfWar))
            {
                const egaColor centerColor = GetWallCapCenterColor(x, y, cheat);
                renderableAutoMapIso.AddWallCap(x, y, wallCapMainColor, centerColor);
            }
            else if (!isTileClearFromFogOfWar && !cheat)
            {
                renderableAutoMapIso.AddWallCap(x, y, EgaBlack, EgaBlack);
            }
        }
    }

    // Add black borders
    for (uint16_t y = 0; y < m_levelHeight; y++)
    {
        renderableAutoMapIso.AddWallCap(m_levelWidth, y, EgaBlack, EgaBlack);
    }
    for (uint16_t x = 0; x < m_levelWidth; x++)
    {
        renderableAutoMapIso.AddWallCap(x, m_levelHeight, EgaBlack, EgaBlack);
    }

    RenderableSprites& renderableSprites = renderableAutoMapIso.GetSpritesMutable();

    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            // Actors
            Actor* actor = GetBlockingActor(x, y);
            if (actor != nullptr && (actor->IsActive() || cheat))
            {
                const Picture* actorPicture = egaGraph.GetPicture(actor->GetPictureIndex());
                if (actorPicture != nullptr)
                {
                    RenderableSprites::SpriteOrientation orientation = RenderableSprites::SpriteOrientation::Isometric;

                    if (actor->GetState() == StateIdArch)
                    {
                        int16_t storedOrientation = actor->GetTemp1();
                        if (storedOrientation == 0)
                        {
                            actor->SetTemp1(IsSolidWall(x - 1, y) && !IsExplosiveWall(x - 1, y) && !IsDoor(x - 1, y) ||
                                IsSolidWall(x + 1, y) && !IsExplosiveWall(x + 1, y) && !IsDoor(x + 1, y) ||
                                (GetBlockingActor(x - 1, y) != nullptr && GetBlockingActor(x - 1, y)->GetState() == StateIdArch) ||
                                (GetBlockingActor(x + 1, y) != nullptr && GetBlockingActor(x + 1, y)->GetState() == StateIdArch)
                                ? RenderableSprites::SpriteOrientation::AlongXAxis : RenderableSprites::SpriteOrientation::AlongYAxis);
                            storedOrientation = actor->GetTemp1();
                        }
                        orientation = (RenderableSprites::SpriteOrientation)storedOrientation;
                    }

                    renderableSprites.AddSprite(actorPicture, actor->GetX(), actor->GetY(), orientation);
                }
            }
        }
    }

    if (cheat)
    {
        for (uint16_t i = 0; i < m_maxNonBlockingActors; i++)
        {
            // Projectiles
            const Actor* projectile = GetNonBlockingActor(i);
            if (projectile != nullptr)
            {
                const Picture* actorPicture = egaGraph.GetPicture(projectile->GetPictureIndex());
                if (actorPicture != nullptr)
                {
                    renderableSprites.AddSprite(actorPicture, projectile->GetX(), projectile->GetY(), RenderableSprites::SpriteOrientation::Isometric);
                }
            }
        }
    }

    RenderableText& locationNames = renderableAutoMapIso.GetTextMutable();
    for (std::pair<uint8_t, locationNameBestPos> pair : m_locationNameBestPositions)
    {
        const std::string& locationMessage = egaGraph.GetWorldLocationNames(GetLevelIndex())->GetLocationName(pair.first);
        if ((cheat || IsTileClearFromFogOfWar(pair.second.x, pair.second.y)) &&
            // Filter out some garbage location messages from Catacomb 3D that should not be
            // shown on the automap.
            locationMessage != "I" &&
            locationMessage != "Description I" &&
            locationMessage != "Description L" &&
            locationMessage != "Description N" &&
            locationMessage != "Description P" &&
            locationMessage != "Description Q")
        {
            const int16_t x = ((pair.second.x + 2) * 32) + 16;
            const int16_t y = ((pair.second.y + 2) * 32);
            const uint16_t availableSpaceInPixels = pair.second.horizontalSpaceInTiles * 32;

            const std::string& locationMessage = egaGraph.GetWorldLocationNames(GetLevelIndex())->GetLocationName(pair.first);

            std::vector<std::string> subStrings;
            if (locationNames.GetWidthInPixels(locationMessage) <= availableSpaceInPixels)
            {
                subStrings.push_back(locationMessage);
            }
            else
            {
                // The text does not fit on a single line; try to split in two
                locationNames.SplitTextInTwo(locationMessage, subStrings);
                if (subStrings.size() == 2 &&
                    (locationNames.GetWidthInPixels(subStrings.at(0)) > availableSpaceInPixels ||
                        locationNames.GetWidthInPixels(subStrings.at(1)) > availableSpaceInPixels))
                {
                    // Even when split in two it does not split; try to split in three
                    locationNames.SplitTextInThree(locationMessage, subStrings);
                }
            }

            const egaColor textColor = (GetGroundColor() == EgaBrightWhite) ? EgaBlack : EgaBrightWhite;

            if (subStrings.size() == 1)
            {
                locationNames.Centered(subStrings.at(0), textColor, x, y + 11);
            }
            else if (subStrings.size() == 2)
            {
                locationNames.Centered(subStrings.at(0), textColor, x, y + 6);
                locationNames.Centered(subStrings.at(1), textColor, x, y + 16);
            }
            else
            {
                locationNames.Centered(subStrings.at(0), textColor, x, y + 1);
                locationNames.Centered(subStrings.at(1), textColor, x, y + 11);
                locationNames.Centered(subStrings.at(2), textColor, x, y + 21);
            }
        }
    }

    renderableAutoMapIso.FinalizeFrame();
}

uint16_t Level::GetTileIdFromActor(const Actor* actor)
{
    const uint16_t tileId0 = actor->GetDecorateActor().spawnOnAllDifficulties;
    const uint16_t tileId1 = actor->GetDecorateActor().spawnOnNormalAndHard;
    const uint16_t tileId2 = actor->GetDecorateActor().spawnOnHard;
    const uint16_t firstTileId =
        (tileId0 > 0 && (tileId0 < tileId1 || tileId1 == 0) && (tileId0 < tileId2 || tileId2 == 0)) ? tileId0 :
        (tileId1 > 0 && (tileId1 < tileId0 || tileId0 == 0) && (tileId1 < tileId2 || tileId2 == 0)) ? tileId1 :
        tileId2;

    return (firstTileId > 0) ? firstTileId - 1 : 0;
}

void Level::SetupAutoMapTopDown(
    RenderableAutoMapTopDown& renderableAutoMapTopDown,
    EgaGraph& egaGraph,
    const float aspectRatio,
    const uint16_t tileSize,
    const uint16_t additionalMargin,
    const float originX,
    const float originY,
    const bool cheat)
{
    // Scale factor is compared to the original 320 x 200 resolution
    const uint16_t wallsScaleFactor = tileSize / 16;
    const uint16_t additionalMarginScaled = 2 * additionalMargin * wallsScaleFactor;

    renderableAutoMapTopDown.PrepareFrame(aspectRatio, tileSize, originX, originY);
    renderableAutoMapTopDown.SetPlayerPosition(m_playerActor->GetX(), m_playerActor->GetY(), m_playerActor->GetAngle());

    Renderable3DTiles& floorTiles = renderableAutoMapTopDown.GetFloorTilesMutable();
    Renderable3DTiles& borderTiles = renderableAutoMapTopDown.GetBorderTilesMutable();
    RenderableTiles& tilesSize16 = renderableAutoMapTopDown.GetTilesSize16Mutable();
    RenderableTiles& tilesSize16Masked = renderableAutoMapTopDown.GetTilesSize16MaskedMutable();
    const int16_t tileWidth = tileSize;
    const int16_t additionalTilesInMargin = (additionalMarginScaled == 0) ? 0 : (additionalMarginScaled / tileWidth) + 1;
    const int16_t firstTileX = (int16_t)originX;
    const int16_t lastTileX = (int16_t)(ceil(renderableAutoMapTopDown.GetOriginal3DViewArea().width / 16) + 1 + additionalTilesInMargin + ceil(originX));
    const int16_t firstTileY = (int16_t)originY;
    const int16_t lastTileY = (int16_t)ceil(originY + renderableAutoMapTopDown.GetOriginal3DViewArea().bottom / 16) + 1;

    const egaColor wallCapMainColor = GetWallCapMainColor();
    const uint16_t numberOfTilesSize16 = egaGraph.GetNumberOfTilesSize16(false);
    for (int16_t y = firstTileY; y < lastTileY; y++)
    {
        for (int16_t x = firstTileX; x < lastTileX; x++)
        {
            const int16_t sx = (int16_t)((x - originX) * tileWidth);
            const int16_t sy = (int16_t)((y - originY) * tileWidth);

            if (x >= 0 && x < m_levelWidth && y >= 0 && y < m_levelHeight &&
                (cheat || IsTileClearFromFogOfWar(x, y)))
            {
                const uint16_t wallIndex = GetWallTile(x, y);
                const uint16_t darkWall = GetDarkWallPictureIndex(wallIndex, 0);
                if (darkWall != 1)
                {
                    const Picture* darkPicture = egaGraph.GetPicture(darkWall);
                    if (darkPicture != nullptr)
                    {
                        if (tileSize == 64)
                        {
                            renderableAutoMapTopDown.AddPicture(darkPicture, { sx, sy });
                        }
                        else
                        {
                            const uint16_t tileIndex = (wallIndex < numberOfTilesSize16) ? wallIndex : 0;
                            tilesSize16.Add(sx, sy, tileIndex);
                        }
                        const egaColor centerColor = GetWallCapCenterColor(x, y, cheat);
                        if (centerColor != wallCapMainColor)
                        {
                            const int16_t border = tileSize / 4;
                            renderableAutoMapTopDown.AddWallCap(centerColor, { sx + border, sy + border });
                        }
                    }
                    else
                    {
                        Renderable3DTiles::tileCoordinate tile = { sx, sy };
                        floorTiles.AddTile(tile);
                    }
                }
                else
                {
                    Renderable3DTiles::tileCoordinate tile = { sx, sy };
                    floorTiles.AddTile(tile);
                }
            }
            else
            {
                Renderable3DTiles::tileCoordinate tile = { sx, sy };
                borderTiles.AddTile(tile);
            }
        }
    }
    floorTiles.SetFloorColor(GetGroundColor());
    borderTiles.SetFloorColor(EgaBlack);
    const float actorMinX = (float)(firstTileX - 1);
    const float actorMaxX = (float(lastTileX + 1));
    const float actorMinY = (float)(firstTileY - 1);
    const float actorMaxY = (float(lastTileY + 1));
    const uint16_t numberOfTilesSize16Masked = egaGraph.GetNumberOfTilesSize16(true);
    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            // Actors
            const Actor* actor = GetBlockingActor(x, y);
            if (actor != nullptr && (actor->IsActive() || cheat))
            {
                if (actor->GetX() > actorMinX &&
                    actor->GetX() < actorMaxX &&
                    actor->GetY() > actorMinY &&
                    actor->GetY() < actorMaxY)
                {
                    if (tileSize == 64)
                    {
                        const Picture* actorPicture = egaGraph.GetPicture(actor->GetPictureIndex());
                        if (actorPicture != nullptr)
                        {
                            const int16_t marginX = (tileWidth - actorPicture->GetImageWidth()) / 2;
                            const int16_t sx = (int16_t)((x - originX) * tileWidth) + marginX;
                            const int16_t sy = (int16_t)((y - originY) * tileWidth);
                            renderableAutoMapTopDown.AddPicture(actorPicture, { sx, sy });
                        }
                    }
                    else
                    {
                        const uint16_t tileId = GetTileIdFromActor(actor);
                        if (tileId > 0 && tileId < numberOfTilesSize16Masked)
                        {
                            const int16_t sx = (int16_t)((x - originX) * tileWidth);
                            const int16_t sy = (int16_t)((y - originY) * tileWidth);
                            tilesSize16Masked.Add(sx, sy, tileId);
                        }
                    }
                }
            }
        }
    }

    const int16_t halfTileWidth = tileWidth / 2;
    if (cheat)
    {
        for (uint16_t i = 0; i < m_maxNonBlockingActors; i++)
        {
            // Projectiles
            const Actor* projectile = GetNonBlockingActor(i);
            if (projectile != nullptr)
            {
                if (projectile->GetX() > actorMinX &&
                    projectile->GetX() < actorMaxX &&
                    projectile->GetY() > actorMinY &&
                    projectile->GetY() < actorMaxY)
                {
                    if (tileSize == 64)
                    {
                        const Picture* actorPicture = egaGraph.GetPicture(projectile->GetPictureIndex());
                        if (actorPicture != nullptr)
                        {
                            const int16_t marginX = (tileWidth - actorPicture->GetImageWidth()) / 2;
                            const int16_t sx = (int16_t)((projectile->GetX() - originX) * tileWidth + marginX) - halfTileWidth;
                            const int16_t sy = (int16_t)((projectile->GetY() - originY) * tileWidth) - halfTileWidth;
                            renderableAutoMapTopDown.AddPicture(actorPicture, { sx, sy });
                        }
                    }
                    else
                    {
                        const uint16_t tileId = GetTileIdFromActor(projectile);
                        if (tileId > 0 && tileId < numberOfTilesSize16Masked)
                        {
                            const int16_t sx = (int16_t)((projectile->GetX() - originX) * tileWidth) - halfTileWidth;
                            const int16_t sy = (int16_t)((projectile->GetY() - originY) * tileWidth) - halfTileWidth;
                            tilesSize16Masked.Add(sx, sy, tileId);
                        }
                    }
                }
            }
        }
    }

    if (tileSize == 16)
    {
        // Add additional tile for player
        const float playerAngle = m_playerActor->GetAngle();
        const uint16_t tileId =
            (playerAngle > 315 || playerAngle < 45) ? 0 :
            (playerAngle < 135) ? 1 :
            (playerAngle < 225) ? 2 :
            3;
        const int16_t tileX = ((int16_t)((m_playerActor->GetX() - originX) * tileWidth) - halfTileWidth);
        const int16_t tileY = ((int16_t)((m_playerActor->GetY() - originY) * tileWidth) - halfTileWidth);
        tilesSize16Masked.Add(tileX, tileY, tileId);
    }

    RenderableText& locationNames = renderableAutoMapTopDown.GetTextMutable();

    for (std::pair<uint8_t, locationNameBestPos> pair : m_locationNameBestPositions)
    {
        const std::string& locationMessage = egaGraph.GetWorldLocationNames(GetLevelIndex())->GetLocationName(pair.first);
        if ((cheat || IsTileClearFromFogOfWar(pair.second.x, pair.second.y)) &&
            // Filter out some garbage location messages from Catacomb 3D that should not be
            // shown on the automap.
            locationMessage != "I" &&
            locationMessage != "Description I" &&
            locationMessage != "Description L" &&
            locationMessage != "Description N" &&
            locationMessage != "Description P" &&
            locationMessage != "Description Q")
        {
            if (tileSize == 64)
            {
                const int16_t textTileSize = 32;
                const int16_t x = (int16_t)((pair.second.x - originX) * textTileSize) + (textTileSize / 2);
                const int16_t y = (int16_t)((pair.second.y - originY) * textTileSize);
                const uint16_t availableSpaceInPixels = pair.second.horizontalSpaceInTiles * textTileSize;

                std::vector<std::string> subStrings;
                if (locationNames.GetWidthInPixels(locationMessage) <= availableSpaceInPixels)
                {
                    // The text fits on a single line
                    subStrings.push_back(locationMessage);
                }
                else
                {
                    // The text does not fit on a single line; try to split in two
                    locationNames.SplitTextInTwo(locationMessage, subStrings);
                    if (subStrings.size() == 2 &&
                        (locationNames.GetWidthInPixels(subStrings.at(0)) > availableSpaceInPixels ||
                            locationNames.GetWidthInPixels(subStrings.at(1)) > availableSpaceInPixels))
                    {
                        // Even when split in two it does not split; try to split in three
                        locationNames.SplitTextInThree(locationMessage, subStrings);
                    }
                }

                const egaColor textColor = (GetGroundColor() == EgaBrightWhite) ? EgaBlack : EgaBrightWhite;

                if (subStrings.size() == 1)
                {
                    locationNames.Centered(subStrings.at(0), textColor, x, y + 11);
                }
                else if (subStrings.size() == 2)
                {
                    locationNames.Centered(subStrings.at(0), textColor, x, y + 6);
                    locationNames.Centered(subStrings.at(1), textColor, x, y + 16);
                }
                else
                {
                    locationNames.Centered(subStrings.at(0), textColor, x, y + 1);
                    locationNames.Centered(subStrings.at(1), textColor, x, y + 11);
                    locationNames.Centered(subStrings.at(2), textColor, x, y + 21);
                }
            }
            else
            {
                // tileSize == 16
                const int16_t textTileSize = 16;
                const int16_t x = (int16_t)((pair.second.x - originX) * textTileSize) + (textTileSize / 2);
                const int16_t y = (int16_t)((pair.second.y - originY) * textTileSize);
                const uint16_t availableSpaceInPixels = pair.second.horizontalSpaceInTiles * textTileSize;

                const std::string locationMessage = RemoveTrailingSpaces(egaGraph.GetWorldLocationNames(GetLevelIndex())->GetLocationName(pair.first));
                std::vector<std::string> subStrings;
                if (locationNames.GetWidthInPixels(locationMessage) <= availableSpaceInPixels)
                {
                    // The text fits on a single line
                    subStrings.push_back(locationMessage);
                }
                else
                {
                    // The text does not fit on a single line; try to split in two
                    locationNames.SplitTextInTwo(locationMessage, subStrings);
                    if ((subStrings.size() == 2) && (pair.second.verticalSpaceInTiles > 2))
                    {
                        if ((locationNames.GetWidthInPixels(subStrings.at(0)) > availableSpaceInPixels) ||
                            (locationNames.GetWidthInPixels(subStrings.at(1)) > CalculateHorizontalSpaceInTiles(pair.second.x, pair.second.y + 1)* textTileSize))
                        {
                            // Does not fit
                            subStrings.clear();
                        }
                    }
                    else
                    {
                            // Does not fit
                            subStrings.clear();
                    }
                }

                const egaColor textColor = (GetGroundColor() == EgaBrightWhite) ? EgaBlack : EgaBrightWhite;

                if (subStrings.size() == 1)
                {
                    locationNames.Centered(subStrings.at(0), textColor, x, y + 3);
                }
                else if (subStrings.size() == 2)
                {
                    locationNames.Centered(subStrings.at(0), textColor, x, y + 4);
                    locationNames.Centered(subStrings.at(1), textColor, x, y + 18);
                }
            }
        }
    }
}

uint16_t Level::GetDarkWallPictureIndex(const uint16_t tileIndex, const uint32_t ticks) const
{
    if (tileIndex < m_wallsInfo.size())
    {
        const std::vector<uint16_t>& frames = m_wallsInfo.at(tileIndex).textureDark;
        const uint32_t numberOfFrames = (uint32_t)frames.size();
        if (numberOfFrames > 1)
        {
            const uint32_t frameDurationInTicks = 8;
            const uint32_t animDurationInTicks = frameDurationInTicks * numberOfFrames;
            const uint32_t currentFrame = (ticks % animDurationInTicks) / frameDurationInTicks;
            return frames[currentFrame];
        }
        else
        {
            return frames[0];
        }
    }
    else
    {
        return 1;
    }
}

uint16_t Level::GetLightWallPictureIndex(const uint16_t tileIndex, const uint32_t ticks) const
{
    if (tileIndex < m_wallsInfo.size())
    {
        const std::vector<uint16_t>& frames = m_wallsInfo.at(tileIndex).textureLight;
        const uint32_t numberOfFrames = (uint32_t)frames.size();
        if (numberOfFrames > 1)
        {
            const uint32_t frameDurationInTicks = 8;
            const uint32_t animDurationInTicks = frameDurationInTicks * numberOfFrames;
            const uint32_t currentFrame = (ticks % animDurationInTicks) / frameDurationInTicks;
            return frames[currentFrame];
        }
        else
        {
            return frames[0];
        }
    }
    else
    {
        return 1;
    }
}

void Level::RemoveActor(Actor* actor)
{
    for (uint16_t i = 0; i < m_maxNonBlockingActors; i++)
    {
        if (m_nonBlockingActors[i] == actor)
        {
            delete m_nonBlockingActors[i];
            m_nonBlockingActors[i] = nullptr;
        }
    }

    for (uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
    {
        if (m_blockingActors[i] == actor)
        {
            delete m_blockingActors[i];
            m_blockingActors[i] = nullptr;
        }
    }
}

egaColor Level::GetWallCapMainColor() const
{
    const egaColor groundColor = GetGroundColor();
    return (groundColor == EgaLightGray) ? EgaDarkGray : EgaLightGray;
}

egaColor Level::GetWallCapCenterColor(const uint16_t x, const uint16_t y, const bool cheat) const
{
    const egaColor wallCapMainColor = GetWallCapMainColor();
    const egaColor removableWallColor = (wallCapMainColor == EgaDarkGray) ? EgaLightGray : EgaDarkGray;
    const KeyId requiredKey = GetRequiredKeyForDoor(x, y);
    const bool isDoor = IsDoor(x, y);
    const egaColor centerColor =
        isDoor && requiredKey == RedKey ? EgaRed :
        isDoor && requiredKey == BlueKey ? EgaBlue :
        isDoor && requiredKey == GreenKey ? EgaGreen :
        isDoor && requiredKey == YellowKey ? EgaBrightYellow :
        IsRemovableDoor(x, y) || (cheat && (IsExplosiveWall(x, y) || IsFakeWall(x, y))) ? removableWallColor :
        IsVictoryDoor(x, y) ? EgaBrightCyan :
        wallCapMainColor;

    return centerColor;
}

uint16_t Level::CalculateHorizontalSpaceInTiles(const uint16_t x, const uint16_t y) const
{
    const uint16_t floorTile = GetWallTile(x, y);
    uint16_t tilesToTheLeft = 0;
    while (x - tilesToTheLeft > 0 && GetWallTile(x - tilesToTheLeft - 1, y) == floorTile)
    {
        tilesToTheLeft++;
    }
    uint16_t tilesToTheRight = 0;
    while (x + tilesToTheRight + 1 < GetLevelWidth() && GetWallTile(x + tilesToTheRight + 1, y) == floorTile)
    {
        tilesToTheRight++;
    }

    return (tilesToTheLeft > tilesToTheRight) ? (tilesToTheRight * 2) + 1 : (tilesToTheLeft * 2) + 1;
}

uint16_t Level::CalculateVerticalSpaceInTiles(const uint16_t x, const uint16_t y) const
{
    const uint16_t floorTile = GetWallTile(x, y);
    uint16_t tilesAbove = 0;
    while (y - tilesAbove > 0 && GetWallTile(x, y - tilesAbove - 1) == floorTile)
    {
        tilesAbove++;
    }
    uint16_t tilesBelow = 0;
    while (y + tilesBelow + 1 < GetLevelHeight() && GetWallTile(x, y + tilesBelow + 1) == floorTile)
    {
        tilesBelow++;
    }

    return (tilesAbove > tilesBelow) ? (tilesBelow * 2) + 1 : (tilesAbove * 2) + 1;
}

void Level::UpdateLocationNamesBestPositions()
{
    m_locationNameBestPositions.clear();

    for (uint16_t y = 0; y < GetLevelHeight(); y++)
    {
        for (uint16_t x = 0; x < GetLevelWidth(); x++)
        {
            const uint16_t wallTile = GetWallTile(x, y);
            if (wallTile > 180)
            {
                const uint8_t locationNameIndex = (uint8_t)(wallTile - 180);
                const uint16_t horizontalSpaceInTiles = CalculateHorizontalSpaceInTiles(x, y);
                const uint16_t verticalSpaceInTiles = CalculateVerticalSpaceInTiles(x, y);
                const locationNameBestPos currentPos = { horizontalSpaceInTiles, verticalSpaceInTiles, x, y };
                if (m_locationNameBestPositions.find(locationNameIndex) == m_locationNameBestPositions.end())
                {
                    m_locationNameBestPositions.insert(std::make_pair(locationNameIndex, currentPos));
                }
                else
                {
                    const locationNameBestPos& previousBestPos = m_locationNameBestPositions.at(locationNameIndex);
                    if (currentPos.horizontalSpaceInTiles > previousBestPos.horizontalSpaceInTiles ||
                        ((currentPos.horizontalSpaceInTiles == previousBestPos.horizontalSpaceInTiles) &&
                        (currentPos.verticalSpaceInTiles > previousBestPos.verticalSpaceInTiles)))
                    {
                        m_locationNameBestPositions.at(locationNameIndex) = currentPos;
                    }
                }
            }
        }
    }
}

const std::string Level::RemoveTrailingSpaces(const std::string& str) const
{
    const char spaceChar = ' ';
    const size_t lastCharIndex = str.find_last_not_of(spaceChar);
    std::string result = str.substr(0, lastCharIndex + 1);
    return result;
}

bool Level::IsPointVisible(
    const float pointX, const float pointY,
    const float x1, const float y1,
    const float x2, const float y2)
{
    const float xv1 = x2 - x1;
    const float yv1 = y2 - y1;
    const float xv2 = x2 - pointX;
    const float yv2 = y2 - pointY;
    const float xp = xv1 * yv2 - yv1 * xv2; // Cross product

    return xp > 0;
}