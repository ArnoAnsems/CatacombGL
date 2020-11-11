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
// OpenGLTextures
//
// Implements OpenGL texture functionality.
//
#pragma once

#include <stdint.h>

class OpenGLTextures
{
public:
    static const unsigned int GL_TEXTURE_2D = 0x0DE1;
    static const unsigned int GL_TEXTURE_MAG_FILTER = 0x2800;
    static const unsigned int GL_TEXTURE_MIN_FILTER = 0x2801;
    static const unsigned int GL_TEXTURE_WRAP_S = 0x2802;
    static const unsigned int GL_TEXTURE_WRAP_T = 0x2803;
    static const unsigned int GL_UNSIGNED_BYTE = 0x1401;
    static const unsigned int GL_FLOAT = 0x1406;
    static const unsigned int GL_DEPTH_COMPONENT = 0x1902;
    static const unsigned int GL_RGBA = 0x1908;
    static const unsigned int GL_NEAREST = 0x2600;
    static const unsigned int GL_CLAMP = 0x2900;

    OpenGLTextures();
    ~OpenGLTextures();

    bool IsSupported() const;

    void GlGenTextures(unsigned int n, unsigned int* textures) const;
    void GlBindTexture(unsigned int target, unsigned int texture) const;
    void GlTexParameteri(unsigned int target, unsigned int pname, int param) const;
    void GlTexImage2D(
        unsigned int target,
        int level,
        int internalformat,
        unsigned int width,
        unsigned int height,
        int border,
        unsigned int format,
        unsigned int type,
        void* pixels) const;

private:
    typedef void(__cdecl* GL_GenTextures_Func)(unsigned int, unsigned int*);
    typedef void(__cdecl* GL_BindTexture_Func)(unsigned int, unsigned int);
    typedef void(__cdecl* GL_TexParameteri_Func)(unsigned int, unsigned int, int);
    typedef void(__cdecl* GL_TexImage2D_Func)(unsigned int, int, int, unsigned int, unsigned int, int, unsigned int, unsigned int, void*);

    GL_GenTextures_Func m_genTexturesFuncPtr;
    GL_BindTexture_Func m_bindTextureFuncPtr;
    GL_TexParameteri_Func m_texParameteriFuncPtr;
    GL_TexImage2D_Func m_texImage2DFuncPtr;

    bool m_isSupported;
};

