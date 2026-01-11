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

#include "Picture.h"

Picture::Picture(const unsigned int textureId, const uint16_t imageWidth, const uint16_t imageHeight, const uint16_t textureWidth, const uint16_t textureHeight) :
    m_textureId(textureId),
    m_imageWidth(imageWidth),
    m_imageHeight(imageHeight),
    m_textureWidth(textureWidth),
    m_textureHeight(textureHeight)
{
}

Picture::~Picture()
{

}

unsigned int Picture::GetTextureId() const
{
    return m_textureId;
}

uint16_t Picture::GetImageWidth() const
{
    return m_imageWidth;
}

uint16_t Picture::GetImageHeight() const
{
    return m_imageHeight;
}

uint16_t Picture::GetTextureWidth() const
{
    return m_textureWidth;
}

uint16_t Picture::GetTextureHeight() const
{
    return m_textureHeight;
}
