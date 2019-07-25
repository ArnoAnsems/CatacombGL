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
#include "FileChunk.h"
#include "Decompressor.h"
#include "string.h"
#include "stdio.h"
#include "IRenderer.h"

Shape::Shape(IRenderer& renderer) :
    m_renderer (renderer)
{
    m_offsetX = 0;
    m_offsetY = 0;
    m_picture = NULL;
}

Shape::~Shape()
{
    if (m_picture != NULL)
    {
        delete m_picture;
        m_picture = NULL;
    }
}

struct CMP1Header
{
    uint16_t CompType;
    uint32_t OrginalLen;			// Orginal FileLength of compressed Data.
    uint32_t CompressLen;			// Length of data after compression (A MUST for LZHUFF!)
};

FileChunk* ext_BLoad(const char *SourceFile)
{
    FILE* handle = NULL;

    FileChunk* SrcPtr = NULL;
    FileChunk* DstPtr = NULL;
    uint8_t Buffer[4];
    struct CMP1Header CompHeader;

    memset((void *)&CompHeader,0,sizeof(struct CMP1Header));

    //
    // Open file to load....
    //

    fopen_s(&handle, SourceFile, "rb");
    if (handle == NULL)
    {
        return NULL;
    }

    fread(Buffer, 1, 4, handle);

    if (strncmp((char *)Buffer,"CMP1",4) != 0)
    {
        fclose(handle);
        return NULL;
    }

    //
    // Compressed under new file format...
    //
    fread((void*)&CompHeader.CompType, sizeof(CompHeader.CompType), 1, handle);

    if (CompHeader.CompType != 2)   // 2 == LZH
    {
        fclose(handle);
        return NULL;
    }

    fread((void*)&CompHeader.OrginalLen, sizeof(CompHeader.OrginalLen), 1, handle);
    fread((void*)&CompHeader.CompressLen, sizeof(CompHeader.CompressLen), 1, handle);

    SrcPtr = new FileChunk(CompHeader.CompressLen);
    fread(SrcPtr->GetChunk(), 1, SrcPtr->GetSize(), handle);
    DstPtr = new FileChunk(CompHeader.OrginalLen);

    fclose(handle);

    Decompressor::lzhDecompress(SrcPtr->GetChunk(), DstPtr->GetChunk(), CompHeader.OrginalLen, CompHeader.CompressLen);

    delete SrcPtr;
    return(DstPtr);
}

struct BitMapHeader {
    uint16_t w,h,x,y;
    uint8_t	d,trans,comp,pad;
};


bool Shape::LoadFromFile(const char* filename)
{
#define CHUNK(Name) ( \
    (*ptr == *Name) &&			\
    (*(ptr+1) == *(Name+1)) &&	\
    (*(ptr+2) == *(Name+2)) &&	\
    (*(ptr+3) == *(Name+3)) \
    )

    uint8_t* data = NULL;
    int32_t dataSize = 0;
    uint16_t bytesPerRow = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t numberOfPlanes = 0;
    uint8_t compressed = 0;

    // Decompress to ram and return ptr to data and return len of data in
    //	passed variable...

    FileChunk* IFFfile = ext_BLoad(filename);
    if (!IFFfile)
    {
        return false;
    }

    // Evaluate the file
    //
    uint8_t *ptr = (uint8_t*)IFFfile->GetChunk();
    if (!CHUNK("FORM"))
        goto EXIT_FUNC;
    ptr += 4;

#define BE_Cross_Swap16(x) ((uint16_t)(((uint16_t)(x)<<8)|((uint16_t)(x)>>8)))
#define BE_Cross_Swap32(x) ((uint32_t)(((uint32_t)(x)<<24)|(((uint32_t)(x)<<8)&0x00FF0000)|(((uint32_t)(x)>>8)&0x0000FF00)|((uint32_t)(x)>>24)))

    uint32_t FileLen = BE_Cross_Swap32(*(uint32_t*)ptr);
    ptr += 4;

    if (!CHUNK("ILBM"))
        goto EXIT_FUNC;
    ptr += 4;

    while (FileLen != 0)
    {
        uint32_t ChunkLen = BE_Cross_Swap32(*(int32_t*)(ptr+4));
        ChunkLen = (ChunkLen+1) & 0xFFFFFFFE;

        if (CHUNK("BMHD"))
        {
            ptr += 8;
            width = BE_Cross_Swap16(((struct BitMapHeader*)ptr)->w);
            height = BE_Cross_Swap16(((struct BitMapHeader*)ptr)->h);
            m_offsetX = BE_Cross_Swap16(((struct BitMapHeader*)ptr)->x);
            m_offsetY = BE_Cross_Swap16(((struct BitMapHeader*)ptr)->y);

            numberOfPlanes = ((struct BitMapHeader*)ptr)->d;
            if (numberOfPlanes != 4)
            {
                Logging::Instance().FatalError("Failed to read shape " + std::string(filename) + ": number of planes is " + std::to_string(numberOfPlanes) + "; expected: 4");
            }

            const uint8_t transparent = ((struct BitMapHeader*)ptr)->trans;
            compressed = ((struct BitMapHeader*)ptr)->comp;
            const uint8_t pad = ((struct BitMapHeader*)ptr)->pad;

            ptr += ChunkLen;
        }
        else if (CHUNK("BODY"))
        {
            ptr += 4;
            dataSize = BE_Cross_Swap32(*((int32_t*)ptr));
            ptr += 4;

            if ((int32_t)FileLen < dataSize)
            {
                Logging::Instance().AddLogMessage("Failed to read shape " + std::string(filename) + ": body reports data size of " + std::to_string(dataSize) + " bytes, but only " + std::to_string(FileLen) + " bytes remaining in the file");
            }

            bytesPerRow = (width + 7) >> 3;
            data = new uint8_t[dataSize];
            if (!data)
                goto EXIT_FUNC;
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

EXIT_FUNC:;
    if (IFFfile)
    {
        delete IFFfile;
        IFFfile = NULL;
    }

    FileChunk* chunk = new FileChunk(bytesPerRow * numberOfPlanes * height);

	const bool NotWordAligned = bytesPerRow & 1;

    if (data == NULL)
    {
        Logging::Instance().AddLogMessage("Failed to read shape " + std::string(filename) + ": body not found");
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

    const unsigned int textureId = m_renderer.LoadFileChunkIntoTexture(chunk, bytesPerRow * numberOfPlanes * 2, height, false);
    delete chunk;

    m_picture = new Picture(textureId, width, height);

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