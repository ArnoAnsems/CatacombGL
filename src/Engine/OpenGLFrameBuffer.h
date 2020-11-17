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

#include "OpenGLBasic.h"
#include <string>

class OpenGLFrameBuffer
{
public:
    OpenGLFrameBuffer(const OpenGLBasic& openGLBasic);
    ~OpenGLFrameBuffer();

    bool IsSupported() const;

    void Bind(const uint16_t width, const uint16_t height);
    void Unbind();
    unsigned int GetTextureId() const;

private:
    void ResizeBuffer(const uint16_t width, const uint16_t height);
    const std::string FrameBufferStatusToString(const unsigned int status);
    typedef void (__stdcall* GL_GenFrameBuffers_Func)(int, unsigned int*);
    typedef void (__stdcall* GL_BindFramebuffer_Func)(unsigned int, unsigned int);
    typedef void (__stdcall* GL_FrameBufferTexture2D_Func)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    typedef unsigned int (__stdcall* GL_CheckFrameBufferStatus_Func)(unsigned int);

    GL_GenFrameBuffers_Func m_genFrameBuffersFuncPtr;
    GL_BindFramebuffer_Func m_bindFrameBufferFuncPtr;
    GL_FrameBufferTexture2D_Func m_frameBufferTexture2DFuncPtr;
    GL_CheckFrameBufferStatus_Func m_checkFrameBufferStatusFuncPtr;

    const OpenGLBasic& m_openGLBasic;
    bool m_isSupported;
    unsigned int m_frameBufferObject;
    unsigned int m_textureIdColor;
    unsigned int m_textureIdDepth;
    uint16_t m_bufferWidth;
    uint16_t m_bufferHeight;
};
