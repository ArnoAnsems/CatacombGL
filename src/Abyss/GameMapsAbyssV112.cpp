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

#include "GameMapsAbyssV112.h"
#include "GameMapsAbyss.h"

GameMapsAbyssV112::GameMapsAbyssV112()
{
	m_gameMapsStaticData = { "GAMEMAPS.ABS", gameMapsOffsetsAbyss, gameMapsInfoAbyss, {}, 15, 29 };
	for (const WallInfo& wallInfo : wallsInfoAbyss)
	{
		WallInfo wallInfoV112 = { {},{}, wallInfo.wallType };
		for (const uint16_t originalTextureLight : wallInfo.textureLight)
		{
			const uint16_t textureLightV112 = originalTextureLight - 1;
			wallInfoV112.textureLight.push_back(textureLightV112);
		}
		for (const uint16_t originalTextureDark : wallInfo.textureDark)
		{
			const uint16_t textureDarkV112 = originalTextureDark - 1;
			wallInfoV112.textureDark.push_back(textureDarkV112);
		}
		m_gameMapsStaticData.wallsInfo.emplace_back(wallInfoV112);
	}
}

const gameMapsStaticData& GameMapsAbyssV112::Get() const
{
	return m_gameMapsStaticData;
}
