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

uint32_t RendererStub::LoadFileChunkIntoTexture(const FileChunk* /*decompressedChunk*/, const uint16_t /*width*/, const uint16_t /*height*/, const bool /*transparent*/)
{
    return 0u;
}

uint32_t RendererStub::LoadMaskedFileChunkIntoTexture(const FileChunk* /*decompressedChunk*/, const uint16_t /*width*/, const uint16_t /*height*/)
{
    return 0u;
}

uint32_t RendererStub::LoadTilesSize8MaskedIntoTexture(const FileChunk* /*decompressedChunk*/)
{
    return 0;
}

uint32_t RendererStub::LoadFontIntoTexture(const bool* /*fontPicture*/)
{
    return 0u;
}

void RendererStub::RenderTextLeftAligned(const char* /*text*/, const Font* /*font*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::RenderTextCentered(const char* /*text*/, const Font* /*font*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::RenderNumber(const uint16_t /*value*/, const Font* /*font*/, const uint8_t /*maxDigits*/, const egaColor /*colorIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::Prepare2DRendering()
{
}

void RendererStub::Unprepare2DRendering()
{
}

void RendererStub::Render2DPicture(const Picture* /*picture*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::Render2DTileSize8Masked(const Picture* /*tiles*/, const uint16_t /*tileIndex*/, const uint16_t /*offsetX*/, const uint16_t /*offsetY*/)
{
}

void RendererStub::Render2DBar(const uint16_t /*x*/, const uint16_t /*y*/, const uint16_t /*width*/, const uint16_t /*height*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::RenderRadarBlip(const float /*x*/, const float /*y*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::Prepare3DRendering(const bool /*depthShading*/, const float /*aspectRatio*/, uint16_t /*fov*/)
{
}

void RendererStub::PrepareWalls()
{
}

void RendererStub::UnprepareWalls()
{
}

void RendererStub::Render3DWall(const Picture* /*picture*/, const int16_t /*tileX*/, const int16_t /*tileY*/, const int16_t /*orientation*/)
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

void RendererStub::PrepareFloorAndCeiling()
{
}

void RendererStub::UnprepareFloorAndCeiling()
{
}

void RendererStub::RenderFloor(const uint16_t /*tileX*/, const uint16_t /*tileY*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::RenderCeiling(const uint16_t /*tileX*/, const uint16_t /*tileY*/, const egaColor /*colorIndex*/)
{
}

void RendererStub::PrepareVisibilityMap()
{
}

void RendererStub::UnprepareVisibilityMap()
{
}
