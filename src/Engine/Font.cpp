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

#include "Font.h"
#include <cstring>

Font::Font(const uint8_t width[256], const TextureAtlas* const textureAtlas) :
    m_textureAtlas(textureAtlas)
{
    std::memcpy(&m_width[0], &width[0], sizeof(width[0]) * 256);
}

Font::~Font()
{
    delete m_textureAtlas;
}

const TextureAtlas* const Font::GetTextureAtlas() const
{
    return m_textureAtlas;
}

uint16_t Font::GetCharacterWidth(const uint16_t charIndex) const
{
    return m_width[charIndex];
}
