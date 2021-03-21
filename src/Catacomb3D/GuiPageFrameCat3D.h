// Copyright (C) 2021 Arno Ansems
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

#include "..\Engine\GuiElementBase.h"
#include "..\Engine\EgaGraph.h"

class GuiPageFrameCat3D : public GuiElementBase
{
public:
    enum MenuHeader
    {
        MenuHeaderMain,
        MenuHeaderNewGame,
        MenuHeaderConfig,
        MenuHeaderVideo,
        MenuHeaderControls,
        MenuHeaderSound,
        MenuHeaderMusic,
        MenuHeaderLoadGame,
        MenuHeaderSaveGame
    };

    GuiPageFrameCat3D(
        const PlayerInput& playerInput,
        EgaGraph& egaGraph,
        const MenuHeader menuHeader,
        RenderableText& renderableText);

    virtual void Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const override;

    void SetInstructions(const std::string& firstLine, const std::string& secondLine, const std::string& thirdLine);

private:
    const MenuHeader m_menuHeader;
    RenderableText& m_renderableText;
    const Picture* m_pictureMainMenu;
    const Picture* m_pictureNewGame;
    const Picture* m_pictureConfig;
    const Picture* m_pictureControls;
    const Picture* m_pictureSound;
    const Picture* m_pictureMusic;
    const Picture* m_pictureLoadGame;
    const Picture* m_pictureSaveGame;
    std::string m_instructionsFirstLine;
    std::string m_instructionsSecondLine;
    std::string m_instructionsThirdLine;
};

