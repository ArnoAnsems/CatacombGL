// Copyright (C) 1993 - 2014 Flat Rock Software
// Copyright (C) 2014 - 2018 NY00123
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

// Based on GELIB.C from the Catacomb Abyss source code, with modifications from the Reflection Keen source port.

#include "Shape.h"
#include "IRenderer.h"
#include "Decompressor.h"
#include "FileChunk.h"
#include "Uint16Utility.h"
#include "Uint32Utility.h"
#include <cstdint>
#include <cstring> // required for std::strncmp() and memcpy()
#include <fstream>
#include <ios>

Shape::Shape(const IRenderer& renderer) :
    m_renderer (renderer)
{
    m_offsetX = 0;
    m_offsetY = 0;
    m_picture = nullptr;
}

Shape::~Shape()
{
    delete m_picture;
    m_picture = nullptr;
}

static uint32_t GetFileSize(std::ifstream& file)
{
    const std::streampos currentPos = file.tellg();

    file.seekg(0, std::ios::end);
    const std::streampos endPos = file.tellg();
    const uint32_t fileSize = static_cast<uint32_t>(endPos);

    // Move the stream position back to the current position
    file.seekg(currentPos);

    return fileSize;
}

static FileChunk* DecompressDataFromFile(const std::filesystem::path& sourceFile)
{
    std::ifstream file(sourceFile, std::ios::binary);
    if (!file)
    {
        return nullptr;
    }

    char shapeType[4];
    file.read(shapeType, 4);

    // The shape can be one of two types: COMP or CMP1.
    const bool isComp = (std::strncmp(shapeType, "COMP", 4) == 0);
    const bool isCmp1 = (std::strncmp(shapeType, "CMP1", 4) == 0);
    if (!isComp && !isCmp1)
    {
        Logging::Instance().AddLogMessage("Unrecognized shape type: " + std::string(shapeType));
        file.close();
        return nullptr;
    }

    if (isCmp1)
    {
        uint16_t compType = 0u;
        file.read((char*)&compType, sizeof(compType));

        if (compType != 2)   // 2 == LZH
        {
            Logging::Instance().AddLogMessage("Unrecognized compression type: " + std::to_string(compType));
            file.close();
            return nullptr;
        }
    }

    uint32_t decompressedLength = 0u;
    file.read((char*)&decompressedLength, sizeof(decompressedLength));

    uint32_t compressedLength = 0u;
    if (isCmp1)
    {
        file.read((char*)&compressedLength, sizeof(compressedLength));
    }
    else
    {
        compressedLength = GetFileSize(file) - 8u;
    }

    FileChunk* compressedChunk = new FileChunk(compressedLength);
    file.read((char*)compressedChunk->GetChunk(), compressedChunk->GetSize());

    file.close();

    FileChunk* decompressedChunk = new FileChunk(decompressedLength);
    if (isCmp1)
    {
        Decompressor::lzhDecompress(compressedChunk->GetChunk(), decompressedChunk->GetChunk(), decompressedLength, compressedLength);
    }
    else
    {
        Decompressor::lzwDecompress(compressedChunk->GetChunk(), decompressedChunk->GetChunk(), decompressedLength, compressedLength);
    }

    delete compressedChunk;

    return(decompressedChunk);
}

struct BitMapHeader {
    uint16_t w,h,x,y;
    uint8_t	d,trans,comp,pad;
};

constexpr bool CmpChunk(const uint8_t* const Ptr, const char Name[4]) {
    return  Ptr[0] == Name[0] &&
            Ptr[1] == Name[1] &&
            Ptr[2] == Name[2] &&
            Ptr[3] == Name[3];
}

bool Shape::LoadFromFile(const std::filesystem::path filename)
{
    uint8_t* data = nullptr;
    std::size_t dataSize = 0;
    uint16_t bytesPerRow = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t numberOfPlanes = 0;
    uint8_t compressed = 0;

    // Decompress to ram and return ptr to data and return len of data in
    //	passed variable...

    FileChunk* decompressedShapeData = DecompressDataFromFile(filename);
    if (decompressedShapeData == nullptr)
    {
        return false;
    }

    // LAMBDA: Evaluate the file
    //
    [&](){

    uint8_t *ptr = (uint8_t*)decompressedShapeData->GetChunk();

    if (!CmpChunk(ptr, "FORM"))
        // LAMBDA RETURN
        return;
    ptr += 4;

    std::size_t FileLen = Uint32Utility::Swap(*(uint32_t*)ptr);
    ptr += 4;

    if (!CmpChunk(ptr, "ILBM"))
        // LAMBDA RETURN
        return;
    ptr += 4;

    while (FileLen != 0)
    {
        uint32_t ChunkLen = Uint32Utility::Swap(*(int32_t*)(ptr+4));
        ChunkLen = (ChunkLen+1) & 0xFFFFFFFE;

        if (CmpChunk(ptr, "BMHD"))
        {
            ptr += 8;
            width = Uint16Utility::Swap(((struct BitMapHeader*)ptr)->w);
            height = Uint16Utility::Swap(((struct BitMapHeader*)ptr)->h);
            m_offsetX = Uint16Utility::Swap(((struct BitMapHeader*)ptr)->x);
            m_offsetY = Uint16Utility::Swap(((struct BitMapHeader*)ptr)->y);

            numberOfPlanes = ((struct BitMapHeader*)ptr)->d;
            if (numberOfPlanes != 4)
            {
                Logging::Instance().FatalError("Failed to read shape " + filename.string() + ": number of planes is " + std::to_string(numberOfPlanes) + "; expected: 4");
            }

            const uint8_t transparent = ((struct BitMapHeader*)ptr)->trans;
            compressed = ((struct BitMapHeader*)ptr)->comp;
            const uint8_t pad = ((struct BitMapHeader*)ptr)->pad;

            ptr += ChunkLen;
        }
        else if (CmpChunk(ptr, "BODY"))
        {
            ptr += 4;
            dataSize = Uint32Utility::Swap(*((int32_t*)ptr));
            ptr += 4;

            if (FileLen < dataSize)
            {
                Logging::Instance().AddLogMessage("Failed to read shape " + filename.string() + ": body reports data size of " + std::to_string(dataSize) + " bytes, but only " + std::to_string(FileLen) + " bytes remaining in the file");
            }

            bytesPerRow = (width + 7) >> 3;
            data = new uint8_t[dataSize];
            if (!data)
                // LAMBDA RETURN
                return;
            memcpy(data,ptr,dataSize);
            ptr += ChunkLen;

            break;
        }
        else
        {
            ptr += ChunkLen + 8;
        }

        FileLen -= ChunkLen+8;
    }

    // LAMBDA END
    }();

    if (decompressedShapeData)
    {
        delete decompressedShapeData;
        decompressedShapeData = nullptr;
    }

    FileChunk* chunk = new FileChunk(bytesPerRow * numberOfPlanes * height);

	const bool NotWordAligned = bytesPerRow & 1;

    if (data == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to read shape " + filename.string() + ": body not found");
    }

	int8_t* Src = (int8_t *)(data);
    const uint32_t planeSize = bytesPerRow * height;
    for (int16_t currentRow = 0; currentRow < height; currentRow++)
	{
		for (int8_t currentPlane = 0; currentPlane < numberOfPlanes; currentPlane++)
		{
            uint16_t offset = 0;

			uint16_t remainingBytesInRow = ((bytesPerRow+1) >> 1) << 1;               // IGNORE WORD ALIGN
			while (remainingBytesInRow)
			{
                int8_t n;
				if (compressed)
                {
					n = *Src++;
                }
				else
                {
					n = remainingBytesInRow - 1;
                }

				if (n < 0)
				{
					if (n != -128)
					{
						n = (-n)+1;
						remainingBytesInRow -= n;
						const int8_t Rep = *Src++;
						if ((!remainingBytesInRow) && (NotWordAligned))   // IGNORE WORD ALIGN
                        {
							n--;
                        }

						while (n--)
                        {
                            chunk->GetChunk()[(planeSize * currentPlane) + (currentRow * bytesPerRow) + offset] = Rep;
                            offset++;
                        }
					}
					else
                    {
						remainingBytesInRow--;
                    }
				}
				else
				{
					n++;
					remainingBytesInRow -= n;
					if ((!remainingBytesInRow) && (NotWordAligned))     // IGNORE WORD ALIGN
						n--;

					while (n--)
                    {
                        chunk->GetChunk()[(planeSize * currentPlane) + (currentRow * bytesPerRow) + offset] = *Src++;
                        offset++;
                    }

					if ((!remainingBytesInRow) && (NotWordAligned))     // IGNORE WORD ALIGN
						Src++;
				}
			}
		}
	}

    delete[] data;

    const uint16_t imageWidth = bytesPerRow * numberOfPlanes * 2;
    const uint16_t textureWidth = Uint16Utility::NearestPowerOfTwo(imageWidth);
    const uint16_t textureHeight = Uint16Utility::NearestPowerOfTwo(height);
    const unsigned int textureId = LoadFileChunkIntoTexture(chunk, imageWidth, height, textureWidth, textureHeight, false);
    delete chunk;

    m_picture = new Picture(textureId, imageWidth, height, textureWidth, textureHeight);

    return true;
}

uint16_t Shape::GetOffsetX() const
{
    return m_offsetX;
}

uint16_t Shape::GetOffsetY() const
{
    return m_offsetY;
}

Picture* Shape::GetPicture() const
{
    return m_picture;
}

unsigned int Shape::LoadFileChunkIntoTexture(
    const FileChunk* decompressedChunk,
    const uint16_t imageWidth,
    const uint16_t imageHeight,
    const uint16_t textureWidth,
    const uint16_t textureHeight,
    const bool transparent)
{
    const uint32_t bytesPerOutputPixel = 4;
    const uint32_t numberOfPlanes = 4;
    const uint32_t planeSize = decompressedChunk->GetSize() / numberOfPlanes;
    const uint32_t numberOfEgaPixelsPerByte = 8;
    const uint32_t numberOfPixels = planeSize * numberOfEgaPixelsPerByte;
    const uint32_t textureImageSize = textureWidth * textureHeight * bytesPerOutputPixel;

    if (textureImageSize < imageWidth * imageHeight * bytesPerOutputPixel)
    {
        Logging::Instance().FatalError("Texture image of size " + std::to_string(textureImageSize) + " is too small to contain image of dimensions (" + std::to_string(imageWidth) + " x " + std::to_string(imageHeight) + std::to_string(bytesPerOutputPixel) + ")");
    }

    uint8_t* textureImage = new uint8_t[textureImageSize];

    // Clear the whole texture
    for (uint32_t i = 0; i < textureImageSize; i++)
    {
        textureImage[i] = 0;
    }

    unsigned char* chunk = decompressedChunk->GetChunk();

    for (uint32_t i = 0; i < planeSize; i++)
    {
        for (uint32_t j = 0; j < numberOfEgaPixelsPerByte; j++)
        {
            const unsigned char bitValue = (1 << j);
            const bool blueplane = ((chunk[i] & bitValue) > 0);
            const bool greenplane = ((chunk[i + planeSize] & bitValue) > 0);
            const bool redplane = ((chunk[i + (2 * planeSize)] & bitValue) > 0);
            const bool intensityplane = ((chunk[i + (3 * planeSize)] & bitValue) > 0);
            const egaColor colorIndex =
                (egaColor)((intensityplane ? EgaDarkGray : EgaBlack) +
                (redplane ? EgaRed : EgaBlack) +
                    (greenplane ? EgaGreen : EgaBlack) +
                    (blueplane ? EgaBlue : EgaBlack));
            const bool transparentPixel = transparent && (colorIndex == 5);
            const rgbColor outputColor = EgaToRgb(transparentPixel ? EgaBlack : colorIndex);

            const uint32_t outputImagePixelOffset = ((i * 8) + 7 - j);
            const uint32_t outputImagePixelX = outputImagePixelOffset % imageWidth;
            const uint32_t outputImagePixelY = outputImagePixelOffset / imageWidth;
            const uint32_t outputTextureOffset = ((outputImagePixelY * textureWidth) + outputImagePixelX) * bytesPerOutputPixel;
            textureImage[outputTextureOffset] = outputColor.red;
            textureImage[outputTextureOffset + 1] = outputColor.green;
            textureImage[outputTextureOffset + 2] = outputColor.blue;
            textureImage[outputTextureOffset + 3] = transparentPixel ? 0 : 255;
        }
    }

    const unsigned int textureId = m_renderer.GenerateTextureId();
    m_renderer.LoadPixelDataIntoTexture(textureWidth, textureHeight, textureImage, textureId);

    delete[] textureImage;

    return textureId;
}
