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

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library

class RendererOpenGLWin32: public IRenderer
{
public:
    RendererOpenGLWin32();
    ~RendererOpenGLWin32();

    void Setup() override;
    void SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight) override;
    void SetPlayerAngle(const float angle) override;
    void SetPlayerPosition(const float posX, const float posY) override;
    void SetTextureFilter(const TextureFilterSetting textureFilter) override;
    void SetVSync(const bool enabled) override;
    bool IsVSyncSupported() override;
    unsigned int LoadFileChunkIntoTexture(
        const FileChunk* decompressedChunk,
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t textureWidth,
        const uint16_t textureHeight,
        const bool transparent) override;
    unsigned int  LoadMaskedFileChunkIntoTexture(
        const FileChunk* decompressedChunk,
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t textureWidth,
        const uint16_t textureHeight) override;
    TextureAtlas* CreateTextureAtlasForFont(const bool* fontPicture, const uint16_t lineHeight) override;

    void RenderText(const RenderableText& renderableText) override;

    void Prepare2DRendering(const bool helpWindow) override;
    void Unprepare2DRendering() override;
    void Render2DPicture(const Picture* picture, const uint16_t offsetX, const uint16_t offsetY) override;
    void Render2DPictureSegment(const Picture* picture, const int16_t offsetX, const int16_t offsetY, const uint16_t segmentOffsetX, const uint16_t segmentOffsetY, const uint16_t segmentWidth, const uint16_t segmentHeight) override;
    void Render2DBar(const int16_t x, const int16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex) override;
    void RenderTiles(const RenderableTiles& renderableTiles) override;
    TextureAtlas* CreateTextureAtlasForTilesSize8(const FileChunk* decompressedChunk, const bool masked) override;

    void Prepare3DRendering(const bool depthShading, const float aspectRatio, uint16_t fov, const ViewPorts::ViewPortRect3D original3DViewArea) override;
    uint16_t GetAdditionalMarginDueToWideScreen(const float aspectRatio) override;

    void Render3DWalls(const std::map<unsigned int, std::vector<wallCoordinate>>& textureToWallsMap) override;

    void RenderSprites(const RenderableSprites& renderableSprites) override;
    void RenderFloorAndCeiling(const std::vector<tileCoordinate>& tileCoordinates, const egaColor floorColor, const egaColor ceilingColor) override;

    void PrepareVisibilityMap() override;
    void UnprepareVisibilityMap() override;

    Picture* GetScreenCapture(const unsigned int textureId) override;
    void RemovePixelsFromScreenCapture(const std::vector<std::pair<int16_t, int16_t>>& coordinates) override;
    void RenderScreenCapture(Picture* screenCapture) override;

    uint16_t GetWindowWidth() const override;
    uint16_t GetWindowHeight() const override;
    const std::string& GetGraphicsApiVersion() const override;
    const std::string& GetGraphicsAdapterVendor() const override;
    const std::string& GetGraphicsAdapterModel() const override;

    unsigned int GenerateTextureId() const override;
    void LoadPixelDataIntoTexture(uint32_t width, uint32_t height, uint8_t* pixelData, unsigned int textureId) const;

private:
    static constexpr rgbColor EgaToRgb(const egaColor ega);

    void BindTexture(unsigned int textureId) const;

    unsigned int GenerateSingleColorTexture(const egaColor color) const;
    static const std::string ErrorCodeToString(const GLenum errorCode);

    uint16_t m_windowWidth;
    uint16_t m_windowHeight;
    float m_playerAngle;
    float m_playerPosX, m_playerPosY;
    unsigned int m_singleColorTexture[EgaRange];

    GLint m_textureFilter;
    int32_t m_currentSwapInterval;
    bool m_isVSyncSupported;

    std::string m_graphicsApiVersion;
    std::string m_graphicsAdapterVendor;
    std::string m_graphicsAdapterModel;
};

