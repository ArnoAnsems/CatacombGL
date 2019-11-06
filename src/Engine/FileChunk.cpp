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

#include "stdlib.h"
#include "FileChunk.h"

FileChunk::FileChunk(const uint32_t size)
{
    m_size = size;
    m_chunk = new unsigned char[size];
}

FileChunk::~FileChunk()
{
    delete[] m_chunk;
    m_chunk = nullptr;
}

uint32_t FileChunk::GetSize() const
{
    return m_size;
}

uint8_t* FileChunk::GetChunk() const
{
    return m_chunk;
}