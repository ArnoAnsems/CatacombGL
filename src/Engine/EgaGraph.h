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
// EgaGraph
//
// Class for reading data structures (pictures, fonts, etc.) from an EGAGRAPH file.
//
#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "Huffman.h"
#include "IRenderer.h"
#include "Logging.h"

class Picture;
class Font;
class PictureTable;
class SpriteTable;
class LevelLocationNames;

typedef struct egaGraphStaticData
{
    const std::string filename;
    const std::vector<int32_t>& offsets;
    const huffmanTable& table;
    uint16_t indexOfFirstPicture;
    uint16_t indexOfFirstScaledPicture;
    uint16_t indexOfFirstWallPicture;
    uint16_t indexOfFirstMaskedPicture;
    uint16_t indexOfFirstSprite;
    uint16_t indexOfTileSize8;
    uint16_t indexOfTileSize8Masked;
    uint16_t indexOfFirstTileSize16;
    uint16_t indexOfLastTileSize16;
    uint16_t indexOfFirstTileSize16Masked;
    uint16_t indexOfLastTileSize16Masked;
    uint16_t indexOfFirstWorldLocationNames;
    uint16_t indexOfLastWorldLocationNames;
    uint16_t indexOfHandPicture;
} egaGraphStaticData;

class EgaGraph
{
public:
    EgaGraph(const egaGraphStaticData& staticData, const std::string& path, IRenderer& renderer);
    ~EgaGraph();

    Picture* GetPicture(const uint16_t index);
    Picture* GetMaskedPicture(const uint16_t index);
    Picture* GetSprite(const uint16_t index);
    Font* GetFont(const uint16_t index);
    LevelLocationNames* GetWorldLocationNames(const uint16_t index);
    uint16_t GetNumberOfWorldLocationNames() const;
    uint16_t GetHandPictureIndex() const;

    const TextureAtlas* const GetTilesSize8() const;
    const TextureAtlas* const GetTilesSize8Masked() const;
    const TextureAtlas* const GetTilesSize16() const;
    const TextureAtlas* const GetTilesSize16Masked() const;
    uint16_t GetNumberOfTilesSize16(const bool masked) const;

private:
    uint32_t GetChunkSize(const uint16_t index) const;
    TextureAtlas* CreateTextureAtlasForTilesSize8(const FileChunk* decompressedChunk, const bool masked) const;
    TextureAtlas* CreateTextureAtlasForTilesSize16(const bool masked) const;
    TextureAtlas* CreateTextureAtlasForFont(const bool* fontPicture, const uint16_t lineHeight);
    unsigned int LoadFileChunkIntoTexture(
        const FileChunk* decompressedChunk,
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t textureWidth,
        const uint16_t textureHeight,
        const bool transparent);
    unsigned int LoadMaskedFileChunkIntoTexture(
        const FileChunk* decompressedChunk,
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t textureWidth,
        const uint16_t textureHeight);

    const egaGraphStaticData& m_staticData;

    FileChunk* m_rawData;
    PictureTable* m_pictureTable;
    PictureTable* m_maskedPictureTable;
    SpriteTable* m_spriteTable;
    Picture** m_pictures;
    Picture** m_maskedPictures;
    Picture** m_sprites;
    LevelLocationNames** m_worldLocationNames;
    Huffman* m_huffman;
    Font** m_fonts;
    IRenderer& m_renderer;

    const TextureAtlas* m_tilesSize8TextureAtlas;
    const TextureAtlas* m_tilesSize8MaskedTextureAtlas;
    const TextureAtlas* m_tilesSize16TextureAtlas;
    const TextureAtlas* m_tilesSize16MaskedTextureAtlas;
};

