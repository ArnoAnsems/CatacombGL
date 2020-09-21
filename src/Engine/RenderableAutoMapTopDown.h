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
// RenderableAutoMapTopDownHD
//
// Contains all the data needed to render a top down HD automap.
//
#pragma once

#include <map>
#include "ViewPorts.h"
#include "Picture.h"
#include "RenderableText.h"
#include "Renderable3DTiles.h"
#include "RenderableTiles.h"

class RenderableAutoMapTopDown
{
public:
    typedef struct
    {
        int16_t x;
        int16_t y;
    } pictureCoordinate;

    RenderableAutoMapTopDown(const Font& font, const ViewPorts::ViewPortRect3D original3DViewArea, const TextureAtlas& textureAtlasTilesSize16);

    void PrepareFrame(
        const float aspectRatio,
        const uint16_t tileSize,
        const float originX,
        const float originY);

    const float GetAspectRatio() const;
    const ViewPorts::ViewPortRect3D& GetOriginal3DViewArea() const;
    const Renderable3DTiles& GetFloorTiles() const;
    const Renderable3DTiles& GetBorderTiles() const;
    const RenderableTiles& GetTilesSize16() const;
    
    const std::map <const Picture*, std::vector<pictureCoordinate>> & GetPictures() const;
    const std::map <const egaColor, std::vector<pictureCoordinate>>& GetWallCaps() const;
    const RenderableText& GetText() const;

    const float GetOriginX() const;
    const float GetOriginY() const;
    const float GetPlayerX() const;
    const float GetPlayerY() const;
    const float GetPlayerAngle() const;
    const uint16_t GetTileSize() const;

    void SetPlayerPosition(const float x, const float y, const float angle);
    void AddPicture(const Picture* picture, const pictureCoordinate coordinate);
    void AddWallCap(const egaColor color, const pictureCoordinate coordinate);
    Renderable3DTiles& GetFloorTilesMutable();
    Renderable3DTiles& GetBorderTilesMutable();
    RenderableTiles& GetTilesSize16Mutable();
    RenderableText& GetTextMutable();

    Renderable3DTiles m_floorTiles;
    Renderable3DTiles m_borderTiles;
    RenderableTiles m_tilesSize16;
    RenderableText m_text;
    std::map <const Picture*, std::vector<pictureCoordinate>> m_pictures;
    std::map <const egaColor, std::vector<pictureCoordinate>> m_wallCaps;
    float m_aspectRatio;
    const ViewPorts::ViewPortRect3D m_original3DViewArea;
    float m_originX;
    float m_originY;
    float m_playerX;
    float m_playerY;
    float m_playerAngle;
    uint16_t m_tileSize;
};