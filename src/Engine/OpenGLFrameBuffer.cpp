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

static const unsigned int GL_DRAW_FRAMEBUFFER = 0x8CA9;
static const unsigned int GL_COLOR_ATTACHMENT0 = 0x8CE0;
static const unsigned int GL_DEPTH_ATTACHMENT = 0x8D00;
static const unsigned int GL_FRAMEBUFFER = 0x8D40;

OpenGLFrameBuffer::OpenGLFrameBuffer(const OpenGLTextures& openGLTextures) :
    m_openGLTextures(openGLTextures),
    m_genFrameBuffersFuncPtr(nullptr),
    m_bindFrameBufferFuncPtr(nullptr),
    m_frameBufferTexture2DFuncPtr(nullptr),
    m_isSupported(false),
    m_frameBufferObject(0),
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
    m_genFrameBuffersFuncPtr(1, &m_frameBufferObject);

    m_bindFrameBufferFuncPtr(GL_DRAW_FRAMEBUFFER, m_frameBufferObject);

    m_openGLTextures.GlGenTextures(1, &m_textureIdColor);

    m_openGLTextures.GlBindTexture(OpenGLTextures::GL_TEXTURE_2D, m_textureIdColor);

    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_MAG_FILTER, OpenGLTextures::GL_NEAREST);
    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_MIN_FILTER, OpenGLTextures::GL_NEAREST);
    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_WRAP_S, OpenGLTextures::GL_CLAMP);
    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_WRAP_T, OpenGLTextures::GL_CLAMP);

    m_openGLTextures.GlTexImage2D(OpenGLTextures::GL_TEXTURE_2D, 0, OpenGLTextures::GL_RGBA,
        width,
        height,
        0, OpenGLTextures::GL_RGBA, OpenGLTextures::GL_UNSIGNED_BYTE,
        NULL);

    m_openGLTextures.GlGenTextures(1, &m_textureIdDepth);

    m_openGLTextures.GlBindTexture(OpenGLTextures::GL_TEXTURE_2D, m_textureIdDepth);

    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_MAG_FILTER, OpenGLTextures::GL_NEAREST);
    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_MIN_FILTER, OpenGLTextures::GL_NEAREST);
    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_WRAP_S, OpenGLTextures::GL_CLAMP);
    m_openGLTextures.GlTexParameteri(OpenGLTextures::GL_TEXTURE_2D, OpenGLTextures::GL_TEXTURE_WRAP_T, OpenGLTextures::GL_CLAMP);

    m_openGLTextures.GlTexImage2D(OpenGLTextures::GL_TEXTURE_2D, 0, OpenGLTextures::GL_DEPTH_COMPONENT,
        width,
        height,
        0, OpenGLTextures::GL_DEPTH_COMPONENT, OpenGLTextures::GL_FLOAT,
        NULL);

    m_openGLTextures.GlBindTexture(OpenGLTextures::GL_TEXTURE_2D, 0);

    m_frameBufferTexture2DFuncPtr(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, OpenGLTextures::GL_TEXTURE_2D, m_textureIdColor, 0);
    m_frameBufferTexture2DFuncPtr(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, OpenGLTextures::GL_TEXTURE_2D, m_textureIdDepth, 0);

    m_bindFrameBufferFuncPtr(GL_DRAW_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Bind()
{
    m_bindFrameBufferFuncPtr(GL_DRAW_FRAMEBUFFER, m_frameBufferObject);
}

void OpenGLFrameBuffer::Unbind()
{
    m_bindFrameBufferFuncPtr(GL_DRAW_FRAMEBUFFER, 0);
}

unsigned int OpenGLFrameBuffer::GetTextureId() const
{
    return m_textureIdColor;
}