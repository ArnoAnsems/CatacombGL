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

#include "..\Engine\IRenderer.h"

class RendererStub : public IRenderer
{
public:
    RendererStub();
    ~RendererStub();

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
    unsigned int LoadMaskedFileChunkIntoTexture(
        const FileChunk* decompressedChunk,
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t textureWidth,
        const uint16_t textureHeight) override;
    TextureAtlas* CreateTextureAtlasForFont(const bool* fontPicture, const uint16_t lineHeight) override;

    void RenderTextLeftAligned(const char* text, const Font* font, const egaColor colorIndex, const int16_t offsetX, const int16_t offsetY) override;
    void RenderTextLeftAlignedTruncated(const char* text, const Font* font, const egaColor colorIndex, const int16_t offsetX, const int16_t offsetY, const uint16_t maxLength) override;
    uint8_t RenderTextLeftAlignedMultiLine(const char* text, const Font* font, const egaColor colorIndex, const int16_t offsetX, const int16_t offsetY) override;
    void RenderTextCentered(const char* text, const Font* font, const egaColor colorIndex, const int16_t offsetX, const int16_t offsetY) override;
    void RenderNumber(const uint16_t value, const Font* font, const uint8_t maxDigits, const egaColor colorIndex, const int16_t offsetX, const int16_t offsetY) override;

    void Prepare2DRendering(const bool helpWindow) override;
    void Unprepare2DRendering() override;
    void Render2DPicture(const Picture* picture, const uint16_t offsetX, const uint16_t offsetY) override;
    void Render2DPictureSegment(const Picture* picture, const int16_t offsetX, const int16_t offsetY, const uint16_t segmentOffsetX, const uint16_t segmentOffsetY, const uint16_t segmentWidth, const uint16_t segmentHeight) override;
    void Render2DBar(const int16_t x, const int16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex) override;
    void RenderImagesFromTextureAtlas(const std::vector<imageOnTextureAtlas>& images, const TextureAtlas& textureAtlas) override;
    TextureAtlas* CreateTextureAtlasForTilesSize8(const FileChunk* decompressedChunk, const bool masked) override;

    void Prepare3DRendering(const bool depthShading, const float aspectRatio, uint16_t fov, const ViewPorts::ViewPortRect3D original3DViewArea) override;
    uint16_t GetAdditionalMarginDueToWideScreen(const float aspectRatio) override;

    void Render3DWalls(const std::map<unsigned int, std::vector<wallCoordinate>>& textureToWallsMap) override;
    void Render3DSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation) override;

    void AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation) override;
    void RenderAllSprites();
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
};

