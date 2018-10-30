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
#include "..\Abyss\DecorateMisc.h"
#include "..\Abyss\DecorateBonus.h"

Level::Level(const uint8_t mapIndex, const uint16_t mapWidth, const uint16_t mapHeight, const uint16_t* plane0, const uint16_t* plane2, const LevelInfo& mapInfo, const std::vector<WallInfo>& wallsInfo):
    m_levelWidth (mapWidth),
    m_levelHeight (mapHeight),
    m_levelInfo (mapInfo),
    m_wallsInfo (wallsInfo),
    m_lightningStartTimestamp(0),
    m_levelIndex(mapIndex),
    m_visibilityMap(NULL),
    m_playerActor(new Actor(0, 0, 0, decoratePlayer)),
    m_blockingActors(NULL),
    m_nonBlockingActors(NULL),
    m_wallXVisible(NULL),
    m_wallYVisible(NULL)
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

    m_blockingActors = new Actor*[mapSize];
    for ( uint16_t i = 0; i < mapSize; i++)
    {
        m_blockingActors[i] = NULL;
    }

    m_nonBlockingActors = new Actor*[100];
    for ( uint16_t i = 0; i < 100; i++)
    {
        m_nonBlockingActors[i] = NULL;
    }

    m_wallXVisible = new bool[m_levelWidth * m_levelHeight];
    m_wallYVisible = new bool[m_levelWidth * m_levelHeight];
}

bool Level::LoadActorsFromFile(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors)
{
    m_playerActor = new Actor(file, decorateActors);
    uint16_t numberOfBlockingActors = 0;
    file.read((char*)&numberOfBlockingActors, sizeof(numberOfBlockingActors));
    for (uint16_t i = 0; i < numberOfBlockingActors; i++)
    {
        Actor* blockingActor = new Actor(file, decorateActors);
        SetBlockingActor(blockingActor->GetTileX(), blockingActor->GetTileY(), blockingActor);
    }

    uint16_t numberOfNonBlockingActors = 0;
    file.read((char*)&numberOfNonBlockingActors, sizeof(numberOfNonBlockingActors));
    for (uint16_t i = 0; i < numberOfNonBlockingActors; i++)
    {
        Actor* nonBlockingActor = new Actor(file, decorateActors);
        AddNonBlockingActor(nonBlockingActor);
    }

    return true;
}

Level::~Level()
{
    delete m_plane0;
    delete m_plane2;

    delete m_visibilityMap;
    m_visibilityMap = NULL;

    delete m_wallXVisible;
    delete m_wallYVisible;

    delete m_playerActor;

    if (m_blockingActors != NULL)
    {
        for ( uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
        {
            if (m_blockingActors[i] != NULL)
            {
                delete m_blockingActors[i];
                m_blockingActors[i] = NULL;
            }
        }

        delete[] m_blockingActors;
        m_blockingActors = NULL;
    }

    if (m_nonBlockingActors != NULL)
    {
        for ( uint16_t i = 0; i < 100; i++)
        {
            if (m_nonBlockingActors[i] != NULL)
            {
                delete m_nonBlockingActors[i];
                m_nonBlockingActors[i] = NULL;
            }
        }
        delete[] m_nonBlockingActors;
        m_nonBlockingActors = NULL;
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
        if (m_blockingActors[i] != NULL)
        {
            numberOfBlockingActors++;
        }
    }
    file.write((const char*)&numberOfBlockingActors, sizeof(numberOfBlockingActors));
    for (uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
    {
        if (m_blockingActors[i] != NULL)
        {
            m_blockingActors[i]->StoreToFile(file);
        }
    }
    uint16_t numberOfNonBlockingActors = 0;
    for (uint16_t i = 0; i < 100; i++)
    {
        if (m_nonBlockingActors[i] != NULL)
        {
            numberOfNonBlockingActors++;
        }
    }
    file.write((const char*)&numberOfNonBlockingActors, sizeof(numberOfNonBlockingActors));
    for (uint16_t i = 0; i < 100; i++)
    {
        if (m_nonBlockingActors[i] != NULL)
        {
            m_nonBlockingActors[i]->StoreToFile(file);
        }
    }
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
    return m_plane0[(y * m_levelWidth) + x];
}

uint16_t Level::GetFloorTile(const uint16_t x, const uint16_t y) const
{
    return m_plane2[(y * m_levelWidth) + x];
}

void Level::SetWallTile(const uint16_t x, const uint16_t y, const uint16_t wallTile)
{
    m_plane0[(y * m_levelWidth) + x] = wallTile;
}

void Level::SetFloorTile(const uint16_t x, const uint16_t y, const uint16_t floorTile)
{
    m_plane2[(y * m_levelWidth) + x] = floorTile;
}

bool Level::IsSolidWall(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    return ((wallTile < m_wallsInfo.size()) && (m_wallsInfo.at(wallTile).wallType != WTOpen));
}

bool Level::IsExplosiveWall(const uint16_t x, const uint16_t y) const
{
    const uint16_t spot = (GetFloorTile(x,y) >> 8);
    return (spot == 0xfc);
}

bool Level::IsDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t wallTile = GetWallTile(x, y);
    return ((wallTile < m_wallsInfo.size()) && (m_wallsInfo.at(wallTile).wallType == WTDoor || m_wallsInfo.at(wallTile).wallType == WTDoorRedKeyRequired));
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

KeyId Level::GetRequiredKeyForDoor(const uint16_t x, const uint16_t y) const
{
    const uint16_t tile = GetWallTile(x, y);
    if (m_wallsInfo.at(tile).wallType == WTDoorRedKeyRequired)
    {
        return RedKey;
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
            const uint32_t lightningFrameDuration = 17; //ms (60 Hz)
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
        done = (!(firstWallBackTraced.x == wallHit.x && firstWallBackTraced.y == wallHit.y && firstWallBackTraced.isXWall == wallHit.isXWall)) &&
                ((wallHit.isXWall && m_wallXVisible[(wallHit.y * m_levelWidth) + wallHit.x] == true) ||
                (!wallHit.isXWall && m_wallYVisible[(wallHit.y * m_levelWidth) + wallHit.x] == true));
        if (wallHit.isXWall)
        {
            m_wallXVisible[(wallHit.y * m_levelWidth) + wallHit.x] = true;
        }
        else
        {
            m_wallYVisible[(wallHit.y * m_levelWidth) + wallHit.x] = true;
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

    LevelCoordinate wallEdgeLeft = GetLeftEdgeOfWall(firstWallHit);
    LevelCoordinate intersection2 = GetIntersectionWithOuterWall(wallEdgeLeft);
    float distanceForBackTracing = GetDistanceOnOuterWall(intersection2) - 0.001f;
    BackTraceWalls(distanceForBackTracing, firstWallBackTraced);

    for (uint16_t x = 1; x < m_levelWidth - 1; x++)
    {
        for (uint16_t y = 1; y < m_levelHeight - 1; y++)
        {
            if (!IsSolidWall(x,y) &&
                (m_wallXVisible[(y * m_levelWidth) + x] ||
                m_wallXVisible[(y * m_levelWidth) + x + 1] ||
                m_wallYVisible[(y * m_levelWidth) + x] ||
                m_wallYVisible[((y + 1) * m_levelWidth) + x]))
            {
                m_visibilityMap[(y * m_levelWidth) + x] = true;
            }
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
        LevelCoordinate leftCoordinate = GetOuterWallCoordinate(distanceForBackTracing - additionalDistance);
        LevelWall wallHit;
        RayTraceWall(leftCoordinate, wallHit);
        if (firstWall.x == 0 && firstWall.y == 0)
        {
            firstWall = wallHit;
        }
        doneBackTracing = ((wallHit.isXWall && m_wallXVisible[(wallHit.y * m_levelWidth) + wallHit.x] == true) ||
            (!wallHit.isXWall && m_wallYVisible[(wallHit.y * m_levelWidth) + wallHit.x] == true));

        if (!doneBackTracing)
        {
            if (wallHit.isXWall)
            {
                m_wallXVisible[(wallHit.y * m_levelWidth) + wallHit.x] = true;
            }
            else
            {
                m_wallYVisible[(wallHit.y * m_levelWidth) + wallHit.x] = true;
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

    LevelCoordinate coordinate = { 0.0f, 0.0f };
    if (!wall.isXWall) // YWall
    {
        if (y > wall.y)
        {
            coordinate = { (float)wall.x + 1.0f, (float)wall.y };
        }
        else
        {
            coordinate = { (float)wall.x, (float)wall.y };
        }
    }
    else // XWall
    {
        if (x > wall.x)
        {
            coordinate = { (float)wall.x, (float)wall.y };
        }
        else
        {
            coordinate = { (float)wall.x, (float)wall.y + 1.0f };
        }
    }

    return coordinate;
}

LevelCoordinate Level::GetLeftEdgeOfWall(LevelWall& wall) const
{
    const float x = m_playerActor->GetX();
    const float y = m_playerActor->GetY();
    LevelCoordinate coordinate = { 0.0f, 0.0f };
    if (!wall.isXWall) // YWall
    {
        if (y < wall.y)
        {
            coordinate = { (float)wall.x + 1.0f, (float)wall.y };
        }
        else
        {
            coordinate = { (float)wall.x, (float)wall.y };
        }
    }
    else // XWall
    {
        if (x < wall.x)
        {
            coordinate = { (float)wall.x, (float)wall.y };
        }
        else
        {
            coordinate = { (float)wall.x, (float)wall.y + 1.0f };
        }
    }

    return coordinate;
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
    int32_t tileX, tileY;
    if (x > m_playerActor->GetX())
    {
        tileX = (int32_t)(m_playerActor->GetX() + 1.0f);
    }
    else
    {
        tileX = (int32_t)m_playerActor->GetX();
    }
    if (y > m_playerActor->GetY())
    {
        tileY = (int32_t)(m_playerActor->GetY() + 1.0f);
    }
    else
    {
        tileY = (int32_t)m_playerActor->GetY();
    }

    float squareDistanceX = 0.0f;
    float squareDistanceY = 0.0f;

    const float normalX = (abs(m_playerActor->GetY() - y) < 0.000001) ? 100000.0f : ((float)x - m_playerActor->GetX()) / ((float)y - m_playerActor->GetY());
    const float normalY = (abs(m_playerActor->GetX() - x) < 0.000001) ? 100000.0f : ((float)y - m_playerActor->GetY()) / ((float)x - m_playerActor->GetX());

    while (traceStateX == LookingForWall || traceStateY == LookingForWall)
    {
        if ((traceStateX == LookingForWall) &&
           ((traceStateY == LookingForWall && squareDistanceX <= squareDistanceY) ||
            (traceStateY == WallFound) ||
            (traceStateY == NoWallFound)))
        {
            if (x > m_playerActor->GetX())
            {
                if ((float)tileX < x)
                {
                    hitWallX_x = (uint16_t)tileX;
                    hitWallX_y = ((((float)tileX - m_playerActor->GetX()) * normalY) + m_playerActor->GetY());

                    const float deltaX_x = abs(m_playerActor->GetX() - (float)hitWallX_x);
                    const float deltaX_y = abs(m_playerActor->GetY() - hitWallX_y);
                    squareDistanceX = (deltaX_x * deltaX_x) + (deltaX_y * deltaX_y);

                    if (squareDistanceX > squareDistanceY && traceStateY == WallFound)
                    {
                        traceStateX = WallIsFurtherAway;
                    }
                    else if (IsSolidWall((uint16_t)tileX, (uint16_t)hitWallX_y))
                    {
                        traceStateX = WallFound;
                    }
                    else if (squareDistanceX > squareDistanceY && traceStateY == LookingForWall && hitWallY_y > 0)
                    {
                        m_wallYVisible[(hitWallY_y * m_levelWidth) + (uint16_t)hitWallY_x] = true;
                    }
                    else
                    {
                        m_wallXVisible[((uint16_t)hitWallX_y * m_levelWidth) + hitWallX_x] = true;
                    }
                    tileX++;
                }
                else
                {
                    traceStateX = NoWallFound;
                }
            }
            else
            {
                if ((float)tileX > x)
                {
                    hitWallX_x = (uint16_t)tileX;
                    hitWallX_y = (((m_playerActor->GetX() - (float)tileX) * -normalY) + m_playerActor->GetY());

                    const float deltaX_x = abs(m_playerActor->GetX() - (float)hitWallX_x);
                    const float deltaX_y = abs(m_playerActor->GetY() - hitWallX_y);
                    squareDistanceX = (deltaX_x * deltaX_x) + (deltaX_y * deltaX_y);

                    if (squareDistanceX >= squareDistanceY && traceStateY == WallFound)
                    {
                        traceStateX = WallIsFurtherAway;
                    }
                    else if (IsSolidWall((uint16_t)tileX - 1, (uint16_t)hitWallX_y))
                    {
                        traceStateX = WallFound;
                    }
                    else if (squareDistanceX > squareDistanceY && traceStateY == LookingForWall && hitWallY_y > 0)
                    {
                        m_wallYVisible[(hitWallY_y * m_levelWidth) + (uint16_t)hitWallY_x] = true;
                    }
                    else
                    {
                        m_wallXVisible[((uint16_t)hitWallX_y * m_levelWidth) + hitWallX_x] = true;
                    }
                    tileX--;
                }
                else
                {
                    traceStateX = NoWallFound;
                }
            }
        }
        else
        {
            if (y > m_playerActor->GetY())
            {
                if ((float)tileY < y)
                {
                    hitWallY_x = ((((float)tileY - m_playerActor->GetY()) * normalX) + m_playerActor->GetX());
                    hitWallY_y = (uint16_t)tileY;

                    const float deltaY_x = abs(m_playerActor->GetX() - hitWallY_x);
                    const float deltaY_y = abs(m_playerActor->GetY() - (float)hitWallY_y);
                    squareDistanceY = (deltaY_x * deltaY_x) + (deltaY_y * deltaY_y);

                    if (squareDistanceY >= squareDistanceX && traceStateX == WallFound)
                    {
                        traceStateY = WallIsFurtherAway;
                    }
                    else if (IsSolidWall((uint16_t)hitWallY_x, (uint16_t)tileY))
                    {
                        traceStateY = WallFound;
                    }
                    else if (squareDistanceY > squareDistanceX && traceStateX == LookingForWall && hitWallX_x > 0)
                    {
                        m_wallXVisible[((uint16_t)hitWallX_y * m_levelWidth) + hitWallX_x] = true;
                    }
                    else
                    {
                        m_wallYVisible[(hitWallY_y * m_levelWidth) + (uint16_t)hitWallY_x] = true;
                    }
                    tileY++;
                }
                else
                {
                    traceStateY = NoWallFound;
                }
            }
            else
            {
                if ((float)tileY > y)
                {
                    hitWallY_x = (((m_playerActor->GetY() - (float)tileY) * -normalX) + m_playerActor->GetX());
                    hitWallY_y = (uint16_t)tileY;

                    const float deltaY_x = abs(m_playerActor->GetX() - hitWallY_x);
                    const float deltaY_y = abs(m_playerActor->GetY() - (float)hitWallY_y);
                    squareDistanceY = (deltaY_x * deltaY_x) + (deltaY_y * deltaY_y);

                    if (squareDistanceY >= squareDistanceX && traceStateX == WallFound)
                    {
                        traceStateY = WallIsFurtherAway;
                    }
                    else if (IsSolidWall((uint16_t)hitWallY_x, (uint16_t)tileY - 1))
                    {
                        traceStateY = WallFound;
                    }
                    else if (squareDistanceY > squareDistanceX && traceStateX == LookingForWall && hitWallX_x > 0)
                    {
                        m_wallXVisible[((uint16_t)hitWallX_y * m_levelWidth) + hitWallX_x] = true;
                    }
                    else
                    {
                        m_wallYVisible[(hitWallY_y * m_levelWidth) + (uint16_t)hitWallY_x] = true;
                    }
                    tileY--;
                }
                else
                {
                    traceStateY = NoWallFound;
                }
            }
        }
    }

    if (traceStateX == WallFound && ((traceStateY == WallFound && squareDistanceX <= squareDistanceY) || traceStateY != WallFound))
    {
        wallHit.x = hitWallX_x;
        wallHit.y = (uint16_t)hitWallX_y;
        wallHit.isXWall = true;
    }
    else
    {
        wallHit.x = (uint16_t)hitWallY_x;
        wallHit.y = hitWallY_y;
        wallHit.isXWall = false;
    }
}

bool Level::IsTileVisibleForPlayer(const uint16_t x, const uint16_t y) const
{
    return ( m_visibilityMap[y * m_levelWidth + x] == true);
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

Actor** Level::GetBlockingActors()
{
    return m_blockingActors;
}

Actor** Level::GetNonBlockingActors()
{
    return m_nonBlockingActors;
}

void Level::SetBlockingActor(const uint16_t x, const uint16_t y, Actor* actor)
{
    m_blockingActors[(y * m_levelWidth) + x] = actor;
}

Actor* Level::GetBlockingActor(const uint16_t x, const uint16_t y) const
{
    return m_blockingActors[(y * m_levelWidth) + x];
}

Actor* Level::GetNonBlockingActor(const uint16_t index) const
{
    return m_nonBlockingActors[index];
}

void Level::AddNonBlockingActor(Actor* projectile)
{
    uint16_t i = 0;
    while (m_nonBlockingActors[i] != NULL && i < 100)
    {
        i++;
    }
    if (i < 100)
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
        if ((GetBlockingActor(tileX, tileY - 1) != NULL) || IsSolidWall(tileX, tileY - 1))
        {
            return false;
        }
        actor->SetTile(tileX, tileY - 1);
        return true;

    case northeast:
        if ((GetBlockingActor(tileX + 1, tileY - 1) != NULL) || IsSolidWall(tileX + 1, tileY - 1))
        {
            return false;
        }
        actor->SetTile(tileX + 1, tileY - 1);
        return true;

    case east:
        if ((GetBlockingActor(tileX + 1, tileY) != NULL) || IsSolidWall(tileX + 1, tileY))
        {
            return false;
        }
        actor->SetTile(tileX + 1, tileY);
        return true;

    case southeast:
        if ((GetBlockingActor(tileX + 1, tileY + 1) != NULL) || IsSolidWall(tileX + 1, tileY + 1))
        {
            return false;
        }
        actor->SetTile(tileX + 1, tileY + 1);
        return true;

    case south:
        if ((GetBlockingActor(tileX, tileY + 1) != NULL) || IsSolidWall(tileX, tileY + 1))
        {
            return false;
        }
        actor->SetTile(tileX, tileY + 1);
        return true;

    case southwest:
        if ((GetBlockingActor(tileX - 1, tileY + 1) != NULL) || IsSolidWall(tileX - 1, tileY + 1))
        {
            return false;
        }
        actor->SetTile(tileX - 1, tileY + 1);
        return true;

    case west:
        if ((GetBlockingActor(tileX - 1, tileY) != NULL) || IsSolidWall(tileX - 1, tileY))
        {
            return false;
        }
        actor->SetTile(tileX - 1, tileY);
        return true;

    case northwest:
        if ((GetBlockingActor(tileX - 1, tileY - 1) != NULL) || IsSolidWall(tileX - 1, tileY - 1))
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
    actorDirection dirtable[9] = {northwest,north,northeast,west,nodir,east,southwest,south,southeast};
    actorDirection opposite[9] = {south,west,north,east,southwest,northwest,northeast,southeast,nodir};

    actorDirection tdir = actorDirection::nodir;
    actorDirection d[3];

    actorDirection olddir=actor->GetDirection();
    actorDirection turnaround=opposite[olddir];

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

    int8_t deltax=(int8_t)targetX - actor->GetTileX();
    int8_t deltay=(int8_t)targetY - actor->GetTileY();

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
        for (int8_t i = (int8_t)north;tdir<=(int8_t)west;i++)
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
    actorDirection dirtable[9] = { northwest,north,northeast,west,nodir,east,southwest,south,southeast };

    actorDirection oldDir = actor->GetDirection();
    actorDirection tdir;
    actorDirection d[3];

    const float targetX = m_playerActor->GetX();
    const float targetY = m_playerActor->GetY();

    int8_t deltax = (int8_t)targetX - actor->GetTileX();
    int8_t deltay = (int8_t)targetY - actor->GetTileY();

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
        for (int8_t i = (int8_t)north; tdir <= (int8_t)west; i++)
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
    if (abs(dx) < 1.0)
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
    else if (abs(dy) < 1.0)
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

void Level::ExplodeWall(const uint16_t x, const uint16_t y, const uint32_t timestamp)
{
    if (m_blockingActors[(y * m_levelWidth) + x] == NULL)
    {
        m_blockingActors[(y * m_levelWidth) + x] = new Actor(x, y, timestamp, decorateExplodingWall);
        m_blockingActors[(y * m_levelWidth) + x]->SetActive(true);
    }
}

void Level::SpawnExplosion(const float x, const float y, const int16_t delay, const uint32_t timestamp)
{
    Actor* actor = new Actor(x, y, timestamp, decorateExplosion);
    actor->SetTemp2(delay);
    AddNonBlockingActor(actor);
}

void Level::SpawnBigExplosion(const float x, const float y, const uint16_t delay, const uint32_t range, const uint32_t timestamp)
{
    SpawnExplosion(x-((rand() % range) / 65536.0f),y+((rand() % range) / 65536.0f),rand() % delay, timestamp);
    SpawnExplosion(x+((rand() % range) / 65536.0f),y-((rand() % range) / 65536.0f),rand() % delay, timestamp);
    SpawnExplosion(x-((rand() % range) / 65536.0f),y-((rand() % range) / 65536.0f),rand() % delay, timestamp);
    SpawnExplosion(x+((rand() % range) / 65536.0f),y+((rand() % range) / 65536.0f),rand() % delay, timestamp);
}

bool Level::IsWaterLevel() const
{
    return GetGroundColor() == EgaBlue;
}

void Level::DrawVisibilityMap(IRenderer& renderer)
{
    renderer.PrepareVisibilityMap();

    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            const uint16_t wall = GetWallTile(x, y);
            if (IsTileVisibleForPlayer(x, y))
            {
                renderer.RenderFloor(x, y, EgaBrightWhite);
            }
        }
    }
    renderer.UnprepareVisibilityMap();
}

void Level::DrawFloorAndCeiling(IRenderer& renderer, const uint32_t timeStamp)
{
    renderer.PrepareFloorAndCeiling();
    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            if (IsTileVisibleForPlayer(x, y))
            {
                renderer.RenderFloor(x, y, GetGroundColor());
                renderer.RenderCeiling(x, y, GetSkyColor(timeStamp));
            }
        }
    }
    renderer.UnprepareFloorAndCeiling();
}

void Level::DrawWalls(IRenderer& renderer, EgaGraph* egaGraph, const uint32_t ticks)
{
    renderer.PrepareWalls();

    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            if (m_wallYVisible[(y * m_levelWidth) + x])
            {
                const uint16_t northwallIndex = GetWallTile(x, y - 1);
                const uint16_t northWall = GetDarkWallPictureIndex(northwallIndex, ticks);
                if (northWall != 1)
                {
                    Picture* northPicture = egaGraph->GetPicture(northWall);
                    renderer.Render3DWall(northPicture, x, y - 1, 180);
                }
            }

            if (m_wallXVisible[(y * m_levelWidth) + x + 1])
            {
                const uint16_t eastwallIndex = GetWallTile(x + 1, y);
                const uint16_t eastWall = GetLightWallPictureIndex(eastwallIndex, ticks);
                if (eastWall != 1)
                {
                    Picture* eastPicture = egaGraph->GetPicture(eastWall);
                    renderer.Render3DWall(eastPicture, x + 1, y, 270);
                }
            }


            if (m_wallYVisible[((y + 1) * m_levelWidth) + x])
            {
                const uint16_t southwallIndex = GetWallTile(x, y + 1);
                const uint16_t southWall = GetDarkWallPictureIndex(southwallIndex, ticks);
                if (southWall != 1)
                {
                    Picture* southPicture = egaGraph->GetPicture(southWall);
                    renderer.Render3DWall(southPicture, x, y + 1, 0);
                }
            }

            if (m_wallXVisible[(y * m_levelWidth) + x])
            {
                const uint16_t westwallIndex = GetWallTile(x - 1, y);
                const uint16_t westWall = GetLightWallPictureIndex(westwallIndex, ticks);
                if (westWall != 1)
                {
                    Picture* westPicture = egaGraph->GetPicture(westWall);
                    renderer.Render3DWall(westPicture, x - 1, y, 90);
                }
            }
        }
    }

    renderer.UnprepareWalls();
}

uint16_t Level::GetDarkWallPictureIndex(const uint16_t tileIndex, const uint32_t ticks) const
{
    if (tileIndex < m_wallsInfo.size())
    {
        if (m_wallsInfo.at(tileIndex).textureDark.size() > 1)
        {
            const uint32_t frameDurationInTicks = 8;
            const uint32_t animDurationInTicks = frameDurationInTicks * m_wallsInfo.at(tileIndex).textureDark.size();
            const uint32_t currentFrame = (ticks % animDurationInTicks) / frameDurationInTicks;
            return m_wallsInfo.at(tileIndex).textureDark[currentFrame];
        }
        else
        {
            return m_wallsInfo.at(tileIndex).textureDark[0];
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
        if (m_wallsInfo.at(tileIndex).textureLight.size() > 1)
        {
            const uint32_t frameDurationInTicks = 8;
            const uint32_t animDurationInTicks = frameDurationInTicks * m_wallsInfo.at(tileIndex).textureLight.size();
            const uint32_t currentFrame = (ticks % animDurationInTicks) / frameDurationInTicks;
            return m_wallsInfo.at(tileIndex).textureLight[currentFrame];
        }
        else
        {
            return m_wallsInfo.at(tileIndex).textureLight[0];
        }
    }
    else
    {
        return 1;
    }
}
void Level::DrawActors(IRenderer& renderer, EgaGraph* egaGraph)
{
    for (uint16_t y = 1; y < m_levelHeight - 1; y++)
    {
        for (uint16_t x = 1; x < m_levelWidth - 1; x++)
        {
            // Actors
            Actor* actor = GetBlockingActor(x, y);
            if (actor != NULL)
            {
                Picture* actorPicture = egaGraph->GetPicture(actor->GetPictureIndex());
                if (actorPicture != NULL)
                {
                    if (IsActorVisibleForPlayer(actor))
                    {
                        renderer.AddSprite(actorPicture, actor->GetX(), actor->GetY());
                    }
                }
            }
        }
    }

    for (uint16_t i = 0; i < 100; i++)
    {
        // Projectiles
        Actor* projectile = GetNonBlockingActor(i);
        if (projectile != NULL)
        {
            Picture* actorPicture = egaGraph->GetPicture(projectile->GetPictureIndex());
            if (actorPicture != NULL)
            {
                if (IsActorVisibleForPlayer(projectile))
                {
                    renderer.AddSprite(actorPicture, projectile->GetX(), projectile->GetY());
                }
            }
        }
    }

    renderer.RenderAllSprites();
}

void Level::RemoveActor(Actor* actor)
{
    for (uint16_t i = 0; i < 100; i++)
    {
        if (m_nonBlockingActors[i] == actor)
        {
            delete m_nonBlockingActors[i];
            m_nonBlockingActors[i] = NULL;
        }
    }

    for (uint16_t i = 0; i < m_levelWidth * m_levelHeight; i++)
    {
        if (m_blockingActors[i] == actor)
        {
            delete m_blockingActors[i];
            m_blockingActors[i] = NULL;
        }
    }
}