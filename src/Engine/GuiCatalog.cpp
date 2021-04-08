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

#include "GuiCatalog.h"

GuiCatalog::GuiCatalog(
    PlayerInput& playerInput,
    const IRenderer& renderer,
    const std::vector<std::string>& catalogFilenames,
    const std::string& gameFolder) :
    GuiElementBase(playerInput),
    m_shapes(),
    m_currentPage(0),
    m_catalogFilenames(catalogFilenames),
    m_renderer(renderer),
    m_gameFolder(gameFolder)
{
    for (size_t i = 0; i < m_catalogFilenames.size(); i++)
    {
        m_shapes.push_back(nullptr);
    }
}

GuiCatalog::~GuiCatalog()
{

}

const GuiEvent& GuiCatalog::ProcessInput()
{
    if (m_playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        return m_closeEvent;
    }
    else if (m_playerInput.IsKeyJustPressed(SDLK_LEFT))
    {
        if (m_currentPage > 0)
        {
            m_currentPage--;
        }
    }
    else if (m_playerInput.IsKeyJustPressed(SDLK_RIGHT))
    {
        if (m_currentPage < m_catalogFilenames.size() - 1)
        {
            m_currentPage++;
        }
    }

    if (m_shapes.at(m_currentPage) == nullptr)
    {
        m_shapes.at(m_currentPage) = new Shape(m_renderer);
        m_shapes.at(m_currentPage)->LoadFromFile((m_gameFolder + "\\" + m_catalogFilenames.at(m_currentPage)).c_str());
    }

    return GetEvent();
}

void GuiCatalog::Draw(IRenderer& renderer) const
{
    const Shape* shape = m_shapes.at(m_currentPage);
    if (shape != nullptr)
    {
        const Picture* pic = shape->GetPicture();
        if (pic != nullptr)
        {
            if (m_currentPage > 0 && pic->GetImageHeight() < 200)
            {
                // Picture does not cover whole screen.
                // First draw background picture that does cover the whole screen
                uint16_t earlierPage = m_currentPage;
                Picture* backgroundPic = nullptr;
                while (backgroundPic == nullptr && earlierPage > 0)
                {
                    earlierPage--;
                    const Shape* earlierShape = m_shapes.at(earlierPage);
                    if (earlierShape != nullptr)
                    {
                        Picture* earlierPic = earlierShape->GetPicture();
                        if (earlierPic->GetImageHeight() == 200)
                        {
                            backgroundPic = earlierPic;
                        }
                    }
                }

                if (backgroundPic != nullptr)
                {
                    renderer.Render2DPicture(backgroundPic, 0, 0);
                }

            }

            // Draw the actual picture
            renderer.Render2DPicture(pic, 0, 0);
        }
    }
}