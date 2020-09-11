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

#include "RenderableAutoMapIso.h"

RenderableAutoMapIso::RenderableAutoMapIso(
    const Font& font) :
    m_aspectRatio(1.0f),
    m_original3DViewArea(),
    m_originX(1.0f),
    m_originY(1.0f),
    m_playerX(1.0f),
    m_playerY(1.0f),
    m_playerAngle(0.0f),
    m_sprites(),
    m_text(font)
{

}

void RenderableAutoMapIso::PrepareFrame(
    const float aspectRatio,
    const ViewPorts::ViewPortRect3D original3DViewArea,
    const float originX,
    const float originY)
{
    m_aspectRatio = aspectRatio;
    m_original3DViewArea = original3DViewArea;
    m_originX = originX;
    m_originY = originY;
    m_sprites.Reset(originX, originY, 0.0f);
}

void RenderableAutoMapIso::FinalizeFrame()
{
    m_sprites.SortSpritesBackToFront();
}

const float RenderableAutoMapIso::GetAspectRatio() const
{
    return m_aspectRatio;
}

const ViewPorts::ViewPortRect3D& RenderableAutoMapIso::GetOriginal3DViewArea() const
{
    return m_original3DViewArea;
}

const float RenderableAutoMapIso::GetOriginX() const
{
    return m_originX;
}

const float RenderableAutoMapIso::GetOriginY() const
{
    return m_originY;
}

const float RenderableAutoMapIso::GetPlayerX() const
{
    return m_playerX;
}

const float RenderableAutoMapIso::GetPlayerY() const
{
    return m_playerY;
}

const float RenderableAutoMapIso::GetPlayerAngle() const
{
    return m_playerAngle;
}

const Renderable3DWalls& RenderableAutoMapIso::GetWalls() const
{
    return m_walls;
}

const std::map <egaColor, std::vector<RenderableAutoMapIso::quadCoordinates>>& RenderableAutoMapIso::GetWallCaps() const
{
    return m_wallCaps;
}

const Renderable3DTiles& RenderableAutoMapIso::GetFloorTiles() const
{
    return m_floorTiles;
}

const RenderableSprites& RenderableAutoMapIso::GetSprites() const
{
    return m_sprites;
}

const RenderableText& RenderableAutoMapIso::GetText() const
{
    return m_text;
}

void RenderableAutoMapIso::SetPlayerPosition(const float x, const float y, const float angle)
{
    m_playerX = x;
    m_playerY = y;
    m_playerAngle = angle;
}

void RenderableAutoMapIso::AddNorthWall(const uint16_t x, const uint16_t y, const unsigned int textureId)
{
    const Renderable3DWalls::wallCoordinate wall = Renderable3DWalls::wallCoordinate{ x + 1u, y, x, y };
    m_walls.AddWall(textureId, wall);
}

void RenderableAutoMapIso::AddWestWall(const uint16_t x, const uint16_t y, const unsigned int textureId)
{
    Renderable3DWalls::wallCoordinate wall = Renderable3DWalls::wallCoordinate{ x, y, x, y + 1u };
    m_walls.AddWall(textureId, wall);
}

void RenderableAutoMapIso::AddWallCap(
    const uint16_t x,
    const uint16_t y,
    const egaColor mainColor,
    const egaColor centerColor)
{
    if (centerColor != mainColor)
    {
        const float border = 0.2f;
        quadCoordinates quad =
        {
            (float)x + border, (float)y + border,
            (float)x + 1.0f - border, (float)y + border,
            (float)x + 1.0f - border, (float)y + 1.0f - border,
            (float)x + border, (float)y + 1.0f - border,
        };
        m_wallCaps[centerColor].push_back(quad);
        quad =
        {
            (float)x, (float)y,
            (float)x + 1.0f, (float)y,
            (float)x + 1.0f - border, (float)y + border,
            (float)x + border, (float)y + border,
        };
        m_wallCaps[mainColor].push_back(quad);
        quad =
        {
            (float)x + 1.0f, (float)y,
            (float)x + 1.0f, (float)y + 1.0f,
            (float)x + 1.0f - border, (float)y + 1.0f - border,
            (float)x + 1.0f - border, (float)y + border,
        };
        m_wallCaps[mainColor].push_back(quad);
        quad =
        {
            (float)x + 1.0f, (float)y + 1.0f,
            (float)x, (float)y + 1.0f,
            (float)x + border, (float)y + 1.0f - border,
            (float)x + 1.0f - border, (float)y + 1.0f - border,
        };
        m_wallCaps[mainColor].push_back(quad);
        quad =
        {
            (float)x, (float)y + 1.0f,
            (float)x, (float)y,
            (float)x + border, (float)y + border,
            (float)x + border, (float)y + 1.0f - border,
        };
        m_wallCaps[mainColor].push_back(quad);
    }
    else
    {
        quadCoordinates quad =
        {
            (float)x,
            (float)y,
            (float)x + 1.0f,
            (float)y,
            (float)x + 1.0f,
            (float)y + 1.0f,
            (float)x,
            (float)y + 1.0f,
        };
        m_wallCaps[mainColor].push_back(quad);
    }
}

Renderable3DTiles& RenderableAutoMapIso::GetFloorTilesMutable()
{
    return m_floorTiles;
}

RenderableSprites& RenderableAutoMapIso::GetSpritesMutable()
{
    return m_sprites;
}

RenderableText& RenderableAutoMapIso::GetTextMutable()
{
    return m_text;
}