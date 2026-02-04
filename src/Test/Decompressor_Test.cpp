// Copyright (C) 2026 Arno Ansems
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

#include <gtest/gtest.h>
#include "../Engine/Decompressor.h"

class Decompressor_Test : public ::testing::Test
{
protected:
    static void CheckChunkContainsExpectedData(
        const FileChunk& fileChunk, const uint16_t* expectedData, uint32_t expectedSize)
    {
        EXPECT_EQ(fileChunk.GetSize(), expectedSize * 2);
        const uint16_t* chunkData = reinterpret_cast<const uint16_t*>(fileChunk.GetChunk());
        for (uint32_t i = 0; i < expectedSize; i++)
        {
            EXPECT_EQ(chunkData[i], expectedData[i]);
        }
    }
};

TEST_F(Decompressor_Test, ReadUncompressedRLEWData)
{
    constexpr uint16_t compressedData[3] = { 0x1234, 0x5678, 0x90ab };
    constexpr uint16_t compressedDataSize = sizeof(compressedData);
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, 3u, 3u, 0xabcd);
    constexpr uint16_t expectedData[3] = {0x1234, 0x5678, 0x90ab};
    CheckChunkContainsExpectedData(*fileChunk, expectedData, 3u);
}

TEST_F(Decompressor_Test, ReadCompressedRLEWData)
{
    constexpr uint16_t compressedData[6] = { 0xabcd, 0x0004, 0x1234, 0xabcd, 0x0003, 0x5678 };
    constexpr uint16_t compressedDataSize = sizeof(compressedData);
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, 6u, 7u, 0xabcd);
    constexpr uint16_t expectedData[7] = { 0x1234, 0x1234, 0x1234, 0x1234, 0x5678, 0x5678, 0x5678 };
    CheckChunkContainsExpectedData(*fileChunk, expectedData, 7u);
}