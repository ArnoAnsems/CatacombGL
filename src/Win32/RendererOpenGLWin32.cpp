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

#include "RendererOpenGLWin32.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "..\..\ThirdParty\SDL\include\SDL_video.h"
#include "..\Engine\Logging.h"
#include "..\Engine\ViewPorts.h"

const float FloorZ = 2.2f;
const float CeilingZ = 1.0f;
const float PlayerZ = 1.6f;

constexpr GLint GL_CLAMP_TO_EDGE = 0x812F;

// Constructor
RendererOpenGLWin32::RendererOpenGLWin32() :
    m_windowWidth(800u),
    m_windowHeight(600u),
    m_textureFilter(GL_LINEAR),
    m_currentSwapInterval(-1),
    m_isVSyncSupported(false),
    m_graphicsApiVersion(""),
    m_graphicsAdapterVendor(""),
    m_graphicsAdapterModel(""),
    m_openGLTextures(),
    m_openGLFramebuffer(m_openGLTextures)
{
    memset(&m_singleColorTexture, 0, sizeof(m_singleColorTexture[0]) * EgaRange);
}

// Destructor
RendererOpenGLWin32::~RendererOpenGLWin32()
{
}

void RendererOpenGLWin32::Setup()
{
    for (egaColor color = EgaBlack; color < EgaRange; color = egaColor(color + 1))
    {
        m_singleColorTexture[color] = GenerateSingleColorTexture(color);
    }

    m_isVSyncSupported = (SDL_GL_SetSwapInterval(0) == 0);
    const std::string vSyncLogMessage = m_isVSyncSupported ? "VSync is supported" : "VSync is NOT supported";
    Logging::Instance().AddLogMessage(vSyncLogMessage);

    const char* glVersion = (const char*)glGetString(GL_VERSION);
    m_graphicsApiVersion = "OpenGL " + std::string(glVersion);

    const char* glVendor = (const char*)glGetString(GL_VENDOR);
    m_graphicsAdapterVendor = std::string(glVendor);

    const char* glRenderer = (const char*)glGetString(GL_RENDERER);
    m_graphicsAdapterModel = std::string(glRenderer);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glClearStencil(0);
}

void RendererOpenGLWin32::SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}

void RendererOpenGLWin32::SetFrameSettings(const FrameSettings& frameSettings)
{
    m_textureFilter = (frameSettings.textureFilter == Nearest) ? GL_NEAREST : GL_LINEAR;

    const int32_t requestedSwapInterval = (frameSettings.vSyncEnabled) ? 1 : 0;
    if (requestedSwapInterval != m_currentSwapInterval)
    {
        SDL_GL_SetSwapInterval(frameSettings.vSyncEnabled ? 1 : 0);
        m_currentSwapInterval = requestedSwapInterval;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
}

unsigned int RendererOpenGLWin32::GenerateTextureId() const
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    return textureId;
}

void RendererOpenGLWin32::LoadPixelDataIntoTexture(uint32_t width, uint32_t height, uint8_t* pixelData, unsigned int textureId) const
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    GLenum glError = glGetError();
    if (glError != GL_NO_ERROR)
    {
        Logging::Instance().FatalError("Error binding texture (id=" + std::to_string(textureId) + "): glBindTexture returned " + ErrorCodeToString(glError));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    glError = glGetError();
    if (glError != GL_NO_ERROR)
    {
        Logging::Instance().FatalError("Error loading pixel data into texture (id=" + std::to_string(textureId) + ";width=" + std::to_string(width) + ";height=" + std::to_string(height) + "): glTexImage2D returned " + ErrorCodeToString(glError));
    }

    // Do not wrap the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void RendererOpenGLWin32::BindTexture(unsigned int textureId) const
{
    // Select the texture from the picture
    glBindTexture(GL_TEXTURE_2D, textureId);
    const GLenum glError = glGetError();
    if (glError == GL_INVALID_VALUE)
    {
        Logging::Instance().FatalError("Error binding texture (id=" + std::to_string(textureId) + "): glBindTexture returned " + ErrorCodeToString(glError));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureFilter);
}

const std::string RendererOpenGLWin32::ErrorCodeToString(const GLenum errorCode)
{
    const std::string str =
        (errorCode == GL_NO_ERROR) ? "GL_NO_ERROR" :
        (errorCode == GL_INVALID_ENUM) ? "GL_INVALID_ENUM" :
        (errorCode == GL_INVALID_VALUE) ? "GL_INVALID_VALUE" :
        (errorCode == GL_INVALID_OPERATION) ? "GL_INVALID_OPERATION" :
        (errorCode == GL_STACK_OVERFLOW) ? "GL_STACK_OVERFLOW" :
        (errorCode == GL_STACK_UNDERFLOW) ? "GL_STACK_UNDERFLOW" :
        (errorCode == GL_OUT_OF_MEMORY) ? "GL_OUT_OF_MEMORY" :
        "unknown error";

    return str;
}

unsigned int RendererOpenGLWin32::GenerateSingleColorTexture(const egaColor color) const
{
    const uint32_t bytesPerOutputPixel = 4;
    const uint32_t width = 8;
    const uint32_t height = 8;
    GLubyte* textureImage = new GLubyte[width * height * bytesPerOutputPixel];

    const rgbColor outputColor = EgaToRgb(color);
    uint32_t outputPixelOffset = 0;
    for (uint32_t i = 0; i < width * height; i++)
    {
        textureImage[outputPixelOffset] = outputColor.red;
        textureImage[outputPixelOffset + 1] = outputColor.green;
        textureImage[outputPixelOffset + 2] = outputColor.blue;
        textureImage[outputPixelOffset + 3] = 255;
        outputPixelOffset += bytesPerOutputPixel;
    }

    const unsigned int textureId = GenerateTextureId();
    LoadPixelDataIntoTexture(width, height, textureImage, textureId);

    delete[] textureImage;

    return textureId;
}

void RendererOpenGLWin32::RenderText(const RenderableText& renderableText)
{
    const std::vector<RenderableText::renderableCharacter>& characters = renderableText.GetText();
    const Font& font = renderableText.GetFont();
    if (characters.empty())
    {
        // Nothing to render
        return;
    }

    // Select the texture from the picture
    const TextureAtlas* const textureAtlas = font.GetTextureAtlas();
    BindTexture(textureAtlas->GetTextureId());

    egaColor previousColor = EgaBlack;

    // Draw the texture as a quad
    glBegin(GL_QUADS);

    for (uint16_t chari = 0; chari < characters.size(); chari++)
    {
        const egaColor currentColor = characters.at(chari).color;
        if (chari == 0 || currentColor != previousColor)
        {
            const rgbColor colorInRGB = EgaToRgb(currentColor);
            glColor3f((float)(colorInRGB.red) / 256.0f, (float)(colorInRGB.green) / 256.0f, (float)(colorInRGB.blue) / 256.0f);
            previousColor = currentColor;
        }

        const uint8_t charIndex = (uint8_t)characters.at(chari).imageIndex;
        const int16_t offsetX = characters.at(chari).offsetX;
        const int16_t offsetY = characters.at(chari).offsetY;
        const uint16_t charWidth = font.GetCharacterWidth(charIndex);
        const uint16_t charHeight = textureAtlas->GetImageHeight();
        const float textureHeight = textureAtlas->GetImageRelativeHeight();
        const float textureWidth = textureAtlas->GetImageRelativeWidth() * ((float)charWidth / (float)textureAtlas->GetImageWidth());
        const float textureOffsetX = textureAtlas->GetImageRelativeOffsetX(charIndex);
        const float textureOffsetY = textureAtlas->GetImageRelativeOffsetY(charIndex);

        glTexCoord2f(textureOffsetX, textureOffsetY + textureHeight); glVertex2i(offsetX, offsetY + charHeight);
        glTexCoord2f(textureOffsetX + textureWidth, textureOffsetY + textureHeight); glVertex2i(offsetX + charWidth, offsetY + charHeight);
        glTexCoord2f(textureOffsetX + textureWidth, textureOffsetY); glVertex2i(offsetX + charWidth, offsetY);
        glTexCoord2f(textureOffsetX, textureOffsetY); glVertex2i(offsetX, offsetY);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
}

void RendererOpenGLWin32::Prepare2DRendering(const bool helpWindow)
{
    // Set the viewport to the entire window
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    // Make sure no color is set
    glColor3f(1.0f,1.0f,1.0f);

    // Make sure the depth test is disabled
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create a 2D orthographic projection matrix, such that it imitates the 320x200 EGA pixel matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ViewPorts::ViewPortRect2D rect = ViewPorts::GetOrtho2D(m_windowWidth, m_windowHeight, helpWindow);

    gluOrtho2D(rect.left, rect.right, rect.bottom, rect.top);

    glDisable(GL_LIGHTING);
}

void RendererOpenGLWin32::Unprepare2DRendering()
{
    glDisable(GL_BLEND);
}

void RendererOpenGLWin32::Render2DPicture(const Picture* picture, const int16_t offsetX, const int16_t offsetY)
{
    if (picture == nullptr)
    {
        // Nothing to render
        return;
    }

    // Select the texture from the picture
    BindTexture(picture->GetTextureId());

    // Draw the texture as a quad
    const GLint width = (uint16_t)picture->GetImageWidth();
    const GLint height = (uint16_t)picture->GetImageHeight();
    const float relativeImageWidth = (float)picture->GetImageWidth() / (float)picture->GetTextureWidth();
    const float relativeImageHeight = (float)picture->GetImageHeight() / (float)picture->GetTextureHeight();
    glBegin(GL_QUADS);
    glTexCoord2f(0, relativeImageHeight); glVertex2i(offsetX, offsetY + height);
    glTexCoord2f(relativeImageWidth, relativeImageHeight); glVertex2i(offsetX + width, offsetY + height);
    glTexCoord2f(relativeImageWidth, 0); glVertex2i(offsetX + width, offsetY);
    glTexCoord2f(0, 0); glVertex2i(offsetX, offsetY);
    glEnd();
}

void RendererOpenGLWin32::Render2DPictureSegment(const Picture* picture, const int16_t offsetX, const int16_t offsetY, const uint16_t segmentOffsetX, const uint16_t segmentOffsetY, const uint16_t segmentWidth, const uint16_t segmentHeight)
{
    if (picture == nullptr)
    {
        // Nothing to render
        return;
    }

    // Select the texture from the picture
    BindTexture(picture->GetTextureId());

    // Draw the texture as a quad
    const float textureWidth = (float)picture->GetTextureWidth();
    const float textureHeight = (float)picture->GetTextureHeight();
    glBegin(GL_QUADS);
    glTexCoord2f(segmentOffsetX / textureWidth, (segmentOffsetY + segmentHeight) / textureHeight); glVertex2i(offsetX, offsetY + segmentHeight);
    glTexCoord2f((segmentOffsetX + segmentWidth) / textureWidth, (segmentOffsetY + segmentHeight) / textureHeight); glVertex2i(offsetX + segmentWidth, offsetY + segmentHeight);
    glTexCoord2f((segmentOffsetX + segmentWidth) / textureWidth, segmentOffsetY / textureHeight); glVertex2i(offsetX + segmentWidth, offsetY);
    glTexCoord2f(segmentOffsetX / textureWidth, segmentOffsetY / textureHeight); glVertex2i(offsetX, offsetY);
    glEnd();
}

void RendererOpenGLWin32::Render2DBar(const int16_t x, const int16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex)
{
    BindTexture(m_singleColorTexture[colorIndex]);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex2i(x, y + height);
    glTexCoord2i(1, 1); glVertex2i(x + width, y + height);
    glTexCoord2i(1, 0); glVertex2i(x + width, y);
    glTexCoord2i(0, 0); glVertex2i(x, y);
    glEnd();
}

void RendererOpenGLWin32::RenderTiles(const RenderableTiles& renderableTiles)
{
    const TextureAtlas& textureAtlas = renderableTiles.GetTextureAtlas();
    const std::vector<RenderableTiles::RenderableTile>& tiles = renderableTiles.GetTiles();

    if (tiles.empty())
    {
        return;
    }

    const int16_t imageWidth = (int16_t)textureAtlas.GetImageWidth();
    const int16_t imageHeight = (int16_t)textureAtlas.GetImageWidth();
    const float imageRelWidth = textureAtlas.GetImageRelativeWidth();
    const float imageRelHeight = textureAtlas.GetImageRelativeHeight();

    BindTexture(textureAtlas.GetTextureId());

    glBegin(GL_QUADS);
    for (const RenderableTiles::RenderableTile& tile : tiles)
    {
        const int16_t offsetX = tile.offsetX;
        const int16_t offsetY = tile.offsetY;
        const float imageRelOffsetX = textureAtlas.GetImageRelativeOffsetX(tile.imageIndex);
        const float imageRelOffsetY = textureAtlas.GetImageRelativeOffsetY(tile.imageIndex);

        glTexCoord2f(imageRelOffsetX + imageRelWidth, imageRelOffsetY + imageRelHeight); glVertex2i(offsetX + imageWidth, offsetY + imageHeight);
        glTexCoord2f(imageRelOffsetX, imageRelOffsetY + imageRelHeight); glVertex2i(offsetX, offsetY + imageHeight);
        glTexCoord2f(imageRelOffsetX, imageRelOffsetY); glVertex2i(offsetX, offsetY);
        glTexCoord2f(imageRelOffsetX + imageRelWidth, imageRelOffsetY); glVertex2i(offsetX + imageWidth, offsetY);
    }
    glEnd();
}

void RendererOpenGLWin32::Render3DScene(const Renderable3DScene& renderable3DScene)
{
    if (renderable3DScene.GetOriginalScreenResolution())
    {
        const ViewPorts::ViewPortRect3D rect3D = renderable3DScene.GetOriginal3DViewArea();
        const uint16_t additionalMargin = GetAdditionalMarginDueToWideScreen(renderable3DScene.GetAspectRatio());
        const uint16_t bufferWidth = rect3D.width + (additionalMargin * 2);
        m_openGLFramebuffer.Bind(bufferWidth, rect3D.height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, bufferWidth, rect3D.height);

        glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing

        glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

        glCullFace(GL_FRONT);

        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glLoadIdentity();									// Reset The Projection Matrix

        // Calculate The Aspect Ratio Of The Window
        
        gluPerspective((double)renderable3DScene.GetFieldOfView(), (float)bufferWidth / ((float)rect3D.height * 1.2f), 0.1f, 100.0f);

        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(renderable3DScene.GetAngle(), 0.0f, 0.0f, -1.0f);
        glTranslatef(-renderable3DScene.GetOriginX(), -renderable3DScene.GetOriginY(), -PlayerZ);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glNormal3f(0.0f, 0.0f, -1.0f);

        if (renderable3DScene.GetDepthShading())
        {
            GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLfloat LightPosition[] = { renderable3DScene.GetOriginX(), renderable3DScene.GetOriginY(), -PlayerZ, 1.0f };
            glEnable(GL_LIGHTING);
            glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
            glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
            glEnable(GL_LIGHT1);
        }

        glShadeModel(GL_SMOOTH);

        Render3DTiles(renderable3DScene.Get3DTiles());
        Render3DWalls(renderable3DScene.GetWalls());
        RenderSprites(renderable3DScene.GetSprites());

        m_openGLFramebuffer.Unbind();

        Prepare2DRendering(false);

        // Select the texture from the picture
        BindTexture(m_openGLFramebuffer.GetTextureId());

        // Draw the texture as a quad
        const GLint width = bufferWidth;
        const GLint height = rect3D.height;
        const GLint offsetX = rect3D.left - (GLint)additionalMargin;
        const GLint offsetY = rect3D.bottom - rect3D.height;
        const float relativeImageWidth = 1.0f;
        const float relativeImageHeight = 1.0f;
        glBegin(GL_QUADS);
        glTexCoord2f(0, relativeImageHeight); glVertex2i(offsetX, offsetY);
        glTexCoord2f(relativeImageWidth, relativeImageHeight); glVertex2i(offsetX + width, offsetY);
        glTexCoord2f(relativeImageWidth, 0); glVertex2i(offsetX + width, offsetY + height);
        glTexCoord2f(0, 0); glVertex2i(offsetX, offsetY + height);
        glEnd();
    }
    else
    {
        ViewPorts::ViewPortRect3D rect = ViewPorts::Get3D(m_windowWidth, m_windowHeight, renderable3DScene.GetAspectRatio(), renderable3DScene.GetOriginal3DViewArea());

        glViewport(rect.left, rect.bottom, rect.width, rect.height);

        glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing

        glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

        glCullFace(GL_FRONT);

        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glLoadIdentity();									// Reset The Projection Matrix

        // Calculate The Aspect Ratio Of The Window
        gluPerspective((double)renderable3DScene.GetFieldOfView(), (float)rect.width / (float)rect.height, 0.1f, 100.0f);

        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(renderable3DScene.GetAngle(), 0.0f, 0.0f, -1.0f);
        glTranslatef(-renderable3DScene.GetOriginX(), -renderable3DScene.GetOriginY(), -PlayerZ);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glNormal3f(0.0f, 0.0f, -1.0f);

        if (renderable3DScene.GetDepthShading())
        {
            GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLfloat LightPosition[] = { renderable3DScene.GetOriginX(), renderable3DScene.GetOriginY(), -PlayerZ, 1.0f };
            glEnable(GL_LIGHTING);
            glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
            glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
            glEnable(GL_LIGHT1);
        }

        glShadeModel(GL_SMOOTH);

        Render3DTiles(renderable3DScene.Get3DTiles());
        Render3DWalls(renderable3DScene.GetWalls());
        RenderSprites(renderable3DScene.GetSprites());
    }
}

uint16_t RendererOpenGLWin32::GetAdditionalMarginDueToWideScreen(const float aspectRatio)
{
    ViewPorts::ViewPortRect2D rect = ViewPorts::GetOrtho2D(m_windowWidth, m_windowHeight, false);
    if (aspectRatio < (4.0f / 3.0f) + 0.0001f)
    {
        // Original aspect ratio
        return 0;
    }
    else
    {
        return (uint16_t)(rect.right - rect.left - 320.0f + 1.0f) / 2;
    }
}

void RendererOpenGLWin32::Render3DWalls(const Renderable3DWalls& walls)
{
    glEnable(GL_CULL_FACE);
    const std::map<unsigned int, std::vector<Renderable3DWalls::wallCoordinate>>& textureToWallsMap = walls.GetTextureToWallsMap();
    for (const std::pair<unsigned int, std::vector<Renderable3DWalls::wallCoordinate>>& textureToWalls : textureToWallsMap)
    {
        const unsigned int textureId = textureToWalls.first;
        // Select the texture from the picture
        BindTexture(textureId);

        // Draw the texture as a quad
        glBegin(GL_QUADS);
        for (const Renderable3DWalls::wallCoordinate& coordinate : textureToWalls.second)
        {
            glTexCoord2i(1, 1); glVertex3f((float)coordinate.x1, (float)coordinate.y1, FloorZ);
            glTexCoord2i(0, 1); glVertex3f((float)coordinate.x2, (float)coordinate.y2, FloorZ);
            glTexCoord2i(0, 0); glVertex3f((float)coordinate.x2, (float)coordinate.y2, CeilingZ);
            glTexCoord2i(1, 0); glVertex3f((float)coordinate.x1, (float)coordinate.y1, CeilingZ);
        }
        glEnd();
    }

    glDisable(GL_CULL_FACE);
}

void RendererOpenGLWin32::RenderSprites(const RenderableSprites& renderableSprites)
{
    const std::vector<RenderableSprites::RenderableSprite>& sprites = renderableSprites.GetSprites();
    if (sprites.empty())
    {
        // Nothing to render
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glPushMatrix();

    unsigned int previousTextureId = 0;

    for (size_t i = 0; i < sprites.size(); i++)
    {
        const Picture* picture = sprites.at(i).picture;
        const float offsetX = sprites.at(i).offsetX;
        const float offsetY = sprites.at(i).offsetY;
        const RenderableSprites::SpriteOrientation orientation = sprites.at(i).orientation;
        glMatrixMode(GL_MODELVIEW);						// Select The Projection Matrix
        glLoadIdentity();

        glTranslatef(offsetX, offsetY, 0.0f);
        const float angle =
            (orientation == RenderableSprites::RotatedTowardsPlayer) ? renderableSprites.GetAngle() :
            (orientation == RenderableSprites::Isometric) ? 135.0f :
            (orientation == RenderableSprites::AlongYAxis) ? 90.0f :
            0.0f;

        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        const GLfloat halfWidth = (float)(picture->GetImageWidth()) / 128.0f;
        const GLfloat topZ = CeilingZ + ((float)(picture->GetImageHeight()) / 64.0f) * (FloorZ - CeilingZ);

        // Select the texture from the picture
        const unsigned int textureId = picture->GetTextureId();
        if (textureId != previousTextureId || previousTextureId == 0)
        {
            BindTexture(textureId);
            previousTextureId = textureId;
        }

        // Sprites that face the player are a bit sunken into the floor
        const float zOffset = (orientation == RenderableSprites::RotatedTowardsPlayer) ? 0.0625f : 0.0f;

        // Draw the texture as a quad
        const float relativeImageWidth = (float)picture->GetImageWidth() / (float)picture->GetTextureWidth();
        const float relativeImageHeight = (float)picture->GetImageHeight() / (float)picture->GetTextureHeight();
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfWidth, 0.0f, CeilingZ + zOffset);
        glTexCoord2f(relativeImageWidth, 0.0f); glVertex3f(halfWidth, 0.0f, CeilingZ + zOffset);
        glTexCoord2f(relativeImageWidth, relativeImageHeight); glVertex3f(halfWidth, 0.0f, topZ + zOffset);
        glTexCoord2f(0.0f, relativeImageHeight); glVertex3f(-halfWidth, 0.0f, topZ + zOffset);
        glEnd();
    }

    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void RendererOpenGLWin32::Render3DTiles(const Renderable3DTiles& tiles)
{
    // Do not write into the depth buffer. This allows sprites to appear a bit sunken into the floor.
    glDepthMask(GL_FALSE);

    BindTexture(m_singleColorTexture[tiles.GetFloorColor()]);

    const std::vector<Renderable3DTiles::tileCoordinate>& tileCoordinates = tiles.GetTileCoordinates();

    glBegin(GL_QUADS);
    const float z = FloorZ;
    for (const Renderable3DTiles::tileCoordinate& tile : tileCoordinates)
    {
        const float tileX = (float)tile.x;
        const float tileY = (float)tile.y;
        glTexCoord2i(0, 1); glVertex3f(tileX + 1.0f, tileY, z);        // Bottom Left
        glTexCoord2i(1, 1); glVertex3f(tileX + 1.0f, tileY + 1.0f, z); // Bottom Right
        glTexCoord2i(1, 0); glVertex3f(tileX, tileY + 1.0f, z);        // Top Right
        glTexCoord2i(0, 0); glVertex3f(tileX, tileY, z);               // Top Left
    }
    glEnd();

    if (!tiles.IsOnlyFloor())
    {
        BindTexture(m_singleColorTexture[tiles.GetCeilingColor()]);

        glBegin(GL_QUADS);
        const std::vector<Renderable3DTiles::tileCoordinate>& tileCoordinates = tiles.GetTileCoordinates();
        const float z = CeilingZ;
        for (const Renderable3DTiles::tileCoordinate& tile : tileCoordinates)
        {
            const float tileX = (float)tile.x;
            const float tileY = (float)tile.y;
            glTexCoord2i(0, 1); glVertex3f(tileX + 1.0f, tileY, z);        // Bottom Left
            glTexCoord2i(1, 1); glVertex3f(tileX + 1.0f, tileY + 1.0f, z); // Bottom Right
            glTexCoord2i(1, 0); glVertex3f(tileX, tileY + 1.0f, z);        // Top Right
            glTexCoord2i(0, 0); glVertex3f(tileX, tileY, z);               // Top Left
        }
        glEnd();
    }

    glDepthMask(GL_TRUE);
}

void RendererOpenGLWin32::RenderAutoMapTopDown(const RenderableAutoMapTopDown& autoMapTopDown)
{
    const uint16_t wallsScaleFactor = autoMapTopDown.GetTileSize() / 16;
    const uint16_t textScaleFactor = 2;
    PrepareTopDownRendering(autoMapTopDown.GetAspectRatio(), autoMapTopDown.GetOriginal3DViewArea(), wallsScaleFactor);

    const uint16_t tileSize = autoMapTopDown.GetTileSize();
    RenderTopDownFloorTiles(autoMapTopDown.GetFloorTiles(), tileSize);
    RenderTopDownFloorTiles(autoMapTopDown.GetBorderTiles(), tileSize);

    // Draw walls and sprites
    for (const auto picturePair : autoMapTopDown.GetPictures())
    {
        // Select the texture from the picture
        const Picture* picture = picturePair.first;
        BindTexture(picture->GetTextureId());
        const GLint width = (uint16_t)picture->GetImageWidth();
        const GLint height = (uint16_t)picture->GetImageHeight();
        const float relativeImageWidth = (float)picture->GetImageWidth() / (float)picture->GetTextureWidth();
        const float relativeImageHeight = (float)picture->GetImageHeight() / (float)picture->GetTextureHeight();

        // Draw the texture as quads
        glBegin(GL_QUADS);

        for (const RenderableAutoMapTopDown::pictureCoordinate& coordinate : picturePair.second)
        {
            const int16_t offsetX = coordinate.x;
            const int16_t offsetY = coordinate.y;
            glTexCoord2f(0, relativeImageHeight); glVertex2i(offsetX, offsetY + height);
            glTexCoord2f(relativeImageWidth, relativeImageHeight); glVertex2i(offsetX + width, offsetY + height);
            glTexCoord2f(relativeImageWidth, 0); glVertex2i(offsetX + width, offsetY);
            glTexCoord2f(0, 0); glVertex2i(offsetX, offsetY);
        }
        glEnd();
    }
    RenderTiles(autoMapTopDown.GetTilesSize16());
    RenderTiles(autoMapTopDown.GetTilesSize16Masked());

    const int16_t border = tileSize / 4;
    const int16_t width = tileSize - (2 * border);
    for (const auto wallCapPair : autoMapTopDown.GetWallCaps())
    {
        BindTexture(m_singleColorTexture[wallCapPair.first]);
        glBegin(GL_QUADS);
        for (const RenderableAutoMapTopDown::pictureCoordinate& coordinate : wallCapPair.second)
        {
            const int16_t x = coordinate.x;
            const int16_t y = coordinate.y;
            glTexCoord2i(0, 1); glVertex2i(x, y + width);
            glTexCoord2i(1, 1); glVertex2i(x + width, y + width);
            glTexCoord2i(1, 0); glVertex2i(x + width, y);
            glTexCoord2i(0, 0); glVertex2i(x, y);
        }
        glEnd();
    }

    if (autoMapTopDown.GetTileSize() == 64)
    {
        BindTexture(m_singleColorTexture[EgaBrightYellow]);

        glPushMatrix();

        const float playerX = (autoMapTopDown.GetPlayerX() - autoMapTopDown.GetOriginX()) * 64.0f;
        const float playerY = (autoMapTopDown.GetPlayerY() - autoMapTopDown.GetOriginY()) * 64.0f;

        glTranslatef(playerX, playerY, CeilingZ);
        glRotatef(autoMapTopDown.GetPlayerAngle(), 0.0f, 0.0f, 1.0f);

        glBegin(GL_QUADS);
        glTexCoord2i(0, 1); glVertex2f(6.4f, 25.6f);
        glTexCoord2i(1, 1); glVertex2f(-6.4f, 25.6f);
        glTexCoord2i(1, 0); glVertex2f(-6.4f, 0.0f);
        glTexCoord2i(0, 0); glVertex2f(6.4f, 0.0f);
        glEnd();
        glBegin(GL_TRIANGLES);
        glTexCoord2i(0, 1); glVertex2f(25.6f, 0.0f);
        glTexCoord2i(1, 1); glVertex2f(0.0f, -25.6f);
        glTexCoord2i(1, 0); glVertex2f(-25.6f, 0.0f);
        glEnd();

        glPopMatrix();

        PrepareTopDownRendering(autoMapTopDown.GetAspectRatio(), autoMapTopDown.GetOriginal3DViewArea(), textScaleFactor);
    }

    RenderText(autoMapTopDown.GetText());
}

void RendererOpenGLWin32::RenderAutoMapIso(const RenderableAutoMapIso& autoMapIso)
{
    PrepareIsoRendering(
        autoMapIso.GetAspectRatio(),
        autoMapIso.GetOriginal3DViewArea(),
        autoMapIso.GetOriginX(),
        autoMapIso.GetOriginY());

    Render3DTiles(autoMapIso.GetFloorTiles());

    Render3DWalls(autoMapIso.GetWalls());

    RenderIsoWallCaps(autoMapIso.GetWallCaps());

    RenderSprites(autoMapIso.GetSprites());

    BindTexture(m_singleColorTexture[EgaBrightYellow]);

    const float playerX = autoMapIso.GetPlayerX();
    const float playerY = autoMapIso.GetPlayerY();

    glPushMatrix();

    glTranslatef(playerX, playerY, CeilingZ);
    glRotatef(autoMapIso.GetPlayerAngle(), 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3f(0.1f, 0.4f, 0.0f);
    glTexCoord2i(1, 1); glVertex3f(- 0.1f, 0.4f, 0.0f);
    glTexCoord2i(1, 0); glVertex3f(- 0.1f, 0.0f, 0.0f);
    glTexCoord2i(0, 0); glVertex3f(0.1f, 0.0f, 0.0f);
    glEnd();
    glBegin(GL_TRIANGLES);
    glTexCoord2i(0, 1); glVertex3f(0.4f, 0.0f, 0.0f);
    glTexCoord2i(1, 1); glVertex3f(0.0f, - 0.4f, 0.0f);
    glTexCoord2i(1, 0); glVertex3f(- 0.4f, 0.0f, 0.0f);
    glEnd();

    glPopMatrix();

    PrepareIsoRenderingText(
        autoMapIso.GetOriginX(),
        autoMapIso.GetOriginY());

    RenderText(autoMapIso.GetText());
}

bool RendererOpenGLWin32::IsVSyncSupported()
{
    return m_isVSyncSupported;
}

bool RendererOpenGLWin32::IsOriginalScreenResolutionSupported()
{
    return m_openGLFramebuffer.IsSupported();
}

void RendererOpenGLWin32::PrepareIsoRendering(const float aspectRatio, const ViewPorts::ViewPortRect3D original3DViewArea, const float originX, const float originY)
{
    ViewPorts::ViewPortRect3D rect = ViewPorts::Get3D(m_windowWidth, m_windowHeight, aspectRatio, original3DViewArea);

    glViewport(rect.left, rect.bottom, rect.width, rect.height);

    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing

    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

    glCullFace(GL_FRONT);

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    const double x = originX + 9.0;
    const double y = originY + 9.0;
    const double z = -1.0;
    // use this length so that camera is 1 unit away from origin
    const double dist = sqrt(1 / 3.0);
    glOrtho(-16.0f, 16.0f, -4.0f, 4.0f, -20.0f, 20.0f);
    gluLookAt(dist + x, dist + y, z - dist,  // position of camera
        x, y, z,   // where camera is pointing at
        0.0, 0.0, -1.0);  // which direction is up
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);
}

void RendererOpenGLWin32::PrepareIsoRenderingText(const float originX, const float originY)
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    const double x = (originX + 9) * 32;
    const double y = (originY + 9) * 32;
    const double z = -1.0;
    // use this length so that camera is 1 unit away from origin
    const double dist = sqrt(1 / 3.0);
    glOrtho(-512.0f, 512.0f, -128.0f, 128.0f, -640.0f, 640.0f);
    gluLookAt(dist + x, dist + y, z - dist,  // position of camera
        x, y, z,   // where camera is pointing at
        0.0, 0.0, -1.0);  // which direction is up
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RendererOpenGLWin32::PrepareTopDownRendering(const float aspectRatio, const ViewPorts::ViewPortRect3D original3DViewArea, const uint16_t scale)
{
    ViewPorts::ViewPortRect3D rect = ViewPorts::Get3D(m_windowWidth, m_windowHeight, aspectRatio, original3DViewArea);

    glViewport(rect.left, rect.bottom, rect.width, rect.height);

    // Make sure no color is set
    glColor3f(1.0f, 1.0f, 1.0f);

    // Make sure the depth test is disabled
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create a 2D orthographic projection matrix, such that it imitates the 320x200 EGA pixel matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ViewPorts::ViewPortRect2D rect2D = ViewPorts::GetOrtho2D(rect.width, m_windowHeight, false);

    const double statusBarHeight = ((rect2D.bottom - rect2D.top) * (200 - original3DViewArea.height) / 200.0);
    gluOrtho2D(rect2D.left * scale, rect2D.right * scale, (rect2D.bottom - statusBarHeight) * scale, rect2D.top * scale);

    glDisable(GL_LIGHTING);
}

void RendererOpenGLWin32::RenderIsoWallCaps(const std::map <egaColor, std::vector<RenderableAutoMapIso::quadCoordinates>>& wallCaps)
{
    glEnable(GL_CULL_FACE);

    for (const std::pair<egaColor, std::vector<RenderableAutoMapIso::quadCoordinates>>& wallCap : wallCaps)
    {
        const unsigned int textureId = m_singleColorTexture[wallCap.first];
        // Select the texture from the picture
        BindTexture(textureId);

        // Draw the texture as a quad
        glBegin(GL_QUADS);
        for (const RenderableAutoMapIso::quadCoordinates& coordinate : wallCap.second)
        {
            glTexCoord2i(1, 1); glVertex3f((float)coordinate.x1, (float)coordinate.y1, CeilingZ);
            glTexCoord2i(0, 1); glVertex3f((float)coordinate.x2, (float)coordinate.y2, CeilingZ);
            glTexCoord2i(0, 0); glVertex3f((float)coordinate.x3, (float)coordinate.y3, CeilingZ);
            glTexCoord2i(1, 0); glVertex3f((float)coordinate.x4, (float)coordinate.y4, CeilingZ);
        }
        glEnd();
    }

    glDisable(GL_CULL_FACE);
}

void RendererOpenGLWin32::RenderTopDownFloorTiles(const Renderable3DTiles& tiles, const uint16_t tileSize)
{
    const unsigned int textureId = m_singleColorTexture[tiles.GetFloorColor()];
    // Select the texture from the picture
    BindTexture(textureId);

    glBegin(GL_QUADS);
    const std::vector<Renderable3DTiles::tileCoordinate>& floorTiles = tiles.GetTileCoordinates();
    for (const Renderable3DTiles::tileCoordinate floorTile : floorTiles)
    {
        // Draw the texture as a quad
        glTexCoord2f(0.0f, 1.0f); glVertex2i(floorTile.x, floorTile.y + tileSize);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(floorTile.x + tileSize, floorTile.y + tileSize);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(floorTile.x + tileSize, floorTile.y);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(floorTile.x, floorTile.y);
        
    }
    glEnd();
}

Picture* RendererOpenGLWin32::GetScreenCapture(const unsigned int textureId)
{
    // Pixels are read as GL_RGBA. Although the alpha channel is stricly speaking not necessary,
    // some graphics adapters do not handle glReadPixels with GL_RGB correctly.
    uint8_t* rawPixelData = new uint8_t[(unsigned int)m_windowWidth * (unsigned int)m_windowHeight * 4u];
    glReadPixels(0, 0, m_windowWidth, m_windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, rawPixelData);

    const uint16_t textureWidth = Picture::GetNearestPowerOfTwo(m_windowWidth);
    const uint16_t textureHeight = Picture::GetNearestPowerOfTwo(m_windowHeight);

    uint8_t* texturePixelData = new uint8_t[(unsigned int)textureWidth * (unsigned int)textureHeight * 4u];

    // Flip pixels upside down
    for (uint16_t y = 0; y < m_windowHeight; y++)
    {
        for (uint16_t x = 0; x < m_windowWidth * 4; x++)
        {
            texturePixelData[(y * textureWidth * 4) + x] = rawPixelData[((m_windowHeight - 1 - y) * m_windowWidth * 4) + x];
        }
    }

    delete[] rawPixelData;

    GLuint newTextureId;
    if (textureId == 0)
    {
        newTextureId = GenerateTextureId();
    }
    else
    {
        newTextureId = textureId;
    }

    LoadPixelDataIntoTexture(textureWidth, textureHeight, texturePixelData, newTextureId);

    delete[] texturePixelData;

    glClear(GL_STENCIL_BUFFER_BIT);

    return new Picture(newTextureId, m_windowWidth, m_windowHeight, textureWidth, textureHeight);
}

void RendererOpenGLWin32::RemovePixelsFromScreenCapture(const std::vector<std::pair<int16_t, int16_t>>& coordinates)
{
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);

    //Place a 1 where rendered
    glStencilFunc(GL_ALWAYS, 1, 1);

    //Replace where rendered
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    ViewPorts::ViewPortRect2D rect = ViewPorts::GetOrtho2D(m_windowWidth, m_windowHeight, false);
    const int16_t xMin = (int16_t)floor(rect.left);
    const int16_t xMax = (int16_t)ceil(rect.right);

    glBegin(GL_QUADS);
    for (auto& coordinate : coordinates)
    {
        int16_t xFirst = coordinate.first;
        while (xFirst >= xMin + 320)
        {
            xFirst -= 320;
        }
        const int16_t y = coordinate.second;
        for (int16_t x = xFirst; x < xMax; x += 320)
        {
            glVertex2i(x, y + 1);
            glVertex2i(x + 1, y + 1);
            glVertex2i(x + 1, y);
            glVertex2i(x, y);
        }
    }
    glEnd();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glDisable(GL_STENCIL_TEST);
}

void RendererOpenGLWin32::RenderScreenCapture(Picture* screenCapture)
{
    if (screenCapture == nullptr)
    {
        return;
    }

    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_NOTEQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Select the texture from the picture
    BindTexture(screenCapture->GetTextureId());

    // Draw the texture as a quad
    glBegin(GL_QUADS);
    ViewPorts::ViewPortRect2D rect = ViewPorts::GetOrtho2D(m_windowWidth, m_windowHeight, false);
    const float relativeImageWidth = (float)screenCapture->GetImageWidth() / (float)screenCapture->GetTextureWidth();
    const float relativeImageHeight = (float)screenCapture->GetImageHeight() / (float)screenCapture->GetTextureHeight();
    glTexCoord2f(0, relativeImageHeight); glVertex2d(rect.left, rect.bottom);
    glTexCoord2f(relativeImageWidth, relativeImageHeight); glVertex2d(rect.right, rect.bottom);
    glTexCoord2f(relativeImageWidth, 0); glVertex2d(rect.right, rect.top);
    glTexCoord2f(0, 0); glVertex2d(rect.left, rect.top);
    glEnd();

    glDisable(GL_STENCIL_TEST);
}

uint16_t RendererOpenGLWin32::GetWindowWidth() const
{
    return m_windowWidth;
}

uint16_t RendererOpenGLWin32::GetWindowHeight() const
{
    return m_windowHeight;
}

const std::string& RendererOpenGLWin32::GetGraphicsApiVersion() const
{
    return m_graphicsApiVersion;
}

const std::string& RendererOpenGLWin32::GetGraphicsAdapterVendor() const
{
    return m_graphicsAdapterVendor;
}

const std::string& RendererOpenGLWin32::GetGraphicsAdapterModel() const
{
    return m_graphicsAdapterModel;
}