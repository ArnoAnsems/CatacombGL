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
// OpenGLBasic
//
// Wrapper for basic OpenGL v1.0 functionality.
//
#pragma once

#include "Macros.h"
#include <cstdint>
#include <GL/gl.h>

class OpenGLBasic
{
public:
    OpenGLBasic();
    ~OpenGLBasic();

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
    typedef void(CALLBACK *GL_GenTextures_Func)(unsigned int, unsigned int*);
    typedef void(CALLBACK *GL_BindTexture_Func)(unsigned int, unsigned int);
    typedef void(CALLBACK *GL_TexParameteri_Func)(unsigned int, unsigned int, int);
    typedef void(CALLBACK *GL_TexImage2D_Func)(unsigned int, int, int, unsigned int, unsigned int, int, unsigned int, unsigned int, void*);

    GL_GenTextures_Func m_genTexturesFuncPtr;
    GL_BindTexture_Func m_bindTextureFuncPtr;
    GL_TexParameteri_Func m_texParameteriFuncPtr;
    GL_TexImage2D_Func m_texImage2DFuncPtr;

    bool m_isSupported;
};

