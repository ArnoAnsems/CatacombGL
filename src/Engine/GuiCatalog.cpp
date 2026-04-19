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
#include "Shape.h"
#include "IRenderer.h"
#include "PlayerInput.h"
#include "Picture.h"

namespace fs = std::filesystem;

GuiCatalog::GuiCatalog(
    PlayerInput& playerInput,
    const IRenderer& renderer,
    const std::vector<std::string>& catalogFilenames,
    const fs::path& gameFolder) :
    GuiElementBase(playerInput),
    m_shapes(),
    m_catalogFilenames(catalogFilenames),
    m_currentPage(0),
    m_mostRecentPageWithFullscreenImage(0),
    m_renderer(renderer),
    m_gameFolder(gameFolder)
{
    for (size_t i = 0; i < m_catalogFilenames.size(); i++)
    {
        m_shapes.push_back(nullptr);
    }
}

const GuiEvent& GuiCatalog::ProcessInput()
{
    const uint16_t lastPage = (uint16_t)m_catalogFilenames.size() - 1;
    if (m_playerInput.IsKeyJustPressed(SDLK_ESCAPE) || m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_B))
    {
        m_mostRecentPageWithFullscreenImage = 0;
        m_currentPage = 0;
        return m_closeEvent;
    }
    else if (m_playerInput.IsKeyJustPressed(SDLK_LEFT) ||
             m_playerInput.IsKeyJustPressed(SDLK_UP) ||
             m_playerInput.IsKeyJustPressed(SDLK_PAGEUP) ||
             m_playerInput.IsKeyJustPressed(SDLK_KP_9) ||
             m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_DPAD_LEFT) ||
             m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_DPAD_UP) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsNegative(SDL_CONTROLLER_AXIS_LEFTX) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsNegative(SDL_CONTROLLER_AXIS_RIGHTX) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsNegative(SDL_CONTROLLER_AXIS_LEFTY) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsNegative(SDL_CONTROLLER_AXIS_RIGHTY))
    {
        if (m_currentPage > 0)
        {
            m_currentPage--;
        }
        else
        {
            m_currentPage = lastPage;
        }
    }
    else if (m_playerInput.IsKeyJustPressed(SDLK_RIGHT) ||
             m_playerInput.IsKeyJustPressed(SDLK_DOWN) ||
             m_playerInput.IsKeyJustPressed(SDLK_PAGEDOWN) ||
             m_playerInput.IsKeyJustPressed(SDLK_KP_3) ||
             m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ||
             m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsPositive(SDL_CONTROLLER_AXIS_LEFTX) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsPositive(SDL_CONTROLLER_AXIS_RIGHTX) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsPositive(SDL_CONTROLLER_AXIS_LEFTY) ||
             m_playerInput.GetGameControllerAxisJustPressedTowardsPositive(SDL_CONTROLLER_AXIS_RIGHTY))

    {
        if (m_currentPage < lastPage)
        {
            m_currentPage++;
        }
        else
        {
            m_currentPage = 0;
        }
    }
    else if (m_playerInput.IsKeyJustPressed(SDLK_HOME) ||
             m_playerInput.IsKeyJustPressed(SDLK_KP_7))
    {
        m_currentPage = 0;
    }
    else if (m_playerInput.IsKeyJustPressed(SDLK_END) ||
             m_playerInput.IsKeyJustPressed(SDLK_KP_1))
    {
        m_currentPage = lastPage;
    }

    if (m_shapes.at(m_currentPage) == nullptr)
    {
        m_shapes.at(m_currentPage) = new Shape(m_renderer);
        m_shapes.at(m_currentPage)->LoadFromFile(m_gameFolder / m_catalogFilenames.at(m_currentPage));
    }

    const Picture* pic = m_shapes.at(m_currentPage)->GetPicture();
    if (pic != nullptr)
    {
        if (pic->GetImageHeight() == 200)
        {
            // This is a fullscreen image
            m_mostRecentPageWithFullscreenImage = m_currentPage;
        }
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
                // First draw a background picture that does cover the whole screen
                const Shape* backgroundShape = m_shapes.at(m_mostRecentPageWithFullscreenImage);
                if (backgroundShape != nullptr)
                {
                    Picture* backgroundPic = backgroundShape->GetPicture();
                    if (backgroundPic != nullptr)
                    {
                        renderer.Render2DPicture(backgroundPic, 0, 0);
                    }
                }
            }

            // Draw the actual picture
            renderer.Render2DPicture(pic, 0, 0);
        }
    }
}
