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
#include "..\Engine\OpenGLFrameBuffer.h"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library

class RendererOpenGLWin32: public IRenderer
{
public:
    RendererOpenGLWin32();
    ~RendererOpenGLWin32();

    //
    // Screen setup
    //
    void Setup() override;
    void SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight) override;
    uint16_t GetWindowWidth() const override;
    uint16_t GetWindowHeight() const override;
    void SetFrameSettings(const FrameSettings& frameSettings) override;

    //
    // Texture creation
    //
    unsigned int GenerateTextureId() const override;
    void LoadPixelDataIntoTexture(uint32_t width, uint32_t height, uint8_t* pixelData, unsigned int textureId) const;

    //
    // 2D rendering
    //
    void Prepare2DRendering(const bool helpWindow) override;
    void Unprepare2DRendering() override;
    void Render2DPicture(const Picture* picture, const int16_t offsetX, const int16_t offsetY) override;
    void Render2DPictureSegment(const Picture* picture, const int16_t offsetX, const int16_t offsetY, const uint16_t segmentOffsetX, const uint16_t segmentOffsetY, const uint16_t segmentWidth, const uint16_t segmentHeight) override;
    void Render2DBar(const int16_t x, const int16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex) override;
    void RenderTiles(const RenderableTiles& renderableTiles) override;
    void RenderText(const RenderableText& renderableText) override;
    uint16_t GetAdditionalMarginDueToWideScreen(const float aspectRatio) override;

    //
    // 3D rendering
    //
    void Render3DScene(const Renderable3DScene& renderable3DScene) override;
    void RenderAutoMapTopDown(const RenderableAutoMapTopDown& autoMapTopDown) override;
    void RenderAutoMapIso(const RenderableAutoMapIso& autoMapIso) override;

    //
    // Screen capture
    //
    Picture* GetScreenCapture(const unsigned int textureId) override;
    void RemovePixelsFromScreenCapture(const std::vector<std::pair<int16_t, int16_t>>& coordinates) override;
    void RenderScreenCapture(Picture* screenCapture) override;

    //
    // Capabilities
    //
    const std::string& GetGraphicsApiVersion() const override;
    const std::string& GetGraphicsAdapterVendor() const override;
    const std::string& GetGraphicsAdapterModel() const override;
    bool IsVSyncSupported() override;
    bool IsOriginalScreenResolutionSupported() override;

private:
    void BindTexture(unsigned int textureId) const;

    unsigned int GenerateSingleColorTexture(const egaColor color) const;
    static const std::string ErrorCodeToString(const GLenum errorCode);

    float PrepareIsoRendering(const float aspectRatio, const ViewPorts::ViewPortRect3D original3DViewArea, const float originX, const float originY);
    void PrepareIsoRenderingText(const float originX, const float originY, const float xScale);
    void RenderIsoWallCaps(const std::map <egaColor, std::vector<RenderableAutoMapIso::quadCoordinates>>& wallCaps);
    void Render3DWalls(const Renderable3DWalls& walls);
    void Render3DTiles(const Renderable3DTiles& tiles);
    void RenderSprites(const RenderableSprites& renderableSprites);
    void PrepareTopDownRendering(const float aspectRatio, const ViewPorts::ViewPortRect3D original3DViewArea, const uint16_t scale);
    void RenderTopDownFloorTiles(const Renderable3DTiles& tiles, const uint16_t tileSize);
    void ApplyDepthShading(const Renderable3DScene& renderable3DScene) const;

    uint16_t m_windowWidth;
    uint16_t m_windowHeight;
    unsigned int m_singleColorTexture[EgaRange];

    GLint m_textureFilter;
    int32_t m_currentSwapInterval;
    bool m_isVSyncSupported;

    std::string m_graphicsApiVersion;
    std::string m_graphicsAdapterVendor;
    std::string m_graphicsAdapterModel;

    OpenGLBasic m_openGLBasic;
    OpenGLFrameBuffer m_openGLFramebuffer;
};

