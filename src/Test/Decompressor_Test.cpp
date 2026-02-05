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

TEST_F(Decompressor_Test, WhenDecompressingAndRLEWDataIsUncompressedThenDataIsUnchanged)
{
    constexpr uint16_t compressedSizeInWords = 3u;
    constexpr uint16_t compressedData[compressedSizeInWords] = { 0x1234, 0x5678, 0x90ab };
    constexpr uint16_t decompressedSizeInWords = compressedSizeInWords;
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, compressedSizeInWords, decompressedSizeInWords, 0xabcd);
    constexpr uint16_t expectedData[decompressedSizeInWords] = {0x1234, 0x5678, 0x90ab};
    CheckChunkContainsExpectedData(*fileChunk, expectedData, decompressedSizeInWords);
}

TEST_F(Decompressor_Test, WhenDecompressingAndRLEWDataIsCompressedThenDataIsDecompressed)
{
    constexpr uint16_t compressedSizeInWords = 6u;
    constexpr uint16_t compressedData[compressedSizeInWords] = { 0xabcd, 0x0004, 0x1234, 0xabcd, 0x0003, 0x5678 };
    constexpr uint16_t decompressedSizeInWords = 7u;
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, compressedSizeInWords, decompressedSizeInWords, 0xabcd);
    constexpr uint16_t expectedData[decompressedSizeInWords] = { 0x1234, 0x1234, 0x1234, 0x1234, 0x5678, 0x5678, 0x5678 };
    CheckChunkContainsExpectedData(*fileChunk, expectedData, decompressedSizeInWords);
}

TEST_F(Decompressor_Test, WhenDecompressingAndRLEWDataIsSpecialTagThenDataIsSpecialTag)
{
    constexpr uint16_t compressedSizeInWords = 3u;
    constexpr uint16_t compressedData[compressedSizeInWords] = { 0xabcd, 0x0001, 0xabcd };
    constexpr uint16_t decompressedSizeInWords = 1u;
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, compressedSizeInWords, decompressedSizeInWords, 0xabcd);
    constexpr uint16_t expectedData[decompressedSizeInWords] = { 0xabcd };
    CheckChunkContainsExpectedData(*fileChunk, expectedData, decompressedSizeInWords);
}

TEST_F(Decompressor_Test, WhenDecompressingAndRLEWDataDoesNotFillUpDecompressedSizeThenDataIsFitted)
{
    constexpr uint16_t compressedSizeInWords = 4u;
    constexpr uint16_t compressedData[compressedSizeInWords] = { 0x5678, 0xabcd, 0x0002, 0x1234 };
    constexpr uint16_t decompressedSizeInWords = 10u;
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, compressedSizeInWords, decompressedSizeInWords, 0xabcd);
    constexpr uint16_t expectedSizeInWords = 3u;
    constexpr uint16_t expectedData[expectedSizeInWords] = { 0x5678, 0x1234, 0x1234 };
    CheckChunkContainsExpectedData(*fileChunk, expectedData, expectedSizeInWords);
}

TEST_F(Decompressor_Test, WhenDecompressingAndRLEWDataIsUncompressedAndDoesNotFitDecompressedSizeThenDataIsTruncated)
{
    constexpr uint16_t compressedSizeInWords = 4u;
    constexpr uint16_t compressedData[compressedSizeInWords] = { 0x1234, 0x5678, 0x90ab, 0xcdef };
    constexpr uint16_t decompressedSizeInWords = 2u;
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, compressedSizeInWords, decompressedSizeInWords, 0xabcd);
    constexpr uint16_t expectedData[decompressedSizeInWords] = { 0x1234, 0x5678 };
    CheckChunkContainsExpectedData(*fileChunk, expectedData, decompressedSizeInWords);
}

TEST_F(Decompressor_Test, WhenDecompressingAndRLEWDataIsCompressedAndDoesNotFitDecompressedSizeThenDataIsTruncated)
{
    constexpr uint16_t compressedSizeInWords = 9u;
    constexpr uint16_t compressedData[compressedSizeInWords] = { 0xabcd, 0x0002, 0x1234, 0xabcd, 0x0003, 0x5678, 0xabcd, 0x0004, 0x90ab };
    constexpr uint16_t decompressedSizeInWords = 5u;
    const FileChunk* fileChunk = Decompressor::RLEW_Decompress(compressedData, compressedSizeInWords, decompressedSizeInWords, 0xabcd);
    constexpr uint16_t expectedData[decompressedSizeInWords] = { 0x1234, 0x1234, 0x5678, 0x5678, 0x5678 };
    CheckChunkContainsExpectedData(*fileChunk, expectedData, decompressedSizeInWords);
}