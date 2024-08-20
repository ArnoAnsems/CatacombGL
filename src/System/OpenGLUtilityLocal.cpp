// Copyright (C) 2024 Arno Ansems
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

#include "OpenGLUtilityLocal.h"
#include <cmath>

void OpenGLUtilityLocal::gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    // Implementation derived from the example as shown at:
    // https://nehe.gamedev.net/article/replacement_for_gluperspective/21002/

    constexpr GLdouble pi = 3.1415926535897932384626433832795;
    const GLdouble fH = tan(fovY / 360.0 * pi) * zNear;
    const GLdouble fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void OpenGLUtilityLocal::gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
    // Implementation derived from remark as seen at:
    // https://learn.microsoft.com/en-us/windows/win32/opengl/gluortho2d

    constexpr GLdouble zNear = -1.0;
    constexpr GLdouble zFar = 1.0;
    glOrtho(left, right, bottom, top, zNear, zFar);
}

void OpenGLUtilityLocal::gluLookAt(
    GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
    GLdouble centerX, GLdouble centerY, GLdouble centerZ,
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    // Implementation derived from specification as seen at:
    // https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml

    GLdouble fx = centerX - eyeX;
    GLdouble fy = centerY - eyeY;
    GLdouble fz = centerZ - eyeZ; 
    normalize(fx, fy, fz);

    normalize(upX, upY, upZ);

    GLdouble sx = 0.0;
    GLdouble sy = 0.0;
    GLdouble sz = 0.0;
    crossProduct(fx, fy, fz, upX, upY, upZ, sx, sy, sz);
    
    normalize(sx, sy, sz);

    GLdouble ux = 0.0;
    GLdouble uy = 0.0;
    GLdouble uz = 0.0;
    crossProduct(sx, sy, sz, fx, fy, fz, ux, uy, uz);
    
    normalize(ux, uy, uz);

    GLdouble rotationMatrix[16] =
    {
        sx,  ux,  -fx, 0.0,
        sy,  uy,  -fy, 0.0,
        sz,  uz,  -fz, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    glMultMatrixd(rotationMatrix);
    glTranslated(-eyeX, -eyeY, -eyeZ);
}

void OpenGLUtilityLocal::crossProduct(
    GLdouble vecAx, GLdouble vecAy, GLdouble vecAz,
    GLdouble vecBx, GLdouble vecBy, GLdouble vecBz,
    GLdouble& crossPx, GLdouble& crossPy, GLdouble& crossPz)
{
    crossPx = vecAy * vecBz - vecAz * vecBy;
    crossPy = vecAz * vecBx - vecAx * vecBz;
    crossPz = vecAx * vecBy - vecAy * vecBx;
}

void OpenGLUtilityLocal::normalize(GLdouble& vecx, GLdouble& vecy, GLdouble& vecz)
{
    const GLdouble length = sqrt(vecx * vecx + vecy * vecy + vecz * vecz);
    vecx = vecx / length;
    vecy = vecy / length;
    vecz = vecz / length;
}
