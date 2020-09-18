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

#include "RenderableAutoMapTopDown.h"

RenderableAutoMapTopDown::RenderableAutoMapTopDown(const Font& font) :
    m_text(font)
{

}

void RenderableAutoMapTopDown::PrepareFrame(
    const float aspectRatio,
    const ViewPorts::ViewPortRect3D original3DViewArea,
    const float originX,
    const float originY)
{
    m_aspectRatio = aspectRatio;
    m_original3DViewArea = original3DViewArea;
    m_originX = originX;
    m_originY = originY;
}

const float RenderableAutoMapTopDown::GetAspectRatio() const
{
    return m_aspectRatio;
}

const ViewPorts::ViewPortRect3D& RenderableAutoMapTopDown::GetOriginal3DViewArea() const
{
    return m_original3DViewArea;
}

const Renderable3DTiles& RenderableAutoMapTopDown::GetFloorTiles() const
{
    return m_floorTiles;
}

const Renderable3DTiles& RenderableAutoMapTopDown::GetBorderTiles() const
{
    return m_borderTiles;
}

const std::map <const Picture*, std::vector<RenderableAutoMapTopDown::pictureCoordinate>>& RenderableAutoMapTopDown::GetPictures() const
{
    return m_pictures;
}

const std::map <const egaColor, std::vector<RenderableAutoMapTopDown::pictureCoordinate>>& RenderableAutoMapTopDown::GetWallCaps() const
{
    return m_wallCaps;
}

const RenderableText& RenderableAutoMapTopDown::GetText() const
{
    return m_text;
}

const float RenderableAutoMapTopDown::GetOriginX() const
{
    return m_originX;
}

const float RenderableAutoMapTopDown::GetOriginY() const
{
    return m_originY;
}

const float RenderableAutoMapTopDown::GetPlayerX() const
{
    return m_playerX;
}

const float RenderableAutoMapTopDown::GetPlayerY() const
{
    return m_playerY;
}

const float RenderableAutoMapTopDown::GetPlayerAngle() const
{
    return m_playerAngle;
}

void RenderableAutoMapTopDown::SetPlayerPosition(const float x, const float y, const float angle)
{
    m_playerX = x;
    m_playerY = y;
    m_playerAngle = angle;
}

void RenderableAutoMapTopDown::AddPicture(const Picture* picture, const pictureCoordinate coordinate)
{
    if (m_pictures.find(picture) == m_pictures.end())
    {
        m_pictures.insert(std::make_pair(picture, std::vector<pictureCoordinate>()));
    }
    m_pictures.at(picture).push_back(coordinate);
}

void RenderableAutoMapTopDown::AddWallCap(const egaColor color, const pictureCoordinate coordinate)
{
    m_wallCaps[color].push_back(coordinate);
}

Renderable3DTiles& RenderableAutoMapTopDown::GetFloorTilesMutable()
{
    return m_floorTiles;
}

Renderable3DTiles& RenderableAutoMapTopDown::GetBorderTilesMutable()
{
    return m_borderTiles;
}

RenderableText& RenderableAutoMapTopDown::GetTextMutable()
{
    return m_text;
}