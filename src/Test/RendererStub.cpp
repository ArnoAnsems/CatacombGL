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

void RendererStub::SetPlayerAngle(const float /*angle*/)
{
}

void RendererStub::SetPlayerPosition(const float /*posX*/, const float /*posY*/)
{
}

void RendererStub::SetTextureFilter(const TextureFilterSetting /*textureFilter*/)
{
}

void RendererStub::SetVSync(const bool /*enabled*/)
{
}

bool RendererStub::IsVSyncSupported()
{
    return true;
}

unsigned int RendererStub::LoadFileChunkIntoTexture(const FileChunk* /*decompressedChunk*/, const uint16_t /*imageWidth*/, const uint16_t /*imageHeight*/, const uint16_t /*textureWidth*/, const uint16_t /*textureHeight*/, const bool /*transparent*/)
{
    return 0u;
}

unsigned int RendererStub::LoadMaskedFileChunkIntoTexture(
    const FileChunk* /*decompressedChunk*/,
    const uint16_t /*imageWidth*/,
    const uint16_t /*imageHeight*/,
    const uint16_t /*textureWidth*/,
    const uint16_t /*textureHeight*/)
{
    return 0u;
}

unsigned int RendererStub::LoadTilesSize8IntoTexture(const FileChunk* /*decompressedChunk*/, const uint16_t /*index*/, const bool /*masked*/)
{
    return 0;
}

unsigned int RendererStub::LoadFontIntoTexture(const bool* /*fontPicture*/, const uint16_t /*lineHeight*/)
{
    return 0u;
}

void RendererStub::RenderTextLeftAligned(const char* /*text*/, const Font* /*font*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::RenderTextLeftAlignedTruncated(const char* /*text*/, const Font* /*font*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/, const uint16_t /*maxLength*/)
{
}

uint8_t RendererStub::RenderTextLeftAlignedMultiLine(const char* /*text*/, const Font* /*font*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
    return 1;
}

void RendererStub::RenderTextCentered(const char* /*text*/, const Font* /*font*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::RenderNumber(const uint16_t /*value*/, const Font* /*font*/, const uint8_t /*maxDigits*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::Prepare2DRendering(const bool /*helpWindow*/)
{
}

void RendererStub::Unprepare2DRendering()
{
}

void RendererStub::Render2DPicture(const Picture* /*picture*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::Render2DPictureSegment(const Picture* /*picture*/, const int16_t /*offsetX*/, const int16_t /*offsetY*/, const uint16_t /*segmentOffsetX*/, const uint16_t /*segmentOffsetY*/, const uint16_t /*segmentWidth*/, const uint16_t /*segmentHeight*/)
{
}

void RendererStub::Render2DBar(const int16_t /*x*/, const int16_t /*y*/, const uint16_t /*width*/, const uint16_t /*height*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::RenderRadarBlip(const float /*x*/, const float /*y*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::Prepare3DRendering(const bool /*depthShading*/, const float /*aspectRatio*/, uint16_t /*fov*/, const ViewPorts::ViewPortRect3D /*original3DViewArea*/)
{
}

uint16_t RendererStub::GetAdditionalMarginDueToWideScreen(const float /*aspectRatio*/)
{
    return 0;
}

void RendererStub::PrepareWalls()
{
}

void RendererStub::UnprepareWalls()
{
}

void RendererStub::Render3DWall(const unsigned int /*textureId*/, const wallCoordinate& /*coordinate*/)
{
}

void RendererStub::Render3DSprite(const Picture* /*picture*/, const float /*offsetX*/, const float /*offsetY*/, const SpriteOrientation /*orientation*/)
{
}

void RendererStub::AddSprite(const Picture* /*picture*/, const float /*offsetX*/, const float /*offsetY*/, const SpriteOrientation /*orientation*/)
{
}

void RendererStub::RenderAllSprites()
{
}

void RendererStub::RenderFloorAndCeiling(const std::vector<tileCoordinate>& /*tileCoordinates*/, const egaColor /*floorColor*/, const egaColor /*ceilingColor*/)
{
}

void RendererStub::PrepareVisibilityMap()
{
}

void RendererStub::UnprepareVisibilityMap()
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