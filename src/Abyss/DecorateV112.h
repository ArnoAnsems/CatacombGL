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

//
// DecorateV112
//
// Decorate structures for the actors in Catacomb Abyss v1.12.
// Compared to Catacomb Abyss v1.13 and v1.24, the EGAGRAPH file of Catacomb Abyss v1.12
// lacks the first picture (FINALEPIC). Therefore, all indexes to pictures need to be lowered by one.
// The conversion of picture indexes in the decorate structures is handled via this class.
//
#pragma once

#include "../Engine/Decorate.h"

class DecorateV112
{
public:
	DecorateV112();
	const std::map<uint16_t, const DecorateActor>& Get() const;

private:
	std::map<uint16_t, const DecorateActor> m_decorateAll;
};