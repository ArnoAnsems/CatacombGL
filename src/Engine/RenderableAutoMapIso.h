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
//
// RenderableAutoMapIso
//
// Contains all the data needed to render an isometric automap.
//
#pragma once

#include "Renderable3DTiles.h"
#include "Renderable3DWalls.h"
#include "RenderableSprites.h"
#include "RenderableText.h"
#include "ViewPorts.h"

class RenderableAutoMapIso
{
public:
    typedef struct
    {
        float x1;
        float y1;
        float x2;
        float y2;
        float x3;
        float y3;
        float x4;
        float y4;
    } quadCoordinates;

    RenderableAutoMapIso(
        const Font& font);

    void PrepareFrame(
        const float aspectRatio,
        const ViewPorts::ViewPortRect3D original3DViewArea,
        const float originX,
        const float originY);
    void FinalizeFrame();

    const float GetAspectRatio() const;
    const ViewPorts::ViewPortRect3D& GetOriginal3DViewArea() const;
    const float GetOriginX() const;
    const float GetOriginY() const;
    const float GetPlayerX() const;
    const float GetPlayerY() const;
    const float GetPlayerAngle() const;
    const Renderable3DWalls& GetWalls() const;
    const std::map <egaColor, std::vector<quadCoordinates>>& GetWallCaps() const;
    const Renderable3DTiles& GetFloorTiles() const;
    const RenderableSprites& GetSprites() const;
    const RenderableText& GetText() const;

    void SetPlayerPosition(const float x, const float y, const float angle);
    void AddNorthWall(const uint16_t x, const uint16_t y, const unsigned int textureId);
    void AddWestWall(const uint16_t x, const uint16_t y, const unsigned int textureId);
    void AddWallCap(const uint16_t x, const uint16_t y, const egaColor mainColor, const egaColor centerColor);
    Renderable3DTiles& GetFloorTilesMutable();
    RenderableSprites& GetSpritesMutable();
    RenderableText& GetTextMutable();

private:
    Renderable3DWalls m_walls;
    std::map <egaColor, std::vector<quadCoordinates>> m_wallCaps;
    Renderable3DTiles m_floorTiles;
    RenderableSprites m_sprites;
    RenderableText m_text;
    float m_aspectRatio;
    ViewPorts::ViewPortRect3D m_original3DViewArea;
    float m_originX;
    float m_originY;
    float m_playerX;
    float m_playerY;
    float m_playerAngle;
};
