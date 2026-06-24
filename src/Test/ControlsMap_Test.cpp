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
#include "SDL3/SDL_mouse.h"

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

TEST_F(ControlsMap_Test, AssignTwoKeysToAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(UsePotion, SDLK_A));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(UsePotion, SDLK_SPACE));

    const auto keysFromAction = m_controlsMap.GetKeysFromAction(UsePotion);
    ASSERT_EQ(keysFromAction.size(), 2);
    EXPECT_EQ(keysFromAction.at(0), SDLK_SPACE);
    EXPECT_EQ(keysFromAction.at(1), SDLK_A);
}

TEST_F(ControlsMap_Test, AssignOneKeyAndOneMouseButtonToAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShootZappper, SDLK_Z));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(ShootZappper, SDL_BUTTON_LEFT));

    const auto keysFromAction = m_controlsMap.GetKeysFromAction(ShootZappper);
    ASSERT_EQ(keysFromAction.size(), 1);
    EXPECT_EQ(keysFromAction.at(0), SDLK_Z);
    const auto mouseButtonsFromAction = m_controlsMap.GetMouseButtonsFromAction(ShootZappper);
    ASSERT_EQ(mouseButtonsFromAction.size(), 1);
    EXPECT_EQ(mouseButtonsFromAction.at(0), SDL_BUTTON_LEFT);
}

TEST_F(ControlsMap_Test, WhenAThirdKeyIsAssignedToAnActionThenThePreviousTwoAreErased)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShootZappper, SDLK_E));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShootZappper, SDLK_F));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShootZappper, SDLK_G));

    const auto keysFromAction = m_controlsMap.GetKeysFromAction(ShootZappper);
    ASSERT_EQ(keysFromAction.size(), 1);
    EXPECT_EQ(keysFromAction.at(0), SDLK_G);
}

TEST_F(ControlsMap_Test, WhenTheSameKeyIsAssignedTwiceToAnActionThenIgnore)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShowAutoMap, SDLK_H));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShowAutoMap, SDLK_I));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(ShowAutoMap, SDLK_H));

    const auto keysFromAction = m_controlsMap.GetKeysFromAction(ShowAutoMap);
    ASSERT_EQ(keysFromAction.size(), 2);
    EXPECT_EQ(keysFromAction.at(0), SDLK_H);
    EXPECT_EQ(keysFromAction.at(1), SDLK_I);
}

TEST_F(ControlsMap_Test, WhenNotAllowedKeyIsAssignedToAnActionThenIgnore)
{
    EXPECT_FALSE(m_controlsMap.AssignActionToKey(QuickTurn, SDLK_1));
    EXPECT_FALSE(m_controlsMap.AssignActionToKey(QuickTurn, SDLK_ESCAPE));

    EXPECT_TRUE(m_controlsMap.GetKeysFromAction(QuickTurn).empty());
}

TEST_F(ControlsMap_Test, WhenKeyIsAssignedToActionNoneThenItIsRemovedFromPreviousAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(MoveBackward, SDLK_U));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(MoveBackward, SDLK_V));
    EXPECT_TRUE(m_controlsMap.AssignActionToKey(None, SDLK_U));

    const auto keysFromAction = m_controlsMap.GetKeysFromAction(MoveBackward);
    ASSERT_EQ(keysFromAction.size(), 1);
    EXPECT_EQ(keysFromAction.at(0), SDLK_V);
}

TEST_F(ControlsMap_Test, AssignTwoMouseButtonsToAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(UsePotion, SDL_BUTTON_X1));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(UsePotion, SDL_BUTTON_X2));

    const auto mouseButtonsFromAction = m_controlsMap.GetMouseButtonsFromAction(UsePotion);
    ASSERT_EQ(mouseButtonsFromAction.size(), 2);
    EXPECT_EQ(mouseButtonsFromAction.at(0), SDL_BUTTON_X1);
    EXPECT_EQ(mouseButtonsFromAction.at(1), SDL_BUTTON_X2);
}

TEST_F(ControlsMap_Test, WhenAThirdMouseButtonIsAssignedToAnActionThenThePreviousTwoAreErased)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(Shoot, SDL_BUTTON_LEFT));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(Shoot, SDL_BUTTON_MIDDLE));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(Shoot, SDL_BUTTON_RIGHT));

    const auto mouseButtonsFromAction = m_controlsMap.GetMouseButtonsFromAction(Shoot);
    ASSERT_EQ(mouseButtonsFromAction.size(), 1);
    EXPECT_EQ(mouseButtonsFromAction.at(0), SDL_BUTTON_RIGHT);
}

TEST_F(ControlsMap_Test, WhenTheSameMouseButtonIsAssignedTwiceToAnActionThenIgnore)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(ShootXterminator, SDL_BUTTON_X2));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(ShootXterminator, SDL_BUTTON_LEFT));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(ShootXterminator, SDL_BUTTON_X2));

    const auto mouseButtonsFromAction = m_controlsMap.GetMouseButtonsFromAction(ShootXterminator);
    ASSERT_EQ(mouseButtonsFromAction.size(), 2);
    EXPECT_EQ(mouseButtonsFromAction.at(0), SDL_BUTTON_LEFT);
    EXPECT_EQ(mouseButtonsFromAction.at(1), SDL_BUTTON_X2);
}

TEST_F(ControlsMap_Test, WhenNotAllowedMouseButtonIsAssignedToAnActionThenIgnore)
{
    EXPECT_FALSE(m_controlsMap.AssignActionToMouseButton(StrafeLeft, 6u));
    EXPECT_FALSE(m_controlsMap.AssignActionToMouseButton(StrafeLeft, 255u));

    EXPECT_TRUE(m_controlsMap.GetMouseButtonsFromAction(StrafeLeft).empty());
}

TEST_F(ControlsMap_Test, WhenMouseButtonIsAssignedToActionNoneThenItIsRemovedFromPreviousAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(TurnRight, SDL_BUTTON_LEFT));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(TurnRight, SDL_BUTTON_MIDDLE));
    EXPECT_TRUE(m_controlsMap.AssignActionToMouseButton(None, SDL_BUTTON_LEFT));

    const auto mouseButtonsFromAction = m_controlsMap.GetMouseButtonsFromAction(TurnRight);
    ASSERT_EQ(mouseButtonsFromAction.size(), 1);
    EXPECT_EQ(mouseButtonsFromAction.at(0), SDL_BUTTON_MIDDLE);
}

TEST_F(ControlsMap_Test, AssignTwoGameControllerButtonsToAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(UsePotion, SDL_GAMEPAD_BUTTON_SOUTH));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(UsePotion, SDL_GAMEPAD_BUTTON_WEST));

    const auto gameControllerButtonsFromAction = m_controlsMap.GetGameControllerButtonsFromAction(UsePotion);
    ASSERT_EQ(gameControllerButtonsFromAction.size(), 2);
    EXPECT_EQ(gameControllerButtonsFromAction.at(0), SDL_GAMEPAD_BUTTON_SOUTH);
    EXPECT_EQ(gameControllerButtonsFromAction.at(1), SDL_GAMEPAD_BUTTON_WEST);
}

TEST_F(ControlsMap_Test, AssignOneGameControllerButtonAndOneAxisToAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(ControlAction::Run, SDL_GAMEPAD_BUTTON_TOUCHPAD));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(ControlAction::Run, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));

    const auto gameControllerButtonsFromAction = m_controlsMap.GetGameControllerButtonsFromAction(ControlAction::Run);
    ASSERT_EQ(gameControllerButtonsFromAction.size(), 1);
    EXPECT_EQ(gameControllerButtonsFromAction.at(0), SDL_GAMEPAD_BUTTON_TOUCHPAD);
    const auto gameControllerAxisFromAction = m_controlsMap.GetGameControllerAxisFromAction(ControlAction::Run);
    ASSERT_EQ(gameControllerAxisFromAction.size(), 1);
    EXPECT_EQ(gameControllerAxisFromAction.at(0), SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
}

TEST_F(ControlsMap_Test, WhenAThirdGameControlButtonIsAssignedToAnActionThenThePreviousTwoAreErased)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(Shoot, SDL_GAMEPAD_BUTTON_DPAD_UP));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(Shoot, SDL_GAMEPAD_BUTTON_MISC1));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(Shoot, SDL_GAMEPAD_BUTTON_BACK));

    const auto gameControllerButtonsFromAction = m_controlsMap.GetGameControllerButtonsFromAction(Shoot);
    ASSERT_EQ(gameControllerButtonsFromAction.size(), 1);
    EXPECT_EQ(gameControllerButtonsFromAction.at(0), SDL_GAMEPAD_BUTTON_BACK);
}

TEST_F(ControlsMap_Test, WhenTheSameGameControlButtonIsAssignedTwiceToAnActionThenIgnore)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(QuickTurn, SDL_GAMEPAD_BUTTON_DPAD_LEFT));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(QuickTurn, SDL_GAMEPAD_BUTTON_DPAD_RIGHT));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(QuickTurn, SDL_GAMEPAD_BUTTON_DPAD_LEFT));

    const auto gameControllerButtonsFromAction = m_controlsMap.GetGameControllerButtonsFromAction(QuickTurn);
    ASSERT_EQ(gameControllerButtonsFromAction.size(), 2);
    EXPECT_EQ(gameControllerButtonsFromAction.at(0), SDL_GAMEPAD_BUTTON_DPAD_LEFT);
    EXPECT_EQ(gameControllerButtonsFromAction.at(1), SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
}

TEST_F(ControlsMap_Test, WhenNotAllowedGameControlButtonIsAssignedToAnActionThenIgnore)
{
    EXPECT_FALSE(m_controlsMap.AssignActionToGameControllerButton(Shoot, SDL_GAMEPAD_BUTTON_START));
}

TEST_F(ControlsMap_Test, WhenGameControllerButtonIsAssignedToActionNoneThenItIsRemovedFromThePreviousAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(TurnRight, SDL_GAMEPAD_BUTTON_TOUCHPAD));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(TurnRight, SDL_GAMEPAD_BUTTON_DPAD_DOWN));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerButton(None, SDL_GAMEPAD_BUTTON_TOUCHPAD));

    const auto gameControllerButtonsFromAction = m_controlsMap.GetGameControllerButtonsFromAction(TurnRight);
    ASSERT_EQ(gameControllerButtonsFromAction.size(), 1);
    EXPECT_EQ(gameControllerButtonsFromAction.at(0), SDL_GAMEPAD_BUTTON_DPAD_DOWN);
}

TEST_F(ControlsMap_Test, AssignTwoGameControllerAxisToAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(UsePotion, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(UsePotion, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));

    const auto gameControllerButtonsFromAxis = m_controlsMap.GetGameControllerAxisFromAction(UsePotion);
    ASSERT_EQ(gameControllerButtonsFromAxis.size(), 2);
    EXPECT_EQ(gameControllerButtonsFromAxis.at(0), SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
    EXPECT_EQ(gameControllerButtonsFromAxis.at(1), SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
}

TEST_F(ControlsMap_Test, WhenTheSameGameControllerAxisIsAssignedTwiceToAnActionThenIgnore)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(MoveForward, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(MoveForward, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(MoveForward, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));

    const auto gameControllerButtonsFromAxis = m_controlsMap.GetGameControllerAxisFromAction(MoveForward);
    ASSERT_EQ(gameControllerButtonsFromAxis.size(), 2);
    EXPECT_EQ(gameControllerButtonsFromAxis.at(0), SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
    EXPECT_EQ(gameControllerButtonsFromAxis.at(1), SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
}

TEST_F(ControlsMap_Test, WhenNotAllowedGameControlAxisIsAssignedToAnActionThenIgnore)
{
    EXPECT_FALSE(m_controlsMap.AssignActionToGameControllerAxis(Shoot, SDL_GAMEPAD_AXIS_LEFTX));
    EXPECT_FALSE(m_controlsMap.AssignActionToGameControllerAxis(Shoot, SDL_GAMEPAD_AXIS_RIGHTY));
}

TEST_F(ControlsMap_Test, WhenGameControllerAxisIsAssignedToActionNoneThenItIsRemovedFromThePreviousAction)
{
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(TurnRight, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(TurnRight, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER));
    EXPECT_TRUE(m_controlsMap.AssignActionToGameControllerAxis(None, SDL_GAMEPAD_AXIS_LEFT_TRIGGER));

    const auto gameControllerAxisFromAction = m_controlsMap.GetGameControllerAxisFromAction(TurnRight);
    ASSERT_EQ(gameControllerAxisFromAction.size(), 1);
    EXPECT_EQ(gameControllerAxisFromAction.at(0), SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
}

TEST_F(ControlsMap_Test, WhenGameControllerButtonNameThenNameIsUnique)
{
    for (uint8_t i = static_cast<uint8_t>(SDL_GAMEPAD_BUTTON_SOUTH); i < static_cast<uint8_t>(SDL_GAMEPAD_BUTTON_COUNT); i++)
    {
        const SDL_GamepadButton firstButton = static_cast<SDL_GamepadButton>(i);
        const std::string firstButtonName = ControlsMap::GetGameControllerButtonName(firstButton);
        EXPECT_FALSE(firstButtonName.empty());
        for (uint8_t j = static_cast<uint8_t>(SDL_GAMEPAD_BUTTON_SOUTH); j < static_cast<uint8_t>(SDL_GAMEPAD_BUTTON_COUNT); j++)
        {
            const SDL_GamepadButton secondButton = static_cast<SDL_GamepadButton>(j);
            if (firstButton != secondButton)
            {
                const std::string secondButtonName = ControlsMap::GetGameControllerButtonName(secondButton);
                EXPECT_NE(firstButtonName, secondButtonName);
            }
        }
    }
}