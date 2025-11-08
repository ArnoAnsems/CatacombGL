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
#include "GameSelection.h"
#include "DefaultFont.h"

GameSelection::GameSelection(IRenderer& renderer) :
    m_renderer(renderer)
{

}

GameSelection::~GameSelection()
{

}

void GameSelection::DrawBox(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const std::string& title, RenderableText& renderableText)
{
    constexpr uint8_t shadowHor = 1u;
    constexpr uint8_t shadowVert = 2u;
    constexpr uint8_t borderHor = 3u;
    constexpr uint8_t borderVert = 6u;

    // Outer shadow
    m_renderer.Render2DBar(x, y, width, shadowHor, EgaBrightWhite); // top
    m_renderer.Render2DBar(x, y + height - shadowHor, width, shadowHor, EgaDarkGray); // bottom
    m_renderer.Render2DBar(x, y + shadowHor, shadowVert, height - (2 * shadowHor), EgaDarkGray); // left
    m_renderer.Render2DBar(x + width - shadowVert, y + shadowHor, shadowVert, height - (2 * shadowHor), EgaBrightWhite); // right

    // Border
    m_renderer.Render2DBar(x + shadowVert, y + shadowHor, width - (2 * shadowVert), borderHor, EgaLightGray); // top
    m_renderer.Render2DBar(x + shadowVert, y + height - shadowHor - borderHor, width - (2 * shadowVert), borderHor, EgaLightGray); // bottom
    m_renderer.Render2DBar(x + shadowVert, y + shadowHor + borderHor, borderVert, height - (2 * (borderHor + shadowHor)), EgaLightGray); // left
    m_renderer.Render2DBar(x + width - shadowVert - borderVert, y + shadowHor + borderHor, borderVert, height - (2 * (borderHor + shadowHor)), EgaLightGray); // right

    // Inner shadow
    m_renderer.Render2DBar(x + shadowVert + borderVert, y + shadowHor + borderHor, width - (2 * (shadowVert + borderVert)), shadowHor, EgaBlue); // top
    m_renderer.Render2DBar(x + shadowVert + borderVert, y + height - (2 * shadowHor) - borderHor, width - (2 * (shadowVert + borderVert)), shadowHor, EgaBrightBlue); // bottom
    m_renderer.Render2DBar(x + shadowVert + borderVert, y + (2 * shadowHor) + borderHor, shadowVert, height - (4 * shadowHor) - (2 * borderHor), EgaBrightBlue); // left
    m_renderer.Render2DBar(x + width - (2 * shadowVert) - borderVert, y + (2 * shadowHor) + borderHor, shadowVert, height - (4 * shadowHor) - (2 * borderHor), EgaBlue); // right

    // Inner area
    m_renderer.Render2DBar(x + (2 * shadowVert) + borderVert, y + (2 * shadowHor) + borderHor, width - (4 * shadowVert) - (2 * borderVert), height - (4 * shadowHor) - (2 * borderHor), EgaDarkGray);

    if (!title.empty())
    {
        m_renderer.Render2DBar(x + (2 * shadowVert) + borderVert, y + (2 * shadowHor) + borderHor, width - (4 * shadowVert) - (2 * borderVert), 11, EgaLightGray);
        renderableText.Centered(title, EgaBlack, x + (width / 2), y + (2 * shadowHor) + borderHor + 1);
    }
}

void GameSelection::Draw(const GameSelectionPresentation& presentation)
{
    const Font& defaultFont = *DefaultFont::Get(m_renderer, 10);
    const ViewPorts::ViewPortRect3D dummy3DViewArea = { 0, 320, 0, 200 };
    RenderableText renderableText(defaultFont);
    m_renderer.Prepare2DRendering(true);
    DrawBox(2, 2, 636, 44, "Catacomb Games Library", renderableText);
    renderableText.LeftAligned("Launch a game by pressing the corresponding number [1-7] on the keyboard.", EgaBrightWhite, 18, 19);
    renderableText.LeftAligned("In case a game is not found, use the ENTER and arrow keys to browse to the game data.", EgaBrightWhite, 18, 29);
    DrawBox(2, 48, 316, 66, "Catacombs Pack (GOG)", renderableText);
    DrawBox(322, 48, 316, 44, "Shareware", renderableText);
    DrawBox(322, 94, 316, 34, "Catacomb 3-D Big Box", renderableText);
    for (uint8_t gameIndex = 0; gameIndex < presentation.gameListCatacombsPack.size(); gameIndex++)
    {
        const std::string& gameName = presentation.gameListCatacombsPack.at(gameIndex).first;
        renderableText.LeftAligned(gameName, EgaBrightWhite, 18, 66 + (10 * gameIndex));
        const GameDetectionState detectionState = presentation.gameListCatacombsPack.at(gameIndex).second;
        const char* detectionStateStr =
            (detectionState == Detected) ? "Ready" :
            (detectionState == NotDetected) ? "Not Found" :
            "N/A";
        const egaColor detectionStateColor =
            (detectionState == Detected) ? EgaGreen :
            (detectionState == NotDetected) ? EgaRed :
            EgaLightGray;
        renderableText.Centered(detectionStateStr, detectionStateColor, 274, 66 + (10 * gameIndex));
    }
    for (uint8_t gameIndex = 0; gameIndex < presentation.gameListShareware.size(); gameIndex++)
    {
        const std::string& gameName = presentation.gameListShareware.at(gameIndex).first;
        renderableText.LeftAligned(gameName, EgaBrightWhite, 338, 66 + (10 * gameIndex));
        const GameDetectionState detectionState = presentation.gameListShareware.at(gameIndex).second;
        const char* detectionStateStr =
            (detectionState == Detected) ? "Ready" :
            (detectionState == NotDetected) ? "Not Found" :
            "N/A";
        const egaColor detectionStateColor =
            (detectionState == Detected) ? EgaGreen :
            (detectionState == NotDetected) ? EgaRed :
            EgaLightGray;
        renderableText.Centered(detectionStateStr, detectionStateColor, 594, 66 + (10 * gameIndex));
    }
    for (uint8_t gameIndex = 0; gameIndex < presentation.gameListCatacomb3DBigBox.size(); gameIndex++)
    {
        const std::string& gameName = presentation.gameListCatacomb3DBigBox.at(gameIndex).first;
        renderableText.LeftAligned(gameName, EgaBrightWhite, 338, 112 + (10 * gameIndex));
        const GameDetectionState detectionState = presentation.gameListCatacomb3DBigBox.at(gameIndex).second;
        const char* detectionStateStr =
            (detectionState == Detected) ? "Ready" :
            (detectionState == NotDetected) ? "Not Found" :
            "N/A";
        const egaColor detectionStateColor =
            (detectionState == Detected) ? EgaGreen :
            (detectionState == NotDetected) ? EgaRed :
            EgaLightGray;
        renderableText.Centered(detectionStateStr, detectionStateColor, 594, 112 + (10 * gameIndex));
    }

    DrawBox(2, 116, 316, 82, "Browse", renderableText);
    DrawBox(318, 130, 320, 68, "", renderableText);

    // Connect the two boxes for the Browse window by drawing over the borders.
    m_renderer.Render2DBar(310, 131, 16, 3, EgaLightGray);
    m_renderer.Render2DBar(310, 134, 16, 1, EgaBlue);
    m_renderer.Render2DBar(308, 135, 20, 58, EgaDarkGray);
    m_renderer.Render2DBar(310, 193, 16, 1, EgaBrightBlue);
    m_renderer.Render2DBar(310, 194, 16, 3, EgaLightGray);

    renderableText.LeftAlignedTruncated(presentation.searchFolder.string(), EgaBrightYellow, 18, 138, 550);
    const uint32_t maxSubFoldersVisible = 4;
    const uint32_t endOfSubFolders = ((uint32_t)presentation.subFolders.size() - presentation.subFolderOffset > maxSubFoldersVisible ? presentation.subFolderOffset + maxSubFoldersVisible : (uint32_t)presentation.subFolders.size());
    for (uint32_t subFolderIndex = presentation.subFolderOffset; subFolderIndex < endOfSubFolders; subFolderIndex++)
    {
        const std::string& subFolderName = presentation.subFolders.at(subFolderIndex).filename().string();
        const std::string& nameToRender = (!subFolderName.empty()) ? subFolderName : presentation.subFolders.at(subFolderIndex).root_path().string();
        const egaColor subFolderColor = (subFolderIndex == presentation.selectedSubFolder) ? EgaBrightCyan : EgaBrightWhite;
        renderableText.LeftAlignedTruncated(nameToRender, subFolderColor, 22, 148 + (10 * (subFolderIndex - presentation.subFolderOffset)), 550);
    }
    m_renderer.RenderText(renderableText);
    m_renderer.Unprepare2DRendering();
}
