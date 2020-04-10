// Copyright (C) 2020 Arno Ansems
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

#include "TextureAtlas_Test.h"
#include "..\Engine\TextureAtlas.h"

TextureAtlas_Test::TextureAtlas_Test()
{

}

TextureAtlas_Test::~TextureAtlas_Test()
{

}

TEST(TextureAtlas_Test, GetImageRelativeOffsets)
{
    const unsigned int textureId = 0;
    const uint16_t imageWidth = 16;
    const uint16_t imageHeight = 10;
    const uint16_t numberOfColumns = 5;
    const uint16_t numberOfRows = 3;
    const uint16_t imageSpacingX = 3;
    const uint16_t imageSpacingY = 1;
    TextureAtlas atlas(textureId, imageWidth, imageHeight, numberOfColumns, numberOfRows, imageSpacingX, imageSpacingY);

    EXPECT_EQ(atlas.GetTextureWidth(), 128);
    EXPECT_EQ(atlas.GetTextureHeight(), 32);
    EXPECT_NE(atlas.GetTexturePixelData(), nullptr);

    EXPECT_FLOAT_EQ(atlas.GetImageRelativeWidth(), 0.125f);
    EXPECT_FLOAT_EQ(atlas.GetImageRelativeHeight(), 0.3125f);

    EXPECT_FLOAT_EQ(atlas.GetImageRelativeOffsetX(0), 0.0f);
    EXPECT_FLOAT_EQ(atlas.GetImageRelativeOffsetY(0), 0.0f);

    EXPECT_FLOAT_EQ(atlas.GetImageRelativeOffsetX(14), 0.59375f);
    EXPECT_FLOAT_EQ(atlas.GetImageRelativeOffsetY(14), 0.6875f);
}

TEST(TextureAtlas_Test, StoreImage)
{
    const unsigned int textureId = 0;
    const uint16_t imageWidth = 2;
    const uint16_t imageHeight = 2;
    const uint16_t numberOfColumns = 2;
    const uint16_t numberOfRows = 2;
    const uint16_t imageSpacingX = 3;
    const uint16_t imageSpacingY = 1;

    const uint8_t imagePixelData[16] =
    {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 0, 1, 2, 3, 4, 5
    };
    const uint8_t expectedTexturePixelData[256] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        8, 9, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 9, 0, 1, 2, 3, 4, 5, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    TextureAtlas atlas(textureId, imageWidth, imageHeight, numberOfColumns, numberOfRows, imageSpacingX, imageSpacingY);
    ASSERT_EQ(atlas.GetTextureWidth() * atlas.GetTextureHeight() * 4, 256);

    atlas.StoreImage(0, imagePixelData);
    atlas.StoreImage(3, imagePixelData);

    const uint8_t* texturePixelData = atlas.GetTexturePixelData();
    for (uint16_t p = 0; p < 256; p++)
    {
        EXPECT_EQ(texturePixelData[p], expectedTexturePixelData[p]);
    }
}