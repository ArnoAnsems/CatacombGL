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
// Huffman
//
// Offers Huffman decompression.
//
#pragma once

#include "FileChunk.h"

typedef struct huffmanNode
{
    unsigned short bit0,bit1;
} huffmanNode;

typedef huffmanNode huffmanTable[256];

class Huffman
{
public:
    Huffman(const huffmanTable table);
    ~Huffman();

    FileChunk* Decompress(unsigned char* compressedChunk, const unsigned long compressedSize, const unsigned long decompressedSize);

private:
    huffmanTable m_table;
};

