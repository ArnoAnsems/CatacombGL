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

#include "EgaGraph.h"
#include "IRenderer.h"
#include <fstream>

#include "Picture.h"
#include "Font.h"
#include "DefaultFont.h"
#include "PictureTable.h"
#include "SpriteTable.h"
#include "LevelLocationNames.h"

static const uint16_t numTilesSize8 = 104;
static const uint16_t numTilesSize8Masked = 12;

EgaGraph::EgaGraph(const egaGraphStaticData& staticData, const std::string& path, IRenderer& renderer) :
    m_staticData(staticData),
    m_renderer(renderer)
{
    Logging::Instance().AddLogMessage("Loading " + m_staticData.filename);

    // Initialize Huffman table
    m_huffman = new Huffman(m_staticData.table);

    // Read the entire EGA graph file into memory
    uint32_t fileSize = m_staticData.offsets.back();
    m_rawData = new FileChunk(fileSize);
    std::ifstream file;
    const std::string fullPath = path + m_staticData.filename;
    file.open(fullPath, std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char*)m_rawData->GetChunk(), fileSize);
        if (file.fail())
        {
            Logging::Instance().FatalError("Failed to read " + std::to_string(fileSize) + " bytes from " + m_staticData.filename);
        }
        file.close();
    }
    else
    {
        Logging::Instance().FatalError("Failed to open " + fullPath);
    }

    // Initialize picture table
    uint8_t* compressedPictureTable = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(0)];
    uint32_t compressedSize = GetChunkSize(0) - sizeof(uint32_t);
    uint32_t uncompressedSize = *(uint32_t*)compressedPictureTable;
    FileChunk* pictureTableChunk = m_huffman->Decompress(&compressedPictureTable[sizeof(uint32_t)], compressedSize, uncompressedSize);
    m_pictureTable = new PictureTable(pictureTableChunk);
    delete pictureTableChunk;

    if (m_staticData.indexOfFirstMaskedPicture - m_staticData.indexOfFirstPicture > m_pictureTable->GetCount())
    {
        Logging::Instance().FatalError("Picture table only contains " + std::to_string(m_pictureTable->GetCount()) + " entries, while there are " + std::to_string(m_staticData.indexOfFirstMaskedPicture - m_staticData.indexOfFirstPicture) + " pictures in " + m_staticData.filename);
    }

    // Initialize Pictures
    m_pictures = new Picture*[m_pictureTable->GetCount()];
    for (uint16_t i = 0; i < m_pictureTable->GetCount(); i++)
    {
        m_pictures[i] = nullptr;
    }

    // Initialize masked picture table
    uint8_t* compressedMaskedPictureTable = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(1)];
    uint32_t compressedSize2 = GetChunkSize(1) - sizeof(uint32_t);
    uint32_t uncompressedSize2 = *(uint32_t*)compressedMaskedPictureTable;
    FileChunk* maskedPictureTableChunk = m_huffman->Decompress(&compressedMaskedPictureTable[sizeof(uint32_t)], compressedSize2, uncompressedSize2);
    m_maskedPictureTable = new PictureTable(maskedPictureTableChunk);
    delete maskedPictureTableChunk;

    if (m_staticData.indexOfFirstSprite - m_staticData.indexOfFirstMaskedPicture > m_maskedPictureTable->GetCount())
    {
        Logging::Instance().FatalError("Masked picture table only contains " + std::to_string(m_maskedPictureTable->GetCount()) + " entries, while there are " + std::to_string(m_staticData.indexOfFirstSprite - m_staticData.indexOfFirstMaskedPicture) + " masked pictures in " + m_staticData.filename);
    }

    // Initialize Masked Pictures
    m_maskedPictures = new Picture*[m_maskedPictureTable->GetCount()];
    for (uint16_t i = 0; i < m_maskedPictureTable->GetCount(); i++)
    {
        m_maskedPictures[i] = nullptr;
    }

    // Initialize sprites table
    uint8_t* compressedSpritesTable = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(2)];
    uint32_t compressedSize3 = GetChunkSize(2) - sizeof(uint32_t);
    uint32_t uncompressedSize3 = *(uint32_t*)compressedSpritesTable;
    FileChunk* spritesTableChunk = m_huffman->Decompress(&compressedSpritesTable[sizeof(uint32_t)], compressedSize3, uncompressedSize3);
    m_spriteTable = new SpriteTable(spritesTableChunk);
    delete spritesTableChunk;

    if (m_spriteTable->GetCount() == 0)
    {
        Logging::Instance().FatalError("Sprite table in " + m_staticData.filename + " has no entries.");
    }

    // Initialize sprites
    m_sprites = new Picture*[m_spriteTable->GetCount()];
    for (uint16_t i = 0; i < m_spriteTable->GetCount(); i++)
    {
        m_sprites[i] = nullptr;
    }

    // Initialize tiles
    m_tilesSize8 = new Picture*[numTilesSize8];
    for (uint16_t i = 0; i < numTilesSize8; i++)
    {
        m_tilesSize8[i] = nullptr;
    }
    m_tilesSize8Masked = new Picture*[numTilesSize8Masked];
    for (uint16_t i = 0; i < numTilesSize8Masked; i++)
    {
        m_tilesSize8Masked[i] = nullptr;
    }

    // Initialize fonts
    const uint16_t numFonts = m_staticData.indexOfFirstPicture - 3;
    m_fonts = new Font*[numFonts];
    for (uint16_t i = 0; i < numFonts; i++)
    {
        m_fonts[i] = nullptr;
    }

    // Initialize location names
    const uint16_t numberOfWorldLocationNames = m_staticData.indexOfLastWorldLocationNames - m_staticData.indexOfFirstWorldLocationNames + 1;
    m_worldLocationNames = new LevelLocationNames*[numberOfWorldLocationNames];
    for (uint16_t i = 0; i < numberOfWorldLocationNames; i++)
    {
        m_worldLocationNames[i] = nullptr;
    }
}

EgaGraph::~EgaGraph()
{
    for (uint16_t i = 0; i < m_pictureTable->GetCount(); i++)
    {
        delete m_pictures[i];
        m_pictures[i] = nullptr;
    }
    delete[] m_pictures;
    delete m_pictureTable;

    for (uint16_t i = 0; i < m_maskedPictureTable->GetCount(); i++)
    {
        delete m_maskedPictures[i];
        m_maskedPictures[i] = nullptr;
    }
    delete[] m_maskedPictures;
    delete m_maskedPictureTable;

    for (uint16_t i = 0; i < m_spriteTable->GetCount(); i++)
    {
        delete m_sprites[i];
        m_sprites[i] = nullptr;
    }
    delete[] m_sprites;
    delete m_spriteTable;

    const uint16_t numberOfWorldLocationNames = m_staticData.indexOfLastWorldLocationNames - m_staticData.indexOfFirstWorldLocationNames + 1;
    for (uint16_t i = 0; i < numberOfWorldLocationNames; i++)
    {
        delete m_worldLocationNames[i];
        m_worldLocationNames[i] = nullptr;
    }
    delete[] m_worldLocationNames;

    const uint16_t numFonts = m_staticData.indexOfFirstPicture - 3;
    for (uint16_t i = 0; i < numFonts; i++)
    {
        delete m_fonts[i];
        m_fonts[i] = nullptr;

    }
    delete[] m_fonts;

    if (m_tilesSize8 != nullptr)
    {
        for (uint16_t i = 0; i < numTilesSize8; i++)
        {
            delete m_tilesSize8[i];
            m_tilesSize8[i] = nullptr;
        }
        delete[] m_tilesSize8;
    }

    if (m_tilesSize8Masked != nullptr)
    {
        for (uint16_t i = 0; i < numTilesSize8Masked; i++)
        {
            delete m_tilesSize8Masked[i];
            m_tilesSize8Masked[i] = nullptr;
        }
        delete m_tilesSize8Masked;
    }

    delete m_rawData;
    delete m_huffman;
}

Picture* EgaGraph::GetPicture(const uint16_t index)
{
    const uint16_t pictureIndex = index - m_staticData.indexOfFirstPicture;
    if (pictureIndex >= m_pictureTable->GetCount())
    {
        return nullptr;
    }

    if (m_pictures[pictureIndex] == nullptr)
    {
        const bool transparent = ((index > m_staticData.indexOfFirstScaledPicture) && (index < m_staticData.indexOfFirstWallPicture));
        uint8_t* compressedPicture = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index)];
        uint32_t compressedSize = GetChunkSize(index) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedPicture;
        FileChunk* pictureChunk = m_huffman->Decompress(&compressedPicture[sizeof(uint32_t)], compressedSize, uncompressedSize);
        const unsigned int textureId = m_renderer.LoadFileChunkIntoTexture(pictureChunk, m_pictureTable->GetWidth(pictureIndex), m_pictureTable->GetHeight(pictureIndex), transparent);
        m_pictures[pictureIndex] = new Picture(textureId, m_pictureTable->GetWidth(pictureIndex), m_pictureTable->GetHeight(pictureIndex));
        delete pictureChunk;
    }

    return m_pictures[pictureIndex]; 
}

