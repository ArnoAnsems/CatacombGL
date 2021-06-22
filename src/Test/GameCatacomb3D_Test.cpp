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

#include "GameCatacomb3D_Test.h"
#include "../Catacomb3D/GameCatacomb3D.h"
#include "RendererStub.h"

GameCatacomb3D_Test::GameCatacomb3D_Test()
{

}

GameCatacomb3D_Test::~GameCatacomb3D_Test()
{

}

TEST(GameCatacomb3D_Test, CheckGetDecorateActors)
{
    RendererStub renderer;
    GameCatacomb3D gameCatacomb3D(".\\", ".\\", renderer);
    auto decorateActors = gameCatacomb3D.GetDecorateActors();
    ASSERT_GT(decorateActors.size(), 0u);

    // Check each actor
    for (auto decorateActorPair : decorateActors)
    {
        // id in map should match with Actor id
        EXPECT_EQ(decorateActorPair.first, decorateActorPair.second.id);

        // Each actor must have one or more states
        ASSERT_GT(decorateActorPair.second.states.size(), 0u);

        // Initial state must be present in states list
        EXPECT_NE(decorateActorPair.second.states.end(), decorateActorPair.second.states.find(decorateActorPair.second.initialState));

        // Check each state
        for (auto decorateStatePair : decorateActorPair.second.states)
        {
            // Each state animation must contain at least one frame
            EXPECT_GE(decorateStatePair.second.animation.size(), 0u);

            // The next state must be present in states list
            EXPECT_NE(decorateActorPair.second.states.end(), decorateActorPair.second.states.find(decorateStatePair.second.nextState));
        }
    }
}