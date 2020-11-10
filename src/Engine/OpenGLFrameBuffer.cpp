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

#include "OpenGLFrameBuffer.h"
#include "..\..\ThirdParty\SDL\include\SDL_video.h"
#include "..\Engine\Logging.h"

OpenGLFrameBuffer::OpenGLFrameBuffer() :
    m_genFrameBuffersFuncPtr(nullptr),
    m_bindFrameBufferFuncPtr(nullptr),
    m_frameBufferTexture2DFuncPtr(nullptr),
    m_isSupported(false),
    m_textureIdColor(0),
    m_textureIdDepth(0)
{
    m_genFrameBuffersFuncPtr = (GL_GenFrameBuffers_Func)SDL_GL_GetProcAddress("glGenFramebuffers");
    if (m_genFrameBuffersFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glGenFramebuffers");
        return;
    }

    m_bindFrameBufferFuncPtr = (GL_BindFramebuffer_Func)SDL_GL_GetProcAddress("glBindFramebuffer");
    if (m_bindFrameBufferFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glBindFramebuffer");
        return;
    }

    m_frameBufferTexture2DFuncPtr = (GL_FrameBufferTexture2D_Func)SDL_GL_GetProcAddress("glFramebufferTexture2D");
    if (m_frameBufferTexture2DFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glFrameBufferTexture2D_ptr");
        return;
    }
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{

}

bool OpenGLFrameBuffer::IsSupported() const
{
    return m_isSupported;
}

void OpenGLFrameBuffer::SetDimensions(const uint16_t width, const uint16_t height)
{

}

void OpenGLFrameBuffer::Bind()
{

}

void OpenGLFrameBuffer::Unbind()
{

}

unsigned int OpenGLFrameBuffer::GetTextureId() const
{
    return m_textureIdColor;
}