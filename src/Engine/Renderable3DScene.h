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
// Renderable3DScene
//
// Contains all the data needed for the renderer to draw a 3D scene.
//
#pragma once

#include <vector>
#include "Renderable3DWalls.h"
#include "Renderable3DTiles.h"
#include "RenderableSprites.h"
#include "ViewPorts.h"

class Renderable3DScene
{
public:
    Renderable3DScene(const ViewPorts::ViewPortRect3D original3DViewArea);

    void PrepareFrame(
        const float aspectRatio,
        const float playerX,
        const float playerY,
        const float angle,
        const bool depthShading,
        const uint16_t fieldOfView,
        const bool originalScreenResolution,
        const bool cameraBehindPlayer);
    void FinalizeFrame();

    const float GetAspectRatio() const;
    const ViewPorts::ViewPortRect3D& GetOriginal3DViewArea() const;
    const float GetCameraX() const;
    const float GetCameraY() const;
    const float GetAngle() const;
    const Renderable3DWalls& GetWalls() const;
    const Renderable3DTiles& Get3DTiles() const;
    const RenderableSprites& GetSprites() const;
    const bool GetDepthShading() const;
    const uint16_t GetFieldOfView() const;
    const bool GetOriginalScreenResolution() const;

    void AddNorthWall(const uint16_t x, const uint16_t y, const unsigned int textureId);
    void AddSouthWall(const uint16_t x, const uint16_t y, const unsigned int textureId);
    void AddWestWall(const uint16_t x, const uint16_t y, const unsigned int textureId);
    void AddEastWall(const uint16_t x, const uint16_t y, const unsigned int textureId);
    Renderable3DTiles& Get3DTilesMutable();
    RenderableSprites& GetSpritesMutable();

private:
    const ViewPorts::ViewPortRect3D m_original3DViewArea;
    Renderable3DWalls m_walls;
    Renderable3DTiles m_3DTiles;
    RenderableSprites m_sprites;
    float m_aspectRatio;
    float m_cameraX;
    float m_cameraY;
    float m_angle;
    bool m_depthShading;
    uint16_t m_fieldOfView;
    bool m_originalScreenResolution;
    bool m_originalCameraView;
};
