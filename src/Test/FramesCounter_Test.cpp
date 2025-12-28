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

#include <gtest/gtest.h>
#include "../Engine/FramesCounter.h"

class FramesCounter_Test : public ::testing::Test
{
};

TEST(FramesCounter_Test, ZeroFpsWhenNoFramesAdded)
{
    FramesCounter framesCounter;
    EXPECT_EQ(0, framesCounter.GetFramesPerSecond());
}

TEST(FramesCounter_Test, ZeroFpsWhenNotASingleSecondHasPassed)
{
    FramesCounter framesCounter;
    framesCounter.AddFrame(1000);
    framesCounter.AddFrame(1999);
    EXPECT_EQ(0, framesCounter.GetFramesPerSecond());
}

TEST(FramesCounter_Test, TwoFpsWhenTwoFramesAddedInPastSecond)
{
    FramesCounter framesCounter;
    framesCounter.AddFrame(2000); // Counter started and 1 frame added
    framesCounter.AddFrame(2500); // 2 frames added
    framesCounter.AddFrame(3000); // One second has passed; counter reset and number of frames stored from past second
    EXPECT_EQ(2, framesCounter.GetFramesPerSecond());
}

