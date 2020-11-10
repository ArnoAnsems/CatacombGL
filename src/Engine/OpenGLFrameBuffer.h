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

//
// OpenGLFrameBuffer
//
// Implements an OpenGL frame buffer object.
//
#pragma once

#include <stdint.h>

class OpenGLFrameBuffer
{
public:
    OpenGLFrameBuffer();
    ~OpenGLFrameBuffer();

    bool IsSupported() const;

    void SetDimensions(const uint16_t width, const uint16_t height);
    void Bind();
    void Unbind();
    unsigned int GetTextureId() const;

private:
    typedef void (__cdecl* GL_GenFrameBuffers_Func)(int, unsigned int*);
    typedef void (__cdecl* GL_BindFramebuffer_Func)(unsigned int, unsigned int);
    typedef void (__cdecl* GL_FrameBufferTexture2D_Func)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

    GL_GenFrameBuffers_Func m_genFrameBuffersFuncPtr;
    GL_BindFramebuffer_Func m_bindFrameBufferFuncPtr;
    GL_FrameBufferTexture2D_Func m_frameBufferTexture2DFuncPtr;

    bool m_isSupported;
    unsigned int m_textureIdColor;
    unsigned int m_textureIdDepth;
};
