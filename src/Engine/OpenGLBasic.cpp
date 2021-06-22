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

#include "OpenGLBasic.h"
#include "../../ThirdParty/SDL/include/SDL_video.h"
#include "../Engine/Logging.h"

OpenGLBasic::OpenGLBasic() :
    m_genTexturesFuncPtr(nullptr),
    m_bindTextureFuncPtr(nullptr),
    m_texParameteriFuncPtr(nullptr),
    m_texImage2DFuncPtr(nullptr),
    m_isSupported(true)
{
    m_genTexturesFuncPtr = (GL_GenTextures_Func)SDL_GL_GetProcAddress("glGenTextures");
    if (m_genTexturesFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glGenTextures");
        m_isSupported = false;
        return;
    }

    m_bindTextureFuncPtr = (GL_BindTexture_Func)SDL_GL_GetProcAddress("glBindTexture");
    if (m_bindTextureFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glBindTexture");
        m_isSupported = false;
        return;
    }

    m_texParameteriFuncPtr = (GL_TexParameteri_Func)SDL_GL_GetProcAddress("glTexParameteri");
    if (m_texParameteriFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glTexParameteri");
        m_isSupported = false;
        return;
    }

    m_texImage2DFuncPtr = (GL_TexImage2D_Func)SDL_GL_GetProcAddress("glTexImage2D");
    if (m_texImage2DFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glTexImage2D");
        m_isSupported = false;
        return;
    }
}

OpenGLBasic::~OpenGLBasic()
{

}

bool OpenGLBasic::IsSupported() const
{
    return m_isSupported;
}

void OpenGLBasic::GlGenTextures(unsigned int n, unsigned int* textures) const
{
    m_genTexturesFuncPtr(n, textures);
}

void OpenGLBasic::GlBindTexture(unsigned int target, unsigned int texture) const
{
    m_bindTextureFuncPtr(target, texture);
}

void OpenGLBasic::GlTexParameteri(unsigned int target, unsigned int pname, int param) const
{
    m_texParameteriFuncPtr(target, pname, param);
}

void OpenGLBasic::GlTexImage2D(
    unsigned int target,
    int level,
    int internalformat,
    unsigned int width,
    unsigned int height,
    int border,
    unsigned int format,
    unsigned int type,
    void* pixels) const
{
    m_texImage2DFuncPtr(target, level, internalformat, width, height, border, format, type, pixels);
}