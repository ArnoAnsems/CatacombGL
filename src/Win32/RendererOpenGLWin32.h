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
#pragma once

#include "..\Engine\Picture.h"
#include "..\Engine\Font.h"
#include "..\Engine\FileChunk.h"
#include "..\Engine\Decompressor.h"
#include "..\Engine\EgaColor.h"
#include "..\Engine\IRenderer.h"
#include "..\Engine\Logging.h"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library

class RendererOpenGLWin32: public IRenderer
{
public:
    RendererOpenGLWin32(Logging* const logging);
    ~RendererOpenGLWin32();

    void Setup() override;
    void SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight) override;
    void SetPlayerAngle(const float angle) override;
    void SetPlayerPosition(const float posX, const float posY) override;
    void SetTextureFilter(const TextureFilterSetting textureFilter) override;
    void SetVSync(const bool enabled) override;
    bool IsVSyncSupported() override;
    unsigned int LoadFileChunkIntoTexture(const FileChunk* decompressedChunk, const uint16_t width, const uint16_t height, const bool transparent) override;
    unsigned int  LoadMaskedFileChunkIntoTexture(const FileChunk* decompressedChunk, const uint16_t width, const uint16_t height) override;
    unsigned int LoadTilesSize8MaskedIntoTexture(const FileChunk* decompressedChunk) override;
    unsigned int LoadFontIntoTexture(const bool* fontPicture) override;

    void RenderTextLeftAligned(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) override;
    void RenderTextLeftAlignedTruncated(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY, const uint16_t maxLength) override;
    uint8_t RenderTextLeftAlignedMultiLine(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) override;
    void RenderTextCentered(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) override;
    void RenderNumber(const uint16_t value, const Font* font, const uint8_t maxDigits, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY) override;

    void Prepare2DRendering(const bool helpWindow) override;
    void Unprepare2DRendering() override;
    void Render2DPicture(const Picture* picture, const uint16_t offsetX, const uint16_t offsetY) override;
    void Render2DTileSize8Masked(const Picture* tiles, const uint16_t tileIndex, const uint16_t offsetX, const uint16_t offsetY) override;
    void Render2DBar(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex) override;
    void RenderRadarBlip(const float x, const float y, const egaColor colorIndex) override;

    void Prepare3DRendering(const bool depthShading, const float aspectRatio, uint16_t fov) override;

    void PrepareWalls() override;
    void UnprepareWalls() override;
    void Render3DWall(const Picture* picture, const int16_t tileX, const int16_t tileY, const int16_t orientation) override;
    void Render3DSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation) override;

    void AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation) override;
    void RenderAllSprites();
    void PrepareFloorAndCeiling() override;
    void UnprepareFloorAndCeiling() override;
    void RenderFloor(const uint16_t tileX, const uint16_t tileY, const egaColor colorIndex) override;
    void RenderCeiling(const uint16_t tileX, const uint16_t tileY, const egaColor colorIndex) override;

    void PrepareVisibilityMap() override;
    void UnprepareVisibilityMap() override;

private:
    typedef struct
    {
        const Picture* picture;
        float offsetX;
        float offsetY;
        int32_t squaredDistance;
        SpriteOrientation orientation;
    } spriteToRender;

    static rgbColor EgaToRgb(const egaColor ega);

    void quickSort(uint16_t p,uint16_t q);
    uint16_t partition(uint16_t p,uint16_t q);
    void swap(uint16_t p,uint16_t q);
    unsigned int generateSingleColorTexture(const egaColor color) const;

    uint16_t m_windowWidth;
    uint16_t m_windowHeight;
    float m_playerAngle;
    float m_playerPosX, m_playerPosY;
    spriteToRender* m_spritesToRender;
    uint16_t m_numberOfSprites;
    unsigned int m_singleColorTexture[EgaRange];

    GLint m_textureFilter;
    int32_t m_currentSwapInterval;
    bool m_isVSyncSupported;
    Logging* const m_logging;
};

