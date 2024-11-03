// Copyright (C) 2020 Arno Ansems
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

#include "Renderable3DScene.h"
#include <cmath>

Renderable3DScene::Renderable3DScene(const ViewPorts::ViewPortRect3D original3DViewArea) :
    m_original3DViewArea(original3DViewArea),
    m_aspectRatio(1.0f),
    m_cameraX(1.0f),
    m_cameraY(1.0f),
    m_angle(1.0f),
    m_depthShading(true),
    m_fieldOfView(25),
    m_originalScreenResolution(false),
    m_originalCameraView(false)
{

}

void Renderable3DScene::PrepareFrame(
    const float aspectRatio,
    const float playerX,
    const float playerY,
    const float angle,
    const bool depthShading,
    const uint16_t fieldOfView,
    const bool originalScreenResolution,
    const bool cameraBehindPlayer)
{
    m_aspectRatio = aspectRatio;
    const float angleInRadians = (angle + 180.0f) * 3.14159265f / 180.0f;
    // In the original DOS games, the camera position is slightly behind the player position, see the function
    // ThreeDRefresh in C3_DRAW.C. This behavior is optionally emulated here, depending on the flag cameraBehindPlayer.
    constexpr float distanceFromPlayerToCamera = 0.4f;
    m_cameraX = cameraBehindPlayer ? playerX + distanceFromPlayerToCamera * std::sin(angleInRadians) : playerX;
    m_cameraY = cameraBehindPlayer ? playerY - distanceFromPlayerToCamera * std::cos(angleInRadians) : playerY;
    m_angle = angle;
    m_depthShading = depthShading;
    m_fieldOfView = fieldOfView;
    m_walls.Reset();
    m_3DTiles.Reset();
    m_sprites.Reset(m_cameraX, m_cameraY, angle);
    m_originalScreenResolution = originalScreenResolution;
}

void Renderable3DScene::FinalizeFrame()
{
    m_sprites.SortSpritesBackToFront();
}

const float Renderable3DScene::GetAspectRatio() const
{
    return m_aspectRatio;
}

const ViewPorts::ViewPortRect3D& Renderable3DScene::GetOriginal3DViewArea() const
{
    return m_original3DViewArea;
}

const float Renderable3DScene::GetCameraX() const
{
    return m_cameraX;
}

const float Renderable3DScene::GetCameraY() const
{
    return m_cameraY;
}

const float Renderable3DScene::GetAngle() const
{
    return m_angle;
}

const Renderable3DWalls& Renderable3DScene::GetWalls() const
{
    return m_walls;
}

const Renderable3DTiles& Renderable3DScene::Get3DTiles() const
{
    return m_3DTiles;
}

const RenderableSprites& Renderable3DScene::GetSprites() const
{
    return m_sprites;
}

const bool Renderable3DScene::GetDepthShading() const
{
    return m_depthShading;
}

const uint16_t Renderable3DScene::GetFieldOfView() const
{
    return m_fieldOfView;
}

const bool Renderable3DScene::GetOriginalScreenResolution() const
{
    return m_originalScreenResolution;
}

void Renderable3DScene::AddNorthWall(const uint16_t x, const uint16_t y, const unsigned int textureId)
{
    const Renderable3DWalls::wallCoordinate wall = Renderable3DWalls::wallCoordinate{ (uint16_t)(x + 1u), y, x, y };
    m_walls.AddWall(textureId, wall);
}

void Renderable3DScene::AddSouthWall(const uint16_t x, const uint16_t y, const unsigned int textureId)
{
    const Renderable3DWalls::wallCoordinate wall = Renderable3DWalls::wallCoordinate{ x, y, (uint16_t)(x + 1u), y };
    m_walls.AddWall(textureId, wall);
}

void Renderable3DScene::AddWestWall(const uint16_t x, const uint16_t y, const unsigned int textureId)
{
    const Renderable3DWalls::wallCoordinate wall = Renderable3DWalls::wallCoordinate{ x, y, x, (uint16_t)(y + 1u) };
    m_walls.AddWall(textureId, wall);
}

void Renderable3DScene::AddEastWall(const uint16_t x, const uint16_t y, const unsigned int textureId)
{
    const Renderable3DWalls::wallCoordinate wall = Renderable3DWalls::wallCoordinate{ x, (uint16_t)(y + 1u), x, y };
    m_walls.AddWall(textureId, wall);
}

Renderable3DTiles& Renderable3DScene::Get3DTilesMutable()
{
    return m_3DTiles;
}
RenderableSprites& Renderable3DScene::GetSpritesMutable()
{
    return m_sprites;
}
