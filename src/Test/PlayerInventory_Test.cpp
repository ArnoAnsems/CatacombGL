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
#include "../Engine/PlayerInventory.h"
#include "GameStub.h"

class PlayerInventory_Test : public ::testing::Test
{
protected:
    GameStub m_gameStub;
};

TEST_F(PlayerInventory_Test, InitiallyEmptyInventory)
{
    PlayerInventory playerInventory(m_gameStub);

    EXPECT_EQ(playerInventory.GetBolts(), 0);
    EXPECT_EQ(playerInventory.GetNukes(), 0);
    EXPECT_EQ(playerInventory.GetPotions(), 0);
}
