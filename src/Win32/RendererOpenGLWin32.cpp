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

const float FloorZ = 2.0f;
const float CeilingZ = 1.0f;
const float PlayerZ = 1.5f;

#define GL_CLAMP_TO_EDGE 0x812F

typedef bool (APIENTRY *squall2)       (int T);
typedef const char* (APIENTRY *squall) ();

extern squall wglGetExtensionsStringEXT;
extern squall2 wglSwapIntervalEXT;

squall wglGetExtensionsStringEXT = NULL;
squall2 wglSwapIntervalEXT = NULL;

// Constructor
RendererOpenGLWin32::RendererOpenGLWin32()
{
    m_playerAngle = 0.0f;
    m_playerPosX = 2.5f;
    m_playerPosY = 2.5f;

    m_spritesToRender = new spriteToRender[100];
    m_numberOfSprites = 0;
    m_textureFilter = GL_LINEAR;

    memset(&m_singleColorTexture, 0, sizeof(m_singleColorTexture[0]) * EgaRange);

    m_currentSwapInterval = -1;
}

// Destructor
RendererOpenGLWin32::~RendererOpenGLWin32()
{
    if (m_spritesToRender)
    {
        delete m_spritesToRender;
    }
}

void RendererOpenGLWin32::Setup()
{
    for (egaColor color = EgaBlack; color < EgaRange; color = egaColor(color + 1))
    {
        m_singleColorTexture[color] = generateSingleColorTexture(color);
    }

    wglGetExtensionsStringEXT = (squall)wglGetProcAddress("wglGetExtensionsStringEXT");
    m_isVSyncSupported = IsWGLExtensionSupported("WGL_EXT_swap_control");
    if (m_isVSyncSupported)
    {
        wglSwapIntervalEXT = (squall2)wglGetProcAddress("wglSwapIntervalEXT");
    }
}

const RendererOpenGLWin32::rgbColor egaToRgbMap[EgaRange] =
{
    { 0, 0, 0 },    // Black
    { 0, 0, 170 },    // Blue
    { 0, 170, 0},    // Green
    { 0, 170, 170 },    // Cyan
    { 170, 0, 0 },    // Red
    { 170, 0, 170 },    // Magenta
    { 170, 85, 0 },    // Brown
    { 170, 170, 170 },    // Light gray
    { 85, 85, 85 },    // Dark gray
    { 85, 85, 255 },    // Bright blue
    { 85, 255, 85 },    // Bright green
    { 85, 255, 255 },    // Bright cyan
    { 255, 85, 85 },    // Bright red
    { 255, 85, 255 },    // Bright magenta
    { 255, 255, 85 },    // Bright yellow
    { 255, 255, 255 }     // Bright white
};

IRenderer::rgbColor RendererOpenGLWin32::EgaToRgb(const egaColor ega)
{
    if (ega < EgaRange)
    {
        return egaToRgbMap[ega];
    }
    else
    {
        return egaToRgbMap[egaColor::EgaBlack];
    }
}

/*
const unsigned char defaultTexture[32 * 8] =
{
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 4, 6, 4, 6, 6, 4, 6, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 6, 4, 6, 4, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6,
    6, 4, 4, 4, 6, 4, 6, 4, 6, 6, 6, 6, 4, 6, 6, 4, 6, 6, 6, 4, 6, 4, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6,
    6, 4, 4, 4, 6, 4, 6, 4, 6, 6, 6, 6, 4, 6, 6, 4, 4, 4, 6, 6, 4, 6, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6,
    6, 4, 6, 4, 6, 4, 6, 4, 6, 6, 6, 6, 4, 6, 6, 4, 6, 6, 6, 4, 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 4, 6, 4, 6, 6, 4, 6, 6, 6, 6, 6, 4, 6, 6, 4, 4, 4, 6, 4, 6, 4, 6, 6, 6, 4, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
};

GLuint LoadDefaultTexture()
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    GLfloat* textureImage = NULL;
    textureImage = new GLfloat[32 * 8 * 3];
    for (int i = 0; i < 32 * 8; i++)
    {
        const rgbColor color = EgaToRgb(defaultTexture[i]);
        textureImage[i * 3] = color.red;
        textureImage[i * 3 + 1] = color.green;
        textureImage[i * 3 + 2] = color.blue;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 8, 0, GL_RGB, GL_FLOAT, textureImage);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    delete textureImage;

    return textureId;
}
*/

void RendererOpenGLWin32::SetWindowDimensions(const uint16_t windowWidth, const uint16_t windowHeight)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
}

void RendererOpenGLWin32::SetPlayerAngle(const float angle)
{
    m_playerAngle = angle;
}

void RendererOpenGLWin32::SetPlayerPosition(const float posX, const float posY)
{
    m_playerPosX = posX;
    m_playerPosY = posY;
}

uint32_t RendererOpenGLWin32::LoadFileChunkIntoTexture(const FileChunk* decompressedChunk, const uint16_t width, const uint16_t height, const bool transparent)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    const uint32_t bytesPerPixel = transparent ? 4 : 3;
    GLubyte* textureImage = new GLubyte[decompressedChunk->GetSize() * 2 * bytesPerPixel];
    const uint32_t planeSize = decompressedChunk->GetSize() / 4;
    unsigned char* chunk = decompressedChunk->GetChunk();

    for (uint32_t i = 0; i < planeSize; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            bool blueplane = ((chunk[i] & (1 <<j)) > 0);
            bool greenplane =  ((chunk[i + planeSize] & (1 << j)) > 0);
            bool redplane =  ((chunk[i + (2 * planeSize)] & (1 << j)) > 0);
            bool intensityplane = ((chunk[i + (3 * planeSize)] & (1 << j)) > 0);
            const egaColor colorIndex = (egaColor)((intensityplane ? EgaDarkGray : EgaBlack) + (redplane ? EgaRed : EgaBlack) + (greenplane ? EgaGreen : EgaBlack) + (blueplane ? EgaBlue : EgaBlack));
            const bool transparentPixel = transparent && (colorIndex == 5);
            const rgbColor color1 = EgaToRgb(transparentPixel ? EgaBlack : colorIndex);
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel)] = color1.red;
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 1] = color1.green;
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 2] = color1.blue;
            if (transparent)
            {
                textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 3] = transparentPixel ? 0 : 255;
            }
        }
    }
    const int16_t format = transparent ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureImage);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );   // GL_NEAREST

    delete textureImage;

    return textureId;
}

uint32_t RendererOpenGLWin32::LoadMaskedFileChunkIntoTexture(const FileChunk* decompressedChunk, const uint16_t width, const uint16_t height)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    const uint32_t bytesPerPixel = 4;
    GLubyte* textureImage = new GLubyte[width * height * bytesPerPixel];
    const uint32_t planeSize = decompressedChunk->GetSize() / 5;
    unsigned char* chunk = decompressedChunk->GetChunk();

    for (uint32_t i = 0; i < planeSize; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            bool transparencyplane =  ((chunk[i] & (1 <<j)) > 0);
            bool blueplane = ((chunk[i + planeSize] & (1 << j)) > 0);
            bool greenplane =  ((chunk[i + (2 * planeSize)] & (1 << j)) > 0);
            bool redplane = ((chunk[i + (3 * planeSize)] & (1 << j)) > 0);
            bool intensityplane = ((chunk[i + (4 * planeSize)] & (1 << j)) > 0);
            const egaColor colorIndex = (egaColor)((intensityplane ? EgaDarkGray : EgaBlack) + (redplane ? EgaRed : EgaBlack) + (greenplane ? EgaGreen : EgaBlack) + (blueplane ? EgaBlue : EgaBlack));
            const rgbColor color1 = EgaToRgb(transparencyplane ? EgaBlack : colorIndex);
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel)] = color1.red;
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 1] = color1.green;
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 2] = color1.blue;
            textureImage[(i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 3] = transparencyplane ? 0 : 255;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);

    delete textureImage;

    return textureId;
}

uint32_t RendererOpenGLWin32::generateSingleColorTexture(const egaColor color) const
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    const uint32_t bytesPerPixel = 3;
    GLubyte* textureImage = new GLubyte[64 * 64 * bytesPerPixel];

    const rgbColor color1 = EgaToRgb(color);
    for (uint32_t i = 0; i < 64 * 64; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            textureImage[(i * bytesPerPixel)] = color1.red;
            textureImage[(i * bytesPerPixel) + 1] = color1.green;
            textureImage[(i * bytesPerPixel) + 2] = color1.blue;
        }
    }
    const int16_t format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, 64, 64, 0, format, GL_UNSIGNED_BYTE, textureImage);

    delete textureImage;

    return textureId;
}

uint32_t RendererOpenGLWin32::LoadTilesSize8MaskedIntoTexture(const FileChunk* decompressedChunk)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    const uint32_t bytesPerPixel = 4;
    const uint32_t numberOfTiles = decompressedChunk->GetSize() / 40;
    GLubyte* textureImage = new GLubyte[numberOfTiles * 8 * 8 * bytesPerPixel];
    const uint32_t planeSize = 8;
    unsigned char* chunk = decompressedChunk->GetChunk();

    for (uint32_t tile = 0; tile < numberOfTiles; tile++)
    {
        const uint32_t tileChunkOffset = tile * 40;
        const uint32_t tileTextureOffset = tile * 8 * 8 * bytesPerPixel;
        for (uint32_t i = 0; i < planeSize; i++)
        {
            for (int j = 0; j < 8; j++)
            {

                bool transparencyplane =  ((chunk[tileChunkOffset + i] & (1 <<j)) > 0);
                bool blueplane = ((chunk[tileChunkOffset + i + planeSize] & (1 << j)) > 0);
                bool greenplane =  ((chunk[tileChunkOffset + i + (2 * planeSize)] & (1 << j)) > 0);
                bool redplane = ((chunk[tileChunkOffset + i + (3 * planeSize)] & (1 << j)) > 0);
                bool intensityplane = ((chunk[tileChunkOffset + i + (4 * planeSize)] & (1 << j)) > 0);
                const egaColor colorIndex = (egaColor)((intensityplane ? EgaDarkGray : EgaBlack) + (redplane ? EgaRed : EgaBlack) + (greenplane ? EgaGreen : EgaBlack) + (blueplane ? EgaBlue : EgaBlack));
                const rgbColor color1 = EgaToRgb(transparencyplane ? EgaBlack : colorIndex);
                textureImage[tileTextureOffset + (i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel)] = color1.red;
                textureImage[tileTextureOffset + (i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 1] = color1.green;
                textureImage[tileTextureOffset + (i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 2] = color1.blue;
                textureImage[tileTextureOffset + (i * 8 * bytesPerPixel) + ((7 - j) * bytesPerPixel) + 3] = transparencyplane ? 0 : 255;
            }
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, numberOfTiles * 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );   // GL_NEAREST

    delete textureImage;

    return textureId;
}

uint32_t RendererOpenGLWin32::LoadFontIntoTexture(const bool* fontPicture)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    GLubyte* textureImage = new GLubyte[256 * 16 * 10 * 4];
    for (uint32_t i = 0; i < 256 * 16 * 10; i++)
    {
        const bool fontBit = fontPicture[i];
        textureImage[i * 4] = 255;
        textureImage[(i * 4) + 1] = 255;
        textureImage[(i * 4) + 2] =  255;
        textureImage[(i * 4) + 3] = fontBit ? 255 : 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 16 * 10, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    delete textureImage;

    return textureId;
}

void RendererOpenGLWin32::RenderTextCentered(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY)
{
    if (text == NULL || font == NULL)
    {
        // Nothing to render
        return;
    }

    uint16_t totalWidth = 0;
    for (uint16_t chari = 0; chari < strlen(text); chari++)
    {
        const uint8_t charIndex = text[chari];
        totalWidth += font->GetCharacterWidth(charIndex);
    }

    const uint16_t halfTotalWidth = totalWidth / 2;
    const uint16_t leftAlignedOffsetX = offsetX - halfTotalWidth;
    RenderTextLeftAligned(text, font, colorIndex, leftAlignedOffsetX, offsetY);
}

void RendererOpenGLWin32::RenderTextLeftAligned(const char* text, const Font* font, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY)
{
    if (text == NULL || font == NULL)
    {
        // Nothing to render
        return;
    }

    // Set the MODELVIEW matrix to the requested offset
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(GLfloat(offsetX), GLfloat(offsetY), 0.0f);

    // Select the texture from the picture
    glBindTexture(GL_TEXTURE_2D, font->GetTextureId());

    // Do not wrap the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);

    rgbColor color = EgaToRgb(colorIndex);

    // Draw the texture as a quad
    uint16_t combinedWidth = 0;
    glBegin(GL_QUADS);
    glColor3f((float)(color.red) / 256.0f, (float)(color.green) / 256.0f, (float)(color.blue) / 256.0f);
    for (uint16_t chari = 0; chari < strlen(text); chari++)
    {
        const uint8_t charIndex = text[chari];
        uint16_t charWidth =  font->GetCharacterWidth(charIndex);
        float textureHeight = 1.0f / 16.0f;
        float textureWidth = (float)(charWidth) / 256.0f;
        float textureOffsetX = float(charIndex % 16) / 16.0f;
        float textureOffsetY = float(charIndex / 16) / 16.0f;
        

        glTexCoord2f(textureOffsetX, textureOffsetY + textureHeight); glVertex2i(combinedWidth, 10);
        glTexCoord2f(textureOffsetX + textureWidth, textureOffsetY + textureHeight); glVertex2i(combinedWidth + charWidth, 10);
        glTexCoord2f(textureOffsetX + textureWidth, textureOffsetY); glVertex2i(combinedWidth + charWidth, 0);
        glTexCoord2f(textureOffsetX, textureOffsetY); glVertex2i(combinedWidth, 0);
        combinedWidth += charWidth;
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
}

void RendererOpenGLWin32::RenderNumber(const uint16_t value, const Font* font, const uint8_t maxDigits, const egaColor colorIndex, const uint16_t offsetX, const uint16_t offsetY)
{
    char str[10];

    _itoa_s(value,str,10);

    const uint16_t widthOfBlank = font->GetCharacterWidth('0');
    const uint16_t widthOfBlanks = widthOfBlank * (maxDigits - (uint16_t)strlen(str));

    RenderTextLeftAligned(str, font, colorIndex, offsetX + widthOfBlanks, offsetY);
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

    const float classicAspectRatio = 4.0f / 3.0f;
    const float windowAspectRatio = (float)m_windowWidth / (float)m_windowHeight;

    double top, bottom, left, right;
    const double classicWidthInPixels = helpWindow ? 640.0f : 320.0f;
    const double classicHeightInPixels = 200.0f;
    if (windowAspectRatio > classicAspectRatio)
    {
        top = 0.0;
        bottom = classicHeightInPixels;
        double classicWidth = (double)m_windowWidth / windowAspectRatio * classicAspectRatio;
        double windowWidthInClassicPixels = ((double)m_windowWidth / classicWidth) * classicWidthInPixels;
        double borderWidth = (windowWidthInClassicPixels - classicWidthInPixels) * 0.5;
        left = -borderWidth;
        right = windowWidthInClassicPixels - borderWidth;
    }
    else
    {
        left = 0.0;
        right = classicWidthInPixels;
        double classicHeight =  (double)m_windowHeight / (1.0 / windowAspectRatio) * (1.0 / classicAspectRatio);
        double windowHeightInClassicPixels = ((double)m_windowHeight / classicHeight) * classicHeightInPixels;
        double borderHeight = (windowHeightInClassicPixels - classicHeightInPixels) * 0.5;
        top = -borderHeight;
        bottom = windowHeightInClassicPixels - borderHeight;
    }

    gluOrtho2D(left, right, bottom, top);

    glDisable(GL_LIGHTING);
}

void RendererOpenGLWin32::Unprepare2DRendering()
{

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void RendererOpenGLWin32::Render2DPicture(const Picture* picture, const uint16_t offsetX, const uint16_t offsetY)
{
    if (picture == NULL)
    {
        // Nothing to render
        return;
    }

    // Set the MODELVIEW matrix to the requested offset
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(GLfloat(offsetX), GLfloat(offsetY), 0.0f);

    // Select the texture from the picture
    glBindTexture(GL_TEXTURE_2D, picture->GetTextureId());

    // Do not wrap the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);

    // Draw the texture as a quad
    const GLint width = (uint16_t)picture->GetWidth();
    const GLint height = (uint16_t)picture->GetHeight();
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex2i(0, height);
    glTexCoord2i(1, 1); glVertex2i(width, height);
    glTexCoord2i(1, 0); glVertex2i(width, 0);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glEnd();
}

void RendererOpenGLWin32::Render2DTileSize8Masked(const Picture* tiles, const uint16_t tileIndex, const uint16_t offsetX, const uint16_t offsetY)
{
    if (tiles == NULL ||
        tileIndex >= (tiles->GetHeight() / 8))
    {
        // Nothing to render
        return;
    }

    // Set the MODELVIEW matrix to the requested offset
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(GLfloat(offsetX), GLfloat(offsetY), 0.0f);

    // Select the texture from the picture
    glBindTexture(GL_TEXTURE_2D, tiles->GetTextureId());

    // Do not wrap the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);

    // Draw the texture as a quad
    glBegin(GL_QUADS);
    const float textureHeight = 1.0f / (tiles->GetHeight() / 8);
    const float textureWidth = 1.0f;
    const float textureOffsetX = 0.0f;
    const float textureOffsetY = float(tileIndex) / float(tiles->GetHeight() / 8);

    glTexCoord2f(textureOffsetX, textureOffsetY + textureHeight); glVertex2i(0, 8);
    glTexCoord2f(textureOffsetX + textureWidth, textureOffsetY + textureHeight); glVertex2i(8, 8);
    glTexCoord2f(textureOffsetX + textureWidth, textureOffsetY); glVertex2i(8, 0);
    glTexCoord2f(textureOffsetX, textureOffsetY); glVertex2i(0, 0);
    glEnd();
}

void RendererOpenGLWin32::Render2DBar(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const egaColor colorIndex)
{
     glDisable(GL_TEXTURE_2D);

    const rgbColor color = EgaToRgb(colorIndex);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(GLfloat(x), GLfloat(y), 0.0f);

    glBegin(GL_QUADS);
    glColor3f((float)(color.red) / 256.0f, (float)(color.green) / 256.0f, (float)(color.blue) / 256.0f);
    glTexCoord2i(0, 1); glVertex2i(0, height);
    glTexCoord2i(1, 1); glVertex2i(width, height);
    glTexCoord2i(1, 0); glVertex2i(width, 0);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
}

void RendererOpenGLWin32::RenderRadarBlip(const float x, const float y, const egaColor colorIndex)
{
    glDisable(GL_TEXTURE_2D);

    const rgbColor color = EgaToRgb(colorIndex);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);

    const uint16_t height = 1;
    const uint16_t width = 1;

    glBegin(GL_QUADS);
    glColor3f((float)(color.red) / 256.0f, (float)(color.green) / 256.0f, (float)(color.blue) / 256.0f);
    glTexCoord2i(0, 1); glVertex2i(0, height);
    glTexCoord2i(1, 1); glVertex2i(width, height);
    glTexCoord2i(1, 0); glVertex2i(width, 0);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
}

void RendererOpenGLWin32::Prepare3DRendering(const bool depthShading, const float aspectRatio, uint16_t fov)
{
    const float configuredAspectRatio = aspectRatio; //1920.0 / 1080.0;
    const float classicAspectRatio = 4.0f / 3.0f;   // EGA monitors always had a 4:3 aspect ratio.
    const float windowAspectRatio = (float)m_windowWidth / (float)m_windowHeight;

    const uint16_t classicScreenHeightInPixels = 200;   // Based on the classic EGA 320x200 screen resolution.
    const uint16_t classicStatusBarHeightInPixels = 80;
    const uint16_t classic3DViewHeightInPixels = classicScreenHeightInPixels - classicStatusBarHeightInPixels;
    const float normalizedStatusBarHeight = (float)classicStatusBarHeightInPixels / (float)classicScreenHeightInPixels;
    const float normalized3DViewHeight = (float)classic3DViewHeightInPixels / (float)classicScreenHeightInPixels;
    uint16_t left, bottom, width, height;
    if (classicAspectRatio > windowAspectRatio)
    {
        // The aspect ratio of the window is smaller than the classic aspect ratio, which means the view port cannot use the full height
        // of the window. Black borders will appear above and below the game view port.

        // Use the full window width
        left = 0;
        width = m_windowWidth;

        // Adjust the height of the game view port to match with the classic aspect ratio.
        const float gameHeight = (float)m_windowWidth / classicAspectRatio;
        height = (uint16_t)(gameHeight * normalized3DViewHeight);
        const uint16_t borderHeight = (uint16_t)((m_windowHeight - gameHeight) * 0.5);
        bottom = borderHeight + (uint16_t)(gameHeight * normalizedStatusBarHeight);
    }
    else
    {
        // The aspect ratio of the window is bigger than the classic aspect ratio. If the aspect ratio of the window is bigger than
        // the configured aspect ratio, black borders will appear left and right of the game view port.
        
        // Use the full window height
        bottom = (uint16_t)(m_windowHeight * normalizedStatusBarHeight);
        height = (uint16_t)(m_windowHeight * normalized3DViewHeight);

        // The applied aspect ratio is the minimum of the window aspect ratio and the configured aspect ratio.
        const float aspectRatio = (configuredAspectRatio < windowAspectRatio) ? configuredAspectRatio : windowAspectRatio;

        // Adjust the width of the game view port to match with the applied aspect ratio. 
        const uint16_t gameWidth = (uint16_t)(m_windowHeight * aspectRatio);
        const uint16_t borderWidth = (uint16_t)((m_windowWidth - gameWidth) * 0.5);
        left = borderWidth;
        width = gameWidth;
    }
    glViewport(left, bottom, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    
    glClearDepth(1.0f);                         // Depth Buffer Setup

    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing

    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective((double)fov,(float)width / (float)height,0.1f,100.0f);

    
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(m_playerAngle, 0.0f, 0.0f, -1.0f);
    glTranslatef(-m_playerPosX, -m_playerPosY, -PlayerZ);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (depthShading)
    {
        GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 
        GLfloat LightPosition[]= { m_playerPosX, m_playerPosY, -PlayerZ, 1.0f }; 
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); 
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_LIGHT1);
    }

    glShadeModel(GL_SMOOTH);
}

void RendererOpenGLWin32::PrepareWalls()
{
    glEnable(GL_TEXTURE_2D);

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
}

void RendererOpenGLWin32::UnprepareWalls()
{
    glDisable(GL_CULL_FACE);
}

void RendererOpenGLWin32::Render3DWall(const Picture* picture, const int16_t tileX, const int16_t tileY, const int16_t orientation)
{
    if (picture == NULL)
    {
        // Nothing to render
        return;
    }

    glMatrixMode(GL_MODELVIEW);						// Select The Projection Matrix
    glLoadIdentity();
    glTranslatef(tileX + 0.5f, tileY + 0.5f, 0);
    glRotatef(orientation, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, -0.5f, 0.0f);

    // Select the texture from the picture
    glBindTexture(GL_TEXTURE_2D, picture->GetTextureId());

    // Only wrap the texture in horizontal direction
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);

    glNormal3f( 0.0f, 0.0f, -1.0f);

    // Draw the texture as a quad
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex3f(-0.5f, 0.0f, FloorZ);
    glTexCoord2i(0, 1); glVertex3f(0.5f, 0.0f, FloorZ);
    glTexCoord2i(0, 0); glVertex3f(0.5f, 0.0f, CeilingZ);
    glTexCoord2i(1, 0); glVertex3f(-0.5f, 0.0f, CeilingZ);
    glEnd();
}

void RendererOpenGLWin32::Render3DSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation)
{
    glMatrixMode(GL_MODELVIEW);						// Select The Projection Matrix
    glLoadIdentity();
    
    glTranslatef(offsetX, offsetY, 0.0f);
    if (orientation == RotatedTowardsPlayer)
    {
        glRotatef(m_playerAngle, 0.0f, 0.0f, 1.0f);
    }
    else if (orientation == AlongYAxis)
    {
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    }
    const GLfloat halfWidth = (float)(picture->GetWidth()) / 128.0f;
    const GLfloat topZ = CeilingZ + ((float)(picture->GetHeight()) / 64.0f) * (FloorZ - CeilingZ);

    // Select the texture from the picture
    glBindTexture(GL_TEXTURE_2D, picture->GetTextureId());

    // Do not wrap the texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);

    glNormal3f( 0.0f, 0.0f, -1.0f);

    const float zOffset = (orientation == RotatedTowardsPlayer) ? 0.0625f : 0.0f;

    // Draw the texture as a quad
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex3f(-halfWidth, 0.0f, CeilingZ + zOffset);
    glTexCoord2i(1, 0); glVertex3f(halfWidth, 0.0f, CeilingZ + zOffset);
    glTexCoord2i(1, 1); glVertex3f(halfWidth, 0.0f, topZ + zOffset);
    glTexCoord2i(0, 1); glVertex3f(-halfWidth, 0.0f, topZ + zOffset);
    glEnd();
}

void RendererOpenGLWin32::AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation)
{
    if (m_numberOfSprites == 100)
    {
        return;
    }

    m_spritesToRender[m_numberOfSprites].picture = picture;
    m_spritesToRender[m_numberOfSprites].offsetX = offsetX;
    m_spritesToRender[m_numberOfSprites].offsetY = offsetY;
    m_spritesToRender[m_numberOfSprites].squaredDistance = (int32_t)(((offsetX - m_playerPosX) * (offsetX - m_playerPosX)) + ((offsetY - m_playerPosY) * (offsetY - m_playerPosY)));
    m_spritesToRender[m_numberOfSprites].orientation = orientation;
    m_numberOfSprites++;
}

void RendererOpenGLWin32::RenderAllSprites()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    quickSort(0, m_numberOfSprites);

    for (int16_t i = m_numberOfSprites - 1; i >= 0; i--)
    {
        Render3DSprite(m_spritesToRender[i].picture, m_spritesToRender[i].offsetX, m_spritesToRender[i].offsetY, m_spritesToRender[i].orientation);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    m_numberOfSprites = 0;
}

void RendererOpenGLWin32::quickSort(uint16_t p,uint16_t q)
{
    if(p<q)
    {
        int r=partition(p,q);
        quickSort(p,r);  
        quickSort(r+1,q);
    }
}


uint16_t RendererOpenGLWin32::partition(uint16_t p,uint16_t q)
{
    const int32_t x= m_spritesToRender[p].squaredDistance;
    uint16_t i=p;

    for(uint16_t j=p+1; j<q; j++)
    {
        if(m_spritesToRender[j].squaredDistance<=x)
        {
            i=i+1;
            swap(i,j);
        }
    }

    swap(i,p);
    return i;
}

void RendererOpenGLWin32::swap(uint16_t p,uint16_t q)
{
    spriteToRender dummy;
    dummy.picture = m_spritesToRender[p].picture;
    dummy.offsetX = m_spritesToRender[p].offsetX;
    dummy.offsetY = m_spritesToRender[p].offsetY;
    dummy.squaredDistance = m_spritesToRender[p].squaredDistance;
    dummy.orientation = m_spritesToRender[p].orientation;
    m_spritesToRender[p].picture = m_spritesToRender[q].picture;
    m_spritesToRender[p].offsetX = m_spritesToRender[q].offsetX;
    m_spritesToRender[p].offsetY = m_spritesToRender[q].offsetY;
    m_spritesToRender[p].squaredDistance = m_spritesToRender[q].squaredDistance;
    m_spritesToRender[p].orientation = m_spritesToRender[q].orientation;
    m_spritesToRender[q].picture = dummy.picture;
    m_spritesToRender[q].offsetX = dummy.offsetX;
    m_spritesToRender[q].offsetY = dummy.offsetY;
    m_spritesToRender[q].squaredDistance = dummy.squaredDistance;
    m_spritesToRender[q].orientation = dummy.orientation;
}

void RendererOpenGLWin32::PrepareFloorAndCeiling()
{
    glEnable(GL_TEXTURE_2D);
}

void RendererOpenGLWin32::UnprepareFloorAndCeiling()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);                         // Depth Buffer Setup
}

void RendererOpenGLWin32::RenderFloor(const uint16_t tileX, const uint16_t tileY, const egaColor colorIndex)
{
    glBindTexture(GL_TEXTURE_2D, m_singleColorTexture[colorIndex]);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);
    
    glNormal3f( 0.0f, 0.0f, -1.0f);
    
    glBegin(GL_QUADS);

    glTexCoord2i(0, 0); glVertex3f((float)tileX, (float)tileY, FloorZ);					// Top Left
    glTexCoord2i(1, 0); glVertex3f((float)tileX, (float)(tileY + 1), FloorZ);					// Top Right
    glTexCoord2i(1, 1); glVertex3f((float)(tileX + 1), (float)(tileY + 1), FloorZ);					// Bottom Right
    glTexCoord2i(0, 1); glVertex3f((float)(tileX + 1), (float)tileY, FloorZ);					// Bottom Left

    glEnd();
}

void RendererOpenGLWin32::RenderCeiling(const uint16_t tileX, const uint16_t tileY, const egaColor colorIndex)
{
    glBindTexture(GL_TEXTURE_2D, m_singleColorTexture[colorIndex]);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,m_textureFilter);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,m_textureFilter);

    glNormal3f( 0.0f, 0.0f, -1.0f);

    glBegin(GL_QUADS);

    glTexCoord2i(0, 0); glVertex3f((float)tileX, (float)tileY, CeilingZ);					// Top Left
    glTexCoord2i(1, 0); glVertex3f((float)tileX, (float)(tileY + 1), CeilingZ);					// Top Right
    glTexCoord2i(1, 1); glVertex3f((float)(tileX + 1), (float)(tileY + 1), CeilingZ);					// Bottom Right
    glTexCoord2i(0, 1); glVertex3f((float)(tileX + 1), (float)tileY, CeilingZ);					// Bottom Left

    glEnd();
}

void RendererOpenGLWin32::SetTextureFilter(const TextureFilterSetting textureFilter)
{
    m_textureFilter = (textureFilter == Nearest) ? GL_NEAREST : GL_LINEAR;
}

void RendererOpenGLWin32::SetVSync(const bool enabled)
{
    const int32_t requestedSwapInterval = (enabled) ? 1 : 0;
    if (requestedSwapInterval != m_currentSwapInterval)
    {
        if (wglSwapIntervalEXT != NULL)
        {
            wglSwapIntervalEXT(requestedSwapInterval);
        }
        m_currentSwapInterval = requestedSwapInterval;
    }   
}

bool RendererOpenGLWin32::IsWGLExtensionSupported(const char *extension_name)
{
    return (strstr(wglGetExtensionsStringEXT(), extension_name) != NULL);
}

bool RendererOpenGLWin32::IsVSyncSupported()
{
    return m_isVSyncSupported;
}

void RendererOpenGLWin32::PrepareVisibilityMap()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
}

void RendererOpenGLWin32::UnprepareVisibilityMap()
{
    glEnable(GL_DEPTH_TEST);
}