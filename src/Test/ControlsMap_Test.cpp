// Copyright (C) 2026 Arno Ansems
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
#include "../Engine/ControlsMap.h"

class ControlsMap_Test : public ::testing::Test
{
public:
    ControlsMap_Test::ControlsMap_Test()
    {
        m_controlsMap.Clear();
    }

protected:
    ControlsMap m_controlsMap;
};

TEST_F(ControlsMap_Test, assignTwoKeysToAction)
{
    m_controlsMap.AssignActionToKey(UsePotion, SDLK_a);
    m_controlsMap.AssignActionToKey(UsePotion, SDLK_SPACE);

    const auto keysFromAction = m_controlsMap.GetKeysFromAction(UsePotion);
    ASSERT_EQ(keysFromAction.size(), 2);
    EXPECT_EQ(keysFromAction.at(0), SDLK_SPACE);
    EXPECT_EQ(keysFromAction.at(1), SDLK_a);
}
