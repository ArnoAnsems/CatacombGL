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
//
// OpenGLUtilityLocal
//
// Local implementation of three functions from the OpenGL Utility Library (GLU).
// This prevents a dependency on the actual GLU library.
#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

class OpenGLUtilityLocal
{
public:
    OpenGLUtilityLocal() = default;
    ~OpenGLUtilityLocal() = default;

    static void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    static void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
    static void gluLookAt(
        GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
        GLdouble centerX, GLdouble centerY, GLdouble centerZ,
        GLdouble upX, GLdouble upY, GLdouble upZ);

private:
    static void crossProduct(
        GLdouble vecAx, GLdouble vecAy, GLdouble vecAz,
        GLdouble vecBx, GLdouble vecBy, GLdouble vecBz,
        GLdouble& vecPx, GLdouble& vecPy, GLdouble& vecPz);
    static void normalize(GLdouble& vecx, GLdouble& vecy, GLdouble& vecz);
};

