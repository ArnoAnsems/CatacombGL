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

#include "DecorateV112.h"
#include "DecorateAll.h"

DecorateV112::DecorateV112()
{
	for (const std::pair<uint16_t, const DecorateActor>& actorPair : decorateAbyssAll)
	{
		DecorateActor actorV112;
		actorV112.actionParameter = actorPair.second.actionParameter;
		actorV112.damage = actorPair.second.damage;
		actorV112.hitSound = actorPair.second.hitSound;
		actorV112.id = actorPair.second.id;
		actorV112.initialHealth = actorPair.second.initialHealth;
		actorV112.initialState = actorPair.second.initialState;
		actorV112.projectileId = actorPair.second.projectileId;
		actorV112.radarColor = actorPair.second.radarColor;
		actorV112.radarVisibility = actorPair.second.radarVisibility;
		actorV112.size = actorPair.second.size;
		actorV112.spawnOnAllDifficulties = actorPair.second.spawnOnAllDifficulties;
		actorV112.spawnOnHard = actorPair.second.spawnOnHard;
		actorV112.spawnOnNormalAndHard = actorPair.second.spawnOnNormalAndHard;
		actorV112.speed = actorPair.second.speed;
		for (const auto& originalState : actorPair.second.states)
		{
			DecorateAnimation animationV112;
			for (const auto& originalAnimationFrame : originalState.second.animation)
			{
				const DecorateAnimationFrame animationFrameV112 =
				{
					static_cast<uint16_t>(originalAnimationFrame.pictureIndex - 1u),
					originalAnimationFrame.durationInTics,
					originalAnimationFrame.action
				};
				animationV112.emplace_back(animationFrameV112);
			}
			const DecorateState stateV112 = { animationV112, originalState.second.nextState };
			actorV112.states.emplace(std::make_pair(originalState.first, stateV112));
		}
		const std::pair<uint16_t, const DecorateActor> actorPairV112 = std::make_pair(actorPair.first, actorV112);

		m_decorateAll.emplace(actorPairV112);
	}
}

const std::map<uint16_t, const DecorateActor>& DecorateV112::Get() const
{
	return m_decorateAll;
}