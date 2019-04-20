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
#pragma once

#include "Logging.h"
#include "IRenderer.h"
#include "PlayerInput.h"

class Console
{
public:
    Console(const Logging* logging, const std::string& label);
    ~Console();

    void Draw(IRenderer& renderer) const;
    void ProcessInput(PlayerInput& playerInput);

private:
    const Logging* m_logging;
    bool m_active;
    const std::string m_label;
};
