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
// IRenderer
//
// Abstract interface for all rendering tasks.
// Currently only used to abstract the Win32 OpenGL calls from the OS-independent engine library.
// Can potentially be used to create a Vulkan implementation next to the existing OpenGL implementation.
//
#pragma once

#include "Picture.h"
#include "FileChunk.h"
#include "EgaColor.h"

class Font;

class IRenderer
{
public:
    typedef struct
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } rgbColor;

    enum TextureFilterSetting
    {
        Nearest,
        Linear
    };

    enum SpriteOrientation
    {
        RotatedTowardsPlayer,
        AlongXAxis,
        AlongYAxis
    };

    virtual ~IRenderer() {};
    virtual void Setup() = NULL;
    virtual void SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight) = NULL;
    virtual void SetPlayerAngle(const float angle) = NULL;
    virtual void SetPlayerPosition(const float posX, const float posY) = NULL;
    virtual void SetTextureFilter(const TextureFilterSetting textureFilter) = NULL;
    virtual void SetVSync(const bool enabled) = NULL;
    virtual bool IsVSyncSupported() = NULL;
    virtual unsigned int LoadFileChunkIntoTexture(const FileChunk* decompressedChunk, const uint16_t width, const uint16_t height, const bool transparent) = NULL;
    virtual unsigned int LoadMaskedFileChunkIntoTexture(const FileChunk* decompressedChunk, const uint16_t width, const uint16_t height) = NULL;
    virtual unsigned int LoadTilesSize8MaskedIntoTexture(const FileChunk* decompressedChunk) = NULL;
    virtual unsigned int LoadFontIntoTexture(const bool* fontPicture) = NULL;

    virtual void RenderTextLeftAligned(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) = NULL;
    virtual void RenderTextLeftAlignedTruncated(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY, const uint16_t maxLength) = NULL;
    virtual uint8_t RenderTextLeftAlignedMultiLine(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) = NULL;
    virtual void RenderTextCentered(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) = NULL;
    virtual void RenderNumber(const uint16_t value, const Font* font, const uint8_t maxDigits, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) = NULL;

    virtual void Prepare2DRendering(const bool helpWindow) = NULL;
    virtual void Unprepare2DRendering() = NULL;
    virtual void Render2DPicture(const Picture* picture, const uint16_t offsetX, const uint16_t offsetY) = NULL;
    virtual void Render2DTileSize8Masked(const Picture* tiles, const uint16_t tileIndex, const uint16_t offsetX, const uint16_t offsetY) = NULL;
    virtual void Render2DBar(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex) = NULL;
    virtual void RenderRadarBlip(const float x, const float y, const egaColor colorIndex) = NULL;

    virtual void Prepare3DRendering(const bool depthShading, const float aspectRatio, uint16_t fov) = NULL;

    virtual void PrepareWalls() = NULL;
    virtual void UnprepareWalls() = NULL;
    virtual void Render3DWall(const Picture* picture, const int16_t tileX, const int16_t tileY, const int16_t orientation) = NULL;
    virtual void Render3DSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation) = NULL;

    virtual void AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation) = NULL;
    virtual void RenderAllSprites() = NULL;
    virtual void PrepareFloorAndCeiling() = NULL;
    virtual void UnprepareFloorAndCeiling() = NULL;
    virtual void RenderFloor(const uint16_t tileX, const uint16_t tileY, const egaColor colorIndex) = NULL;
    virtual void RenderCeiling(const uint16_t tileX, const uint16_t tileY, const egaColor colorIndex) = NULL;

    virtual void PrepareVisibilityMap() = NULL;
    virtual void UnprepareVisibilityMap() = NULL;
};
