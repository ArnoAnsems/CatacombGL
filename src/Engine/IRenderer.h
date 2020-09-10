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
#include "EgaColor.h"
#include "ViewPorts.h"
#include "RenderableText.h"
#include "RenderableTiles.h"
#include "RenderableSprites.h"
#include "Renderable3DWalls.h"
#include "Renderable3DTiles.h"
#include "RenderableAutoMapIso.h"
#include "Renderable3DScene.h"
#include <vector>
#include <map>
#include <string>

class IRenderer
{
public:
    enum TextureFilterSetting
    {
        Nearest,
        Linear
    };

    typedef struct
    {
        TextureFilterSetting textureFilter;
        bool vSyncEnabled;
    } FrameSettings;

    virtual ~IRenderer() {};

    //
    // Screen setup
    //
    virtual void Setup() = 0;
    virtual void SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight) = 0;
    virtual uint16_t GetWindowWidth() const = 0;
    virtual uint16_t GetWindowHeight() const = 0;
    virtual void SetFrameSettings(const FrameSettings& frameSettings) = 0;

    //
    // Texture creation
    //
    virtual unsigned int GenerateTextureId() const = 0;
    virtual void LoadPixelDataIntoTexture(uint32_t width, uint32_t height, uint8_t* pixelData, unsigned int textureId) const = 0;

    //
    // 2D rendering
    //
    virtual void Prepare2DRendering(const bool helpWindow) = 0;
    virtual void Unprepare2DRendering() = 0;
    virtual void Render2DPicture(const Picture* picture, const int16_t offsetX, const int16_t offsetY) = 0;
    virtual void Render2DPictureSegment(const Picture* picture, const int16_t offsetX, const int16_t offsetY, const uint16_t segmentOffsetX, const uint16_t segmentOffsetY, const uint16_t segmentWidth, const uint16_t segmentHeight) = 0;
    virtual void Render2DBar(const int16_t x, const int16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex) = 0;
    virtual void RenderTiles(const RenderableTiles& renderableTiles) = 0;
    virtual void RenderText(const RenderableText& renderableText) = 0;
    virtual uint16_t GetAdditionalMarginDueToWideScreen(const float aspectRatio) = 0;

    //
    // 3D rendering
    //
    virtual void Render3DScene(const Renderable3DScene& renderable3DScene) = 0;
    virtual void RenderAutoMapIso(const RenderableAutoMapIso& autoMapIso) = 0;
    virtual void PrepareTopDownRendering(const float aspectRatio, const ViewPorts::ViewPortRect3D original3DViewArea, const uint16_t scale) = 0;
    virtual void RenderTopDownFloorTiles(const Renderable3DTiles& tiles) = 0;

    //
    // Screen capture
    //
    virtual Picture* GetScreenCapture(const unsigned int textureId) = 0;
    virtual void RemovePixelsFromScreenCapture(const std::vector<std::pair<int16_t, int16_t>>& coordinates) = 0;
    virtual void RenderScreenCapture(Picture* screenCapture) = 0;

    //
    // Capabilities
    //
    virtual const std::string& GetGraphicsApiVersion() const = 0;
    virtual const std::string& GetGraphicsAdapterVendor() const = 0;
    virtual const std::string& GetGraphicsAdapterModel() const = 0;
    virtual bool IsVSyncSupported() = 0;
};
