// Copyright (C) 2019 Arno Ansems
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

#include "ViewPorts.h"

ViewPorts::ViewPortRect2D ViewPorts::GetOrtho2D(const uint16_t windowWidth, const uint16_t windowHeight, const bool helpWindow)
{
    ViewPortRect2D rect;
    const float classicAspectRatio = 4.0f / 3.0f;
    const float windowAspectRatio = (float)windowWidth / (float)windowHeight;

    const double classicWidthInPixels = helpWindow ? 640.0f : 320.0f;
    const double classicHeightInPixels = 200.0f;
    if (windowAspectRatio > classicAspectRatio)
    {
        rect.top = 0.0;
        rect.bottom = classicHeightInPixels;
        double classicWidth = (double)windowWidth / windowAspectRatio * classicAspectRatio;
        double windowWidthInClassicPixels = ((double)windowWidth / classicWidth) * classicWidthInPixels;
        double borderWidth = (windowWidthInClassicPixels - classicWidthInPixels) * 0.5;
        rect.left = -borderWidth;
        rect.right = windowWidthInClassicPixels - borderWidth;
    }
    else
    {
        rect.left = 0.0;
        rect.right = classicWidthInPixels;
        double classicHeight = (double)windowHeight / (1.0 / windowAspectRatio) * (1.0 / classicAspectRatio);
        double windowHeightInClassicPixels = ((double)windowHeight / classicHeight) * classicHeightInPixels;
        double borderHeight = (windowHeightInClassicPixels - classicHeightInPixels) * 0.5;
        rect.top = -borderHeight;
        rect.bottom = windowHeightInClassicPixels - borderHeight;
    }

    return rect;
}

ViewPorts::ViewPortRect3D ViewPorts::Get3D(const uint16_t windowWidth, const uint16_t windowHeight, const float aspectRatio)
{
    ViewPortRect3D rect;

    const float configuredAspectRatio = aspectRatio; //1920.0 / 1080.0;
    const float classicAspectRatio = 4.0f / 3.0f;   // EGA monitors always had a 4:3 aspect ratio.
    const float windowAspectRatio = (float)windowWidth / (float)windowHeight;

    const uint16_t classicScreenHeightInPixels = 200;   // Based on the classic EGA 320x200 screen resolution.
    const uint16_t classicStatusBarHeightInPixels = 80;
    const uint16_t classic3DViewHeightInPixels = classicScreenHeightInPixels - classicStatusBarHeightInPixels;
    const float normalizedStatusBarHeight = (float)classicStatusBarHeightInPixels / (float)classicScreenHeightInPixels;
    const float normalized3DViewHeight = (float)classic3DViewHeightInPixels / (float)classicScreenHeightInPixels;
    if (classicAspectRatio > windowAspectRatio)
    {
        // The aspect ratio of the window is smaller than the classic aspect ratio, which means the view port cannot use the full height
        // of the window. Black borders will appear above and below the game view port.

        // Use the full window width
        rect.left = 0;
        rect.width = windowWidth;

        // Adjust the height of the game view port to match with the classic aspect ratio.
        const float gameHeight = (float)windowWidth / classicAspectRatio;
        rect.height = (uint16_t)(gameHeight * normalized3DViewHeight);
        const uint16_t borderHeight = (uint16_t)((windowHeight - gameHeight) * 0.5);
        rect.bottom = borderHeight + (uint16_t)(gameHeight * normalizedStatusBarHeight);
    }
    else
    {
        // The aspect ratio of the window is bigger than the classic aspect ratio. If the aspect ratio of the window is bigger than
        // the configured aspect ratio, black borders will appear left and right of the game view port.

        // Use the full window height
        rect.bottom = (uint16_t)(windowHeight * normalizedStatusBarHeight);
        rect.height = (uint16_t)(windowHeight * normalized3DViewHeight);

        // The applied aspect ratio is the minimum of the window aspect ratio and the configured aspect ratio.
        const float aspectRatio = (configuredAspectRatio < windowAspectRatio) ? configuredAspectRatio : windowAspectRatio;

        // Adjust the width of the game view port to match with the applied aspect ratio. 
        const uint16_t gameWidth = (uint16_t)(windowHeight * aspectRatio);
        const uint16_t borderWidth = (uint16_t)((windowWidth - gameWidth) * 0.5);
        rect.left = borderWidth;
        rect.width = gameWidth;
    }

    return rect;
}