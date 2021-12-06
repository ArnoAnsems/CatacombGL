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
// Decompressor
//
// Provides decompression routines required for interpreting the orginal game data files.
//
#pragma once

#include "FileChunk.h"

class Decompressor
{
public:
    static uint32_t lzhDecompress(uint8_t* infile, uint8_t* outfile, uint32_t OrginalLength, uint32_t CompressLength);
    static FileChunk* RLEW_Decompress(const uint8_t* compressedChunk, const uint16_t rlewtag);
    static FileChunk* RLEW_DecompressFromSavedGame(
        const uint8_t* compressedChunk,
        const uint16_t rlewtag,
        const uint16_t maxCompressedSize,
        uint16_t& compressedSize);
    static FileChunk* CarmackExpand (const uint8_t* compressedChunk);

private:

};