Picture* EgaGraph::GetMaskedPicture(const uint16_t index)
{
    const uint16_t pictureIndex = index - m_staticData.indexOfFirstMaskedPicture;
    if (pictureIndex >= m_maskedPictureTable->GetCount())
    {
        return nullptr;
    }

    if (m_maskedPictures[pictureIndex] == nullptr)
    {
        uint8_t* compressedPicture = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index)];
        uint32_t compressedSize = GetChunkSize(index) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedPicture;
        FileChunk* pictureChunk = m_huffman->Decompress(&compressedPicture[sizeof(uint32_t)], compressedSize, uncompressedSize);
        const uint16_t width = m_maskedPictureTable->GetWidth(pictureIndex);
        const uint16_t height = m_maskedPictureTable->GetHeight(pictureIndex);
        const unsigned int textureId = m_renderer.LoadMaskedFileChunkIntoTexture(pictureChunk, width, height);
        m_maskedPictures[pictureIndex] = new Picture(textureId, width, height);
        delete pictureChunk;
    }

    return m_maskedPictures[pictureIndex]; 
}

Picture* EgaGraph::GetSprite(const uint16_t index)
{
    const uint16_t pictureIndex = index - m_staticData.indexOfFirstSprite;
    if (pictureIndex >= m_spriteTable->GetCount())
    {
        return nullptr;
    }

    if (m_sprites[pictureIndex] == nullptr)
    {
        uint8_t* compressedPicture = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index)];
        uint32_t compressedSize = GetChunkSize(index) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedPicture;
        FileChunk* pictureChunk = m_huffman->Decompress(&compressedPicture[sizeof(uint32_t)], compressedSize, uncompressedSize);
        const unsigned int textureId = m_renderer.LoadMaskedFileChunkIntoTexture(pictureChunk, m_spriteTable->GetWidth(pictureIndex), m_spriteTable->GetHeight(pictureIndex));
        m_sprites[pictureIndex] = new Picture(textureId, m_spriteTable->GetWidth(pictureIndex), m_spriteTable->GetHeight(pictureIndex));
        delete pictureChunk;
    }

    return m_sprites[pictureIndex]; 
}

Picture* EgaGraph::GetTilesSize8(const uint16_t index)
{
    if (index >= numTilesSize8)
    {
        return nullptr;
    }

    if (m_tilesSize8[index] == nullptr)
    {
        const uint16_t pictureIndex = m_staticData.indexOfTileSize8;
        uint8_t* compressedPicture = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(pictureIndex)];
        uint32_t compressedSize = GetChunkSize(pictureIndex);
        uint32_t uncompressedSize = 32 * numTilesSize8;
        FileChunk* pictureChunk = m_huffman->Decompress(compressedPicture, compressedSize, uncompressedSize);

        // Just load all the tiles at once, such that the pictureChunk only needs to be decompressed once.
        for (uint16_t i = 0; i < numTilesSize8; i++)
        {
            const unsigned int textureId = m_renderer.LoadTilesSize8IntoTexture(pictureChunk, i, false);
            m_tilesSize8[i] = new Picture(textureId, 8, 8);
        }

        delete pictureChunk;
    }

    return m_tilesSize8[index];
}

Picture* EgaGraph::GetTilesSize8Masked(const uint16_t index)
{
    if (index >= numTilesSize8Masked)
    {
        return nullptr;
    }

    if (m_tilesSize8Masked[index] == nullptr)
    {
        const uint16_t pictureIndex = m_staticData.indexOfTileSize8Masked;
        uint8_t* compressedPicture = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(pictureIndex)];
        uint32_t compressedSize = GetChunkSize(pictureIndex);
        uint32_t uncompressedSize = 40 * numTilesSize8Masked;
        FileChunk* pictureChunk = m_huffman->Decompress(compressedPicture, compressedSize, uncompressedSize);

        // Just load all the tiles at once, such that the pictureChunk only needs to be decompressed once.
        for (uint16_t i = 0; i < numTilesSize8Masked; i++)
        {
            const unsigned int textureId = m_renderer.LoadTilesSize8IntoTexture(pictureChunk, i, true);
            m_tilesSize8Masked[i] = new Picture(textureId, 8, 8);
        }
        delete pictureChunk;
    }

    return m_tilesSize8Masked[index];
}

Font* EgaGraph::GetFont(const uint16_t index)
{
    const uint16_t numFonts = m_staticData.indexOfFirstPicture - 3;
    if (index < 3 || index >= m_staticData.indexOfFirstPicture)
    {
        Logging::Instance().FatalError("No font found at index " + std::to_string(index) + " in " + m_staticData.filename);
    }

    const uint16_t indexInFontArray = index - 3;
    if (m_fonts[indexInFontArray] != nullptr)
    {
        return m_fonts[indexInFontArray];
    }

    uint8_t* compressedFont = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index)];
    uint32_t compressedSize = GetChunkSize(index) - sizeof(uint32_t);
    uint32_t uncompressedSize = *(uint32_t*)compressedFont;
    FileChunk* fontChunk = m_huffman->Decompress(&compressedFont[sizeof(uint32_t)], compressedSize, uncompressedSize);

    const uint16_t NumChar = 256;

    const uint16_t lineHeight = *(uint16_t*)&fontChunk->GetChunk()[0];
    uint16_t characterOffset[NumChar];
    for (uint16_t i = 0; i < NumChar; i++)
    {
        characterOffset[i] = *(uint16_t*)&fontChunk->GetChunk()[2 + (i * 2)];
    }
    uint8_t width[NumChar];
    for (uint16_t i = 0; i < 256; i++)
    {
        width[i] = *(uint8_t*)&fontChunk->GetChunk()[514 + i];
    }

    bool fontPicture[16 * 256 * 10];
    memset(&fontPicture, 0, 16 * 256 * 10);
    for (int i = 0; i < 256; i++)
    {
        if (characterOffset[i] == 0)
        {
            continue;
        }

        const uint8_t sourceLength = (width[i] < 9) ? 1 : 2;
        for (int y = 0; y < lineHeight; y++)
        {
            for (int x = 0; x < sourceLength; x++)
            {
                uint8_t sourceByte = *(uint8_t*)&fontChunk->GetChunk()[characterOffset[i] + (y * sourceLength) + x];
                uint16_t destinationY = (uint16_t)(i * 10) + y;
                uint16_t destinationX = (uint16_t)(x * 8);
                for (uint8_t b = 0; b < 8; b++)
                {
                    fontPicture[destinationX + (destinationY * 16) + 7 - b] = ((sourceByte & (1 << b)) > 0);
                }
            }
        }
    }

    unsigned int textureId = m_renderer.LoadFontIntoTexture(fontPicture);
    m_fonts[indexInFontArray] = new Font(lineHeight, width, textureId);
    delete fontChunk;

    return m_fonts[indexInFontArray];
}

LevelLocationNames* EgaGraph::GetWorldLocationNames(const uint16_t index)
{
    if (m_worldLocationNames[index] == nullptr)
    {
        uint8_t* compressedLocationNames = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index + m_staticData.indexOfFirstWorldLocationNames)];
        uint32_t compressedSize = GetChunkSize(m_staticData.indexOfFirstWorldLocationNames + index) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedLocationNames;
        FileChunk* locationNamesChunk = m_huffman->Decompress(&compressedLocationNames[sizeof(uint32_t)], compressedSize, uncompressedSize);
        m_worldLocationNames[index] = new LevelLocationNames(locationNamesChunk);
        delete locationNamesChunk;
    }

    return m_worldLocationNames[index];
}

uint16_t EgaGraph::GetNumberOfWorldLocationNames() const
{
    return m_staticData.indexOfLastWorldLocationNames - m_staticData.indexOfFirstWorldLocationNames + 1;
}

uint32_t EgaGraph::GetChunkSize(const uint16_t index)
{
    if (index >= m_staticData.offsets.size())
    {
        return 0;
    }

    int32_t pos = m_staticData.offsets.at(index);
    if (pos<0)
    {
        return 0;
    }

    uint16_t next = index + 1;
    while (m_staticData.offsets.at(index) == -1)		// skip past any sparse tiles
    {
        next++;
    }

    return m_staticData.offsets.at(next) - pos;
}

uint16_t EgaGraph::GetHandPictureIndex() const
{
    return m_staticData.indexOfHandPicture;
}