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

#include "Huffman.h"

Huffman::Huffman(const huffmanTable table)
{
    for (int i = 0; i < 256; i++)
    {
        m_table[i].bit0 = table[i].bit0;
        m_table[i].bit1 = table[i].bit1;
    }
}

Huffman::~Huffman()
{

}

FileChunk* Huffman::Decompress(unsigned char* compressedChunk, const unsigned long compressedSize, const unsigned long decompressedSize)
{
    FileChunk* fileChunk = new FileChunk(decompressedSize);

    unsigned char* decompressedChunk = fileChunk->GetChunk();
    unsigned char huffindex = 254;
    unsigned long byteIndex = 0;
    unsigned long destIndex = 0;

    while (byteIndex < compressedSize && destIndex < decompressedSize)
    {
        unsigned char value = compressedChunk[byteIndex];

        unsigned short bitIndex = 0;
        while (bitIndex < 8 && destIndex < decompressedSize)
        {
            unsigned short huffValue = (value & (1 << bitIndex)) ? m_table[huffindex].bit1 : m_table[huffindex].bit0;
            if (huffValue < 256)
            {
                decompressedChunk[destIndex] = (unsigned char)huffValue;
                destIndex++;
                huffindex = 254;
            }
            else
            {
                huffindex = huffValue - 256;
            }

            bitIndex++;
        }
        byteIndex++;
    }

    return fileChunk;
}