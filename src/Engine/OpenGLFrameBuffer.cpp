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
#include "../Engine/Logging.h"
#include <GL/glext.h>
#include <SDL_video.h>

OpenGLFrameBuffer::OpenGLFrameBuffer(const OpenGLBasic& openGLBasic) :
    m_openGLBasic(openGLBasic),
    m_genFrameBuffersFuncPtr(nullptr),
    m_bindFrameBufferFuncPtr(nullptr),
    m_frameBufferTexture2DFuncPtr(nullptr),
    m_checkFrameBufferStatusFuncPtr(nullptr),
    m_isSupported(false),
    m_frameBufferObject(0),
    m_textureIdColor(0),
    m_textureIdDepth(0),
    m_bufferWidth(320),
    m_bufferHeight(200)
{
    // glGenFramebuffers requires OpenGL 3.0
    m_genFrameBuffersFuncPtr = (GL_GenFrameBuffers_Func)SDL_GL_GetProcAddress("glGenFramebuffers");
    if (m_genFrameBuffersFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glGenFramebuffers");
        return;
    }

    // glBindFramebuffer requires OpenGL 3.0
    m_bindFrameBufferFuncPtr = (GL_BindFramebuffer_Func)SDL_GL_GetProcAddress("glBindFramebuffer");
    if (m_bindFrameBufferFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glBindFramebuffer");
        return;
    }

    // glFramebufferTexture2D requires OpenGL 3.0
    m_frameBufferTexture2DFuncPtr = (GL_FrameBufferTexture2D_Func)SDL_GL_GetProcAddress("glFramebufferTexture2D");
    if (m_frameBufferTexture2DFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glFrameBufferTexture2D_ptr");
        return;
    }

    // glCheckFramebufferStatus requires OpenGL 3.0
    m_checkFrameBufferStatusFuncPtr = (GL_CheckFrameBufferStatus_Func)SDL_GL_GetProcAddress("glCheckFramebufferStatus");
    if (m_checkFrameBufferStatusFuncPtr == nullptr)
    {
        Logging::Instance().AddLogMessage("Failed to obtain function pointer for glCheckFramebufferStatus");
        return;
    }

    m_genFrameBuffersFuncPtr(1, &m_frameBufferObject);

    m_openGLBasic.GlGenTextures(1, &m_textureIdColor);
    m_openGLBasic.GlGenTextures(1, &m_textureIdDepth);

    ResizeBuffer(m_bufferWidth, m_bufferHeight);

    const unsigned int frameBufferStatus = m_checkFrameBufferStatusFuncPtr(GL_DRAW_FRAMEBUFFER);
    if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        const std::string statusString = FrameBufferStatusToString(frameBufferStatus);
        const std::string errorString =
            "Framebuffer not complete; glCheckFramebufferStatus returned " +
            std::to_string(frameBufferStatus) + " (" + statusString + ")";
        Logging::Instance().AddLogMessage(errorString);
        return;
    }

    Logging::Instance().AddLogMessage("OpenGL frame buffer is supported");
    m_isSupported = true;
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{

}

bool OpenGLFrameBuffer::IsSupported() const
{
    return m_isSupported;
}

void OpenGLFrameBuffer::ResizeBuffer(const uint16_t width, const uint16_t height)
{
    m_bindFrameBufferFuncPtr(GL_DRAW_FRAMEBUFFER, m_frameBufferObject);

    m_openGLBasic.GlBindTexture(GL_TEXTURE_2D, m_textureIdColor);

    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    m_openGLBasic.GlTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        width,
        height,
        0, GL_RGBA, GL_UNSIGNED_BYTE,
        NULL);

    m_openGLBasic.GlBindTexture(GL_TEXTURE_2D, m_textureIdDepth);

    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    m_openGLBasic.GlTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    m_openGLBasic.GlTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        width,
        height,
        0, GL_DEPTH_COMPONENT, GL_FLOAT,
        NULL);

    m_openGLBasic.GlBindTexture(GL_TEXTURE_2D, 0);

    m_frameBufferTexture2DFuncPtr(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureIdColor, 0);
    m_frameBufferTexture2DFuncPtr(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureIdDepth, 0);

    m_bindFrameBufferFuncPtr(GL_DRAW_FRAMEBUFFER, 0);

    m_bufferWidth = width;
    m_bufferHeight = height;
}

void OpenGLFrameBuffer::Bind(const uint16_t width, const uint16_t height)
{
    if (width != m_bufferWidth || height != m_bufferHeight)
    {
        ResizeBuffer(width, height);
    }
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

const std::string OpenGLFrameBuffer::FrameBufferStatusToString(const unsigned int status)
{
    const std::string str =
        (status == GL_FRAMEBUFFER_COMPLETE) ? "GL_FRAMEBUFFER_COMPLETE" :
        (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) ? "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" :
        (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) ? "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" :
        (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) ? "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" :
        (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) ? "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" :
        (status == GL_FRAMEBUFFER_UNSUPPORTED) ? "GL_FRAMEBUFFER_UNSUPPORTED" :
        "Unknown";

    return str;
}
