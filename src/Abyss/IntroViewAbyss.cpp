// Copyright (C) 2018 Arno Ansems
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

#include "IntroViewAbyss.h"

namespace fs = std::filesystem;

IntroViewAbyss::IntroViewAbyss(GameId gameId, IRenderer& renderer, const fs::path& path) :
    IIntroView(renderer),
    m_gameId(gameId)
{
    if (m_gameId == GameId::CatacombAbyssv112)
    {
        m_shapeSoftdisk = new Shape(renderer);
        const fs::path sdlogo = path / "SDLOGO.ABS";
        m_shapeSoftdisk->LoadFromFile(sdlogo);

        m_shapeTitle = new Shape(renderer);
        const fs::path title = path / "TITLE.ABS";
        m_shapeTitle->LoadFromFile(title);

        m_shapeCredits = new Shape(renderer);
        const fs::path credits = path / "CREDITS.ABS";
        m_shapeCredits->LoadFromFile(credits);
    }
    else
    {
        m_shapeEntering = new Shape(renderer);
        const fs::path shp05 = path / "SHP05.ABS";
        m_shapeEntering->LoadFromFile(shp05);

        m_shapePresents = new Shape(renderer);
        const fs::path shp12 = path / "SHP12.ABS";
        m_shapePresents->LoadFromFile(shp12);

        m_shapeSoftdisk = new Shape(renderer);
        const fs::path shp01 = path / "SHP01.ABS";
        m_shapeSoftdisk->LoadFromFile(shp01);

        m_shapeTitle = new Shape(renderer);
        const fs::path shp02 = path / "SHP02.ABS";
        m_shapeTitle->LoadFromFile(shp02);

        m_shapeCredits = new Shape(renderer);
        const fs::path shp03 = path / "SHP03.ABS";
        m_shapeCredits->LoadFromFile(shp03);

        m_shapeTrilogy = new Shape(renderer);
        const fs::path shp11 = path / "SHP11.ABS";
        m_shapeTrilogy->LoadFromFile(shp11);

        m_shapeSelectDifficulty = new Shape(renderer);
        const fs::path shp07 = path / "SHP07.ABS";
        m_shapeSelectDifficulty->LoadFromFile(shp07);

        m_shapeConfirmDifficulty = new Shape(renderer);
        const fs::path shp06 = path / "SHP06.ABS";
        m_shapeConfirmDifficulty->LoadFromFile(shp06);

        m_shapeNovice = new Shape(renderer);
        const fs::path shp08 = path / "SHP08.ABS";
        m_shapeNovice->LoadFromFile(shp08);

        m_shapeWarrior = new Shape(renderer);
        const fs::path shp09 = path / "SHP09.ABS";
        m_shapeWarrior->LoadFromFile(shp09);

        m_shapeStandBeforeGate = new Shape(renderer);
        const fs::path shp04 = path / "SHP04.ABS";
        m_shapeStandBeforeGate->LoadFromFile(shp04);

        // SHP04 = Stand before gate
        // SHP05 = Prepare
        // SHP06 = Nemesis
        // SHP08 = Ha! Another novice!
        // SHP09 = Warriors blood!
        // SHP10 = Back for more!
        // SHP11 = Trilogy
    }
}

IntroViewAbyss::~IntroViewAbyss()
{
    delete m_shapeEntering;
    m_shapeEntering = nullptr;

    delete m_shapePresents;
    m_shapePresents = nullptr;

    delete m_shapeSoftdisk;
    m_shapeSoftdisk = nullptr;

    delete m_shapeTitle;
    m_shapeTitle = nullptr;

    delete m_shapeCredits;
    m_shapeCredits = nullptr;

    delete m_shapeTrilogy;
    m_shapeTrilogy = nullptr;

    delete m_shapeSelectDifficulty;
    m_shapeSelectDifficulty = nullptr;

    delete m_shapeConfirmDifficulty;
    m_shapeConfirmDifficulty = nullptr;

    delete m_shapeNovice;
    m_shapeNovice = nullptr;

    delete m_shapeWarrior;
    m_shapeWarrior = nullptr;

    delete m_shapeStandBeforeGate;
    m_shapeStandBeforeGate = nullptr;
}

void IntroViewAbyss::DrawIntroduction(const uint32_t timeStamp)
{
    if (m_gameId == GameId::CatacombAbyssv112)
    {
        if (timeStamp < 100)
        {
            // Start from a black screen
            m_renderer.Render2DBar(0, 0, 320, 200, EgaBlack);
            if (m_fadeEffect.OverlayActive())
            {
                m_fadeEffect.SetOverlay(m_renderer);
            }
        }
        else
        {
            const uint8_t pictureIndex = (timeStamp / 5000) % 3;
            switch (pictureIndex)
            {
            case 0:
            {
                m_renderer.Render2DPicture(m_shapeSoftdisk->GetPicture(), 96u, 65u);
                break;
            }
            case 1:
            {
                m_renderer.Render2DPicture(m_shapeTitle->GetPicture(), 0u, 0u);
                break;
            }
            case 2:
            {
                m_renderer.Render2DPicture(m_shapeCredits->GetPicture(), 0u, 0u);
                break;
            }
            }

            if ((timeStamp % 5000) > 4000)
            {
                if (m_fadeEffect.OverlayActive())
                {
                    m_fadeEffect.SetOverlay(m_renderer);
                }
            }

            if ((timeStamp % 5000) < 1000)
            {
                const uint32_t milliSec = timeStamp % 5000;
                m_fadeEffect.DrawOverlay(m_renderer, milliSec);
            }
        }
    }
    else
    {
        if (timeStamp < 5000)
        {
            m_renderer.Render2DPicture(m_shapeEntering->GetPicture(), 20, 72);
        }
        else
        {
            const uint8_t pictureIndex = ((timeStamp - 5000) / 5000) % 5;
            switch (pictureIndex)
            {
            case 0:
            {
                m_renderer.Render2DPicture(m_shapePresents->GetPicture(), m_shapePresents->GetOffsetX(), m_shapePresents->GetOffsetY());
                break;
            }
            case 1:
            {
                m_renderer.Render2DPicture(m_shapeSoftdisk->GetPicture(), m_shapeSoftdisk->GetOffsetX(), m_shapeSoftdisk->GetOffsetY());
                break;
            }
            case 2:
            {
                m_renderer.Render2DPicture(m_shapeTitle->GetPicture(), m_shapeTitle->GetOffsetX(), m_shapeTitle->GetOffsetY());
                break;
            }
            case 3:
            {
                m_renderer.Render2DPicture(m_shapeCredits->GetPicture(), m_shapeCredits->GetOffsetX(), m_shapeCredits->GetOffsetY());
                break;
            }
            case 4:
            {
                m_renderer.Render2DPicture(m_shapeTrilogy->GetPicture(), m_shapeTrilogy->GetOffsetX(), m_shapeTrilogy->GetOffsetY());
                break;
            }
            default:
                break;
            }
        }
    }
}

void IntroViewAbyss::DrawRequestDifficultyLevel()
{
    if (m_gameId != GameId::CatacombAbyssv112)
    {
        m_renderer.Render2DPicture(m_shapeSelectDifficulty->GetPicture(), m_shapeSelectDifficulty->GetOffsetX(), m_shapeSelectDifficulty->GetOffsetY());
    }
}

void IntroViewAbyss::DrawNoviceSelected()
{
    if (m_gameId != GameId::CatacombAbyssv112)
    {
        m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
        m_renderer.Render2DPicture(m_shapeNovice->GetPicture(), 16, 192);
    }
}

void IntroViewAbyss::DrawWarriorSelected()
{
    if (m_gameId != GameId::CatacombAbyssv112)
    {
        m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
        m_renderer.Render2DPicture(m_shapeWarrior->GetPicture(), 16, 192);
    }
}

void IntroViewAbyss::DrawStandBeforeGate()
{
    if (m_gameId != GameId::CatacombAbyssv112)
    {
        m_renderer.Render2DPicture(m_shapeStandBeforeGate->GetPicture(), 0, 0);
    }
}
