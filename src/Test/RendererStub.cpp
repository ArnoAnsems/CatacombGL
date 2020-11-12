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

#include "RendererStub.h"

static const std::string defaultString("");

RendererStub::RendererStub()
{
}

RendererStub::~RendererStub()
{
}

void RendererStub::Setup()
{
}

void RendererStub::SetWindowDimensions(const uint16_t /*windowWidth*/, const uint16_t /*windowHeight*/)
{
}

void RendererStub::SetFrameSettings(const FrameSettings& /*frameSettings*/)
{
}

bool RendererStub::IsVSyncSupported()
{
    return true;
}

bool RendererStub::IsOriginalScreenResolutionSupported()
{
    return true;
}

void RendererStub::RenderText(const RenderableText& /*renderableText*/)
{

}

void RendererStub::Prepare2DRendering(const bool /*helpWindow*/)
{
}

void RendererStub::Unprepare2DRendering()
{
}

void RendererStub::Render2DPicture(const Picture* /*picture*/, const int16_t /*offsetX*/, const int16_t /*offsetY*/)
{
}

void RendererStub::Render2DPictureSegment(const Picture* /*picture*/, const int16_t /*offsetX*/, const int16_t /*offsetY*/, const uint16_t /*segmentOffsetX*/, const uint16_t /*segmentOffsetY*/, const uint16_t /*segmentWidth*/, const uint16_t /*segmentHeight*/)
{
}

void RendererStub::Render2DBar(const int16_t /*x*/, const int16_t /*y*/, const uint16_t /*width*/, const uint16_t /*height*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::RenderTiles(const RenderableTiles& /*renderableTiles*/)
{
}

uint16_t RendererStub::GetAdditionalMarginDueToWideScreen(const float /*aspectRatio*/)
{
    return 0;
}

void RendererStub::Render3DScene(const Renderable3DScene& /*renderable3DScene*/)
{
}

void RendererStub::RenderAutoMapTopDown(const RenderableAutoMapTopDown& /*autoMapTopDown*/)
{
}

void RendererStub::RenderAutoMapIso(const RenderableAutoMapIso& /*autoMapIso*/)
{
}

Picture* RendererStub::GetScreenCapture(const unsigned int /*textureId*/)
{
    return nullptr;
}

void RendererStub::RemovePixelsFromScreenCapture(const std::vector<std::pair<int16_t, int16_t>>& /*coordinates*/)
{

}

void RendererStub::RenderScreenCapture(Picture* /*screenCapture*/)
{

}

uint16_t RendererStub::GetWindowWidth() const
{
    return 1;
}

uint16_t RendererStub::GetWindowHeight() const
{
    return 1;
}

const std::string& RendererStub::GetGraphicsApiVersion() const
{
    return defaultString;
}

const std::string& RendererStub::GetGraphicsAdapterVendor() const
{
    return defaultString;
}

const std::string& RendererStub::GetGraphicsAdapterModel() const
{
    return defaultString;
}

void RendererStub::LoadPixelDataIntoTexture(uint32_t /*width*/, uint32_t /*height*/, uint8_t* /*pixelData*/, unsigned int /*textureId*/) const
{
}

unsigned int RendererStub::GenerateTextureId() const
{
    return 0;
}