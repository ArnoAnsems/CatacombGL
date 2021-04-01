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

#include "GuiPageFrameCat3D.h"
#include "EgaGraphCatacomb3D.h"

GuiPageFrameCat3D::GuiPageFrameCat3D(
    const PlayerInput& playerInput,
    EgaGraph& egaGraph,
    const MenuHeader menuHeader,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_menuHeader(menuHeader),
    m_renderableText(renderableText),
    m_pictureMainMenu(egaGraph.GetPicture(CP_MAINMENUPIC)),
    m_pictureNewGame(egaGraph.GetPicture(CP_NEWGAMEMENUPIC)),
    m_pictureConfig(egaGraph.GetPicture(CP_CONFIGMENUPIC)),
    m_pictureControls(egaGraph.GetPicture(CP_KEYBUTTONPIC)),
    m_pictureSound(egaGraph.GetPicture(CP_SOUNDMENUPIC)),
    m_pictureMusic(egaGraph.GetPicture(CP_MUSICMENUPIC)),
    m_pictureLoadGame(egaGraph.GetPicture(CP_LOADMENUPIC)),
    m_pictureSaveGame(egaGraph.GetPicture(CP_SAVEMENUPIC)),
    m_instructionsFirstLine(""),
    m_instructionsSecondLine(""),
    m_instructionsThirdLine("")
{

}

void GuiPageFrameCat3D::Draw(IRenderer& renderer) const
{
    renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
    renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
    if (m_menuHeader == MenuHeaderMain)
    {
        renderer.Render2DPicture(m_pictureMainMenu, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderNewGame)
    {
        renderer.Render2DPicture(m_pictureNewGame, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderConfig)
    {
        renderer.Render2DPicture(m_pictureConfig, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderVideo)
    {
        // The header for the Video Menu is composed out of letters from other menu headers, as it did not exist
        // in the original game.
        renderer.Render2DPictureSegment(m_pictureSaveGame, 80, 48, 16, 0, 8, 12); // V
        renderer.Render2DPictureSegment(m_pictureMainMenu, 88, 48, 19, 0, 3, 12); // I
        renderer.Render2DPictureSegment(m_pictureLoadGame, 91, 48, 24, 0, 7, 12); // D
        renderer.Render2DPictureSegment(m_pictureSaveGame, 98, 48, 24, 0, 7, 12); // E
        renderer.Render2DPictureSegment(m_pictureLoadGame, 105, 48, 10, 0, 7, 12); // O
        renderer.Render2DPictureSegment(m_pictureMainMenu, 112, 48, 30, 0, 34, 12); // MENU
    }
    else if (m_menuHeader == MenuHeaderControls)
    {
        renderer.Render2DPicture(m_pictureControls, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderSound)
    {
        renderer.Render2DPicture(m_pictureSound, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderMusic)
    {
        renderer.Render2DPicture(m_pictureMusic, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderLoadGame)
    {
        renderer.Render2DPicture(m_pictureLoadGame, 80, 48);
    }
    else if (m_menuHeader == MenuHeaderSaveGame)
    {
        renderer.Render2DPicture(m_pictureSaveGame, 80, 48);
    }

    m_renderableText.LeftAligned(m_instructionsFirstLine, EgaRed, 78, 135);
    m_renderableText.LeftAligned(m_instructionsSecondLine, EgaRed, 163, 135);
    m_renderableText.Centered(m_instructionsThirdLine, EgaRed, 154, 144);
}

void GuiPageFrameCat3D::SetInstructions(const std::string& firstLine, const std::string& secondLine, const std::string& thirdLine)
{
    m_instructionsFirstLine = firstLine;
    m_instructionsSecondLine = secondLine;
    m_instructionsThirdLine = thirdLine;
}