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

static const float originalAspectRatio = 4.0f / 3.0f; // EGA monitors always had a 4:3 aspect ratio.

ViewPorts::ViewPortRect2D ViewPorts::GetOrtho2D(const uint16_t windowWidth, const uint16_t windowHeight, const bool helpWindow)
{
    ViewPortRect2D rect;
    const float windowAspectRatio = (float)windowWidth / (float)windowHeight;

    const double originalWidthInPixels = helpWindow ? 640.0f : 320.0f;
    const double originalHeightInPixels = 200.0f;
    const double overscanBorderWidth = 4.0f;
    const double overscanBorderHeight = 3.0f;
    if (windowAspectRatio > originalAspectRatio)
    {
        rect.top = -overscanBorderHeight;
        rect.bottom = originalHeightInPixels + overscanBorderHeight;
        double originalWidth = (float)windowHeight * originalAspectRatio;
        double windowWidthInClassicPixels = ((double)windowWidth / originalWidth) * (originalWidthInPixels + 8);
        double borderWidth = (windowWidthInClassicPixels - originalWidthInPixels) * 0.5;
        rect.left = -borderWidth;
        rect.right = originalWidthInPixels + borderWidth;
    }
    else
    {
        rect.left = -overscanBorderWidth;
        rect.right = originalWidthInPixels + overscanBorderWidth;
        double originalHeight = (double)windowHeight / (1.0 / windowAspectRatio) * (1.0 / originalAspectRatio);
        double windowHeightInClassicPixels = ((double)windowHeight / originalHeight) * (originalHeightInPixels + 6);
        double borderHeight = (windowHeightInClassicPixels - originalHeightInPixels) * 0.5;
        rect.top = -borderHeight;
        rect.bottom = originalHeightInPixels + borderHeight;
    }

    return rect;
}

ViewPorts::ViewPortRect3D ViewPorts::Get3D(const uint16_t windowWidth, const uint16_t windowHeight, const float aspectRatio, const ViewPortRect3D original3DViewArea)
{
    ViewPortRect3D rect;

    const float configuredAspectRatio = aspectRatio; //1920.0 / 1080.0;
    const float windowAspectRatio = (float)windowWidth / (float)windowHeight;

    const uint16_t originalScreenHeightInPixels = 200 + 6;   // Based on the classic EGA 320x200 screen resolution.
    const uint16_t originalBottomBorderHeightInPixels = originalScreenHeightInPixels - original3DViewArea.bottom - 3;
    const uint16_t original3DViewHeightInPixels = original3DViewArea.height;
    const float normalizedBottomBorderHeight = (float)originalBottomBorderHeightInPixels / (float)originalScreenHeightInPixels;
    const float normalized3DViewHeight = (float)original3DViewHeightInPixels / (float)originalScreenHeightInPixels;

    const uint16_t originalScreenWidthInPixels = 320 + 8;   // Based on the classic EGA 320x200 screen resolution.

    if (originalAspectRatio > windowAspectRatio)
    {
        // The aspect ratio of the window is smaller than the classic aspect ratio, which means the view port cannot use the full height
        // of the window. Black borders will appear above and below the game view port.

        const float originalPixelWidth = (float)windowWidth / originalScreenWidthInPixels;
        const float originalPixelHeight = originalPixelWidth * 1.2f;

        // Use the full window width
        rect.left = (uint16_t)(originalPixelWidth * 4.0f);
        const uint16_t rightBorderWidthInOriginalPixels = 320 - original3DViewArea.width - original3DViewArea.left;
        rect.width = (uint16_t)((original3DViewArea.width - original3DViewArea.left) * originalPixelWidth);

        const uint16_t gameHeight = originalPixelHeight * 200.0f;
        const uint16_t bottomBorder = (uint16_t)((windowHeight - gameHeight) * 0.5);
        
        rect.height = (uint16_t)(originalPixelHeight * original3DViewArea.height);
        rect.bottom = bottomBorder + (uint16_t)(originalPixelHeight * (200 - original3DViewArea.bottom));
    }
    else
    {
        // The aspect ratio of the window is bigger than the classic aspect ratio. If the aspect ratio of the window is bigger than
        // the configured aspect ratio, black borders will appear left and right of the game view port.

        // Use the full window height
        rect.bottom = (uint16_t)(windowHeight * normalizedBottomBorderHeight);
        rect.height = (uint16_t)(windowHeight * normalized3DViewHeight);

        // The applied aspect ratio is the minimum of the window aspect ratio and the configured aspect ratio.
        const float aspectRatio = (configuredAspectRatio < windowAspectRatio) ? configuredAspectRatio : windowAspectRatio;

        // Adjust the width of the game view port to match with the applied aspect ratio. 
        const float originalPixelHeight = (float)windowHeight / originalScreenHeightInPixels;
        const float originalPixelWidth = originalPixelHeight * originalAspectRatio * (200.0f / 320.0f);
        
        const uint16_t rightBorderWidthInOriginalPixels = 320 - original3DViewArea.width - original3DViewArea.left;
        const uint16_t gameWidth = (uint16_t)((float)windowHeight * aspectRatio);
        rect.left = (uint16_t)((windowWidth - gameWidth) * 0.5) + (4.0f * originalPixelWidth);
        rect.width = gameWidth - ((8 + rightBorderWidthInOriginalPixels) * originalPixelWidth);
    }

    return rect;
}