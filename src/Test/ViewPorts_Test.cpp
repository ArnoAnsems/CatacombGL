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

#include "ViewPorts_Test.h"
#include "../Engine/ViewPorts.h"
#include "../Engine/OverscanBorder.h"

static const float originalAspectRatio = 4.0f / 3.0f;
static const float fitToWindowAspectRatio = 10.0f;
static const ViewPorts::ViewPortRect3D original3DViewArea = { 0, 120, 320, 120 };

ViewPorts_Test::ViewPorts_Test()
{

}

ViewPorts_Test::~ViewPorts_Test()
{

}

TEST(ViewPorts_Test, GetOrtho2DClassicWindow)
{
    // The window is in the classic 4:3 aspect ratio.
    ViewPorts::ViewPortRect2D rect2D = ViewPorts::GetOrtho2D(40, 30, false);

    // Take the overscan border into account
    const double overscanBorderWidth = (double)OverscanBorder::GetBorderWidth();
    const double overscanBorderHeight = (double)OverscanBorder::GetBorderHeight();

    // Total width of Ortho2D must be the classic width of 320 pixels.
    EXPECT_DOUBLE_EQ(rect2D.left, 0.0 - overscanBorderWidth);
    EXPECT_DOUBLE_EQ(rect2D.right, 320.0 + overscanBorderWidth);

    // Total height of Ortho2D must be the original height of 200 pixels.
    EXPECT_DOUBLE_EQ(rect2D.top, 0.0 - overscanBorderHeight);
    EXPECT_DOUBLE_EQ(rect2D.bottom, 200.0 + overscanBorderHeight);
}

TEST(ViewPorts_Test, GetOrtho2DWideWindow)
{
    // The window is two times wider compared to the classic 4:3 aspect ratio.
    ViewPorts::ViewPortRect2D rect2D = ViewPorts::GetOrtho2D(80, 30, false);

    // Take the overscan border into account
    const double overscanBorderWidth = (double)OverscanBorder::GetBorderWidth();
    const double overscanBorderHeight = (double)OverscanBorder::GetBorderHeight();

    // Total width of Ortho2D is 640 pixels: two times the classic width of 320 pixels.
    EXPECT_DOUBLE_EQ(rect2D.left, -(160.0 + (2 * overscanBorderWidth)));
    EXPECT_DOUBLE_EQ(rect2D.right, 480.0 + (2 * overscanBorderWidth));

    // Total height of Ortho2D is the original height of 200 pixels.
    EXPECT_DOUBLE_EQ(rect2D.top, 0.0 - overscanBorderHeight);
    EXPECT_DOUBLE_EQ(rect2D.bottom, 200.0 + overscanBorderHeight);
}

TEST(ViewPorts_Test, GetOrtho2DNarrowWindow)
{
    // The window is only half as wide compared to the original 4:3 aspect ratio.
    ViewPorts::ViewPortRect2D rect2D = ViewPorts::GetOrtho2D(20, 30, false);

    // Take the overscan border into account
    const double overscanBorderWidth = (double)OverscanBorder::GetBorderWidth();
    const double overscanBorderHeight = (double)OverscanBorder::GetBorderHeight();

    // Total width of Ortho2D is the classic width of 320 pixels.
    EXPECT_DOUBLE_EQ(rect2D.left, 0.0 - overscanBorderWidth);
    EXPECT_DOUBLE_EQ(rect2D.right, 320.0 + overscanBorderWidth);

    // Total height of Ortho2D is 400 pixels: two times the classic height of 200 pixels.
    EXPECT_DOUBLE_EQ(rect2D.top, -(100.0 + (2 * overscanBorderHeight)));
    EXPECT_DOUBLE_EQ(rect2D.bottom, 300.0 + (2 * overscanBorderHeight));
}

TEST(ViewPorts_Test, Get3DWideWindowWithOriginalAspectRatio)
{
    // The window is two times wider compared to the classic 4:3 aspect ratio.
    ViewPorts::ViewPortRect3D rect3D = ViewPorts::Get3D(80, 30, originalAspectRatio, original3DViewArea);

    // The 3D viewport must be only half as wide as the window.
    EXPECT_EQ(rect3D.left, 20);
    EXPECT_EQ(rect3D.width, 40);

    // The 3D viewport will leave some height at the bottom for the statusbar.
    EXPECT_EQ(rect3D.bottom, 12);
    EXPECT_EQ(rect3D.height, 17);
}

TEST(ViewPorts_Test, Get3DWideWindowWithFitToWindowAspectRatio)
{
    // The window is two times wider compared to the classic 4:3 aspect ratio.
    ViewPorts::ViewPortRect3D rect3D = ViewPorts::Get3D(80, 30, fitToWindowAspectRatio, original3DViewArea);

    // The 3D viewport must be just as wide as the window.
    EXPECT_EQ(rect3D.left, 0);
    EXPECT_EQ(rect3D.width, 80);

    // The 3D viewport will leave some height at the bottom for the statusbar.
    EXPECT_EQ(rect3D.bottom, 12);
    EXPECT_EQ(rect3D.height, 17);
}

TEST(ViewPorts_Test, Get3DNarrowWindowWithOriginalAspectRatio)
{
    // The window is only half as wide compared to the original 4:3 aspect ratio.
    ViewPorts::ViewPortRect3D rect3D = ViewPorts::Get3D(20, 30, originalAspectRatio, original3DViewArea);

    // The 3D viewport must be just as wide as the window.
    EXPECT_EQ(rect3D.left, 0);
    EXPECT_EQ(rect3D.width, 20);

    // The 3D viewport takes half the height of the window (15 pixels) minus 6 pixels for the statusbar
    // and minus 1 pixel for the overscan border.
    EXPECT_EQ(rect3D.bottom, 13);
    EXPECT_EQ(rect3D.height, 8);
}
