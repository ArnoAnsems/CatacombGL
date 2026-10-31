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

#include "IntroViewApocalypse.h"
#include "../Engine/IRenderer.h"
#include "../Engine/Shape.h"

namespace fs = std::filesystem;

IntroViewApocalypse::IntroViewApocalypse(IRenderer& renderer, EgaGraph& egaGraph, const fs::path& path) :
    IIntroView(renderer),
    m_gameControllerHint(renderer, egaGraph)
{
    m_shapeEntering = std::make_unique<Shape>(renderer);
    const fs::path shp8 = path / "SHP8.APC";
    m_shapeEntering->LoadFromFile(shp8);

    m_shapePresents = std::make_unique<Shape>(renderer);
    const fs::path shp14 = path / "SHP14.APC";
    m_shapePresents->LoadFromFile(shp14);

    m_shapeSoftdisk = std::make_unique<Shape>(renderer);
    const fs::path shp1 = path / "SHP1.APC";
    m_shapeSoftdisk->LoadFromFile(shp1);

    m_shapeTitle = std::make_unique<Shape>(renderer);
    const fs::path shp2 = path / "SHP2.APC";
    m_shapeTitle->LoadFromFile(shp2);

    m_shapeCreditsProgramming = std::make_unique<Shape>(renderer);
    const fs::path shp3 = path / "SHP3.APC";
    m_shapeCreditsProgramming->LoadFromFile(shp3);

    m_shapeCreditsArt = std::make_unique<Shape>(renderer);
    const fs::path shp4 = path / "SHP4.APC";
    m_shapeCreditsArt->LoadFromFile(shp4);

    m_shapeCreditsQA = std::make_unique<Shape>(renderer);
    const fs::path shp5 = path / "SHP5.APC";
    m_shapeCreditsQA->LoadFromFile(shp5);

    m_shapeCreditsDesign = std::make_unique<Shape>(renderer);
    const fs::path shp6 = path / "SHP6.APC";
    m_shapeCreditsDesign->LoadFromFile(shp6);

    m_shapeSelectDifficulty = std::make_unique<Shape>(renderer);
    const fs::path shp10 = path / "SHP10.APC";
    m_shapeSelectDifficulty->LoadFromFile(shp10);

    m_shapeConfirmDifficulty = std::make_unique<Shape>(renderer);
    const fs::path shp9 = path / "SHP9.APC";
    m_shapeConfirmDifficulty->LoadFromFile(shp9);

    m_shapeNovice = std::make_unique<Shape>(renderer);
    const fs::path shp11 = path / "SHP11.APC";
    m_shapeNovice->LoadFromFile(shp11);

    m_shapeWarrior = std::make_unique<Shape>(renderer);
    const fs::path shp12 = path / "SHP12.APC";
    m_shapeWarrior->LoadFromFile(shp12);

    m_shapeStandBeforeGate = std::make_unique<Shape>(renderer);
    const fs::path shp7 = path / "SHP7.APC";
    m_shapeStandBeforeGate->LoadFromFile(shp7);

    m_shapeBackForMore = std::make_unique<Shape>(renderer);
    const fs::path shp13 = path / "SHP13.APC";
    m_shapeBackForMore->LoadFromFile(shp13);
}

IntroViewApocalypse::~IntroViewApocalypse()
{
}

void IntroViewApocalypse::DrawIntroduction(const uint32_t timeStamp)
{
    if (timeStamp < 5000)
    {
        m_renderer.Render2DPicture(m_shapeEntering->GetPicture(), 20, 72);
    }
    else
    {
        const uint8_t pictureIndex = ((timeStamp - 5000) / 7000) % 7;
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
            m_renderer.Render2DPicture(m_shapeCreditsProgramming->GetPicture(), m_shapeCreditsProgramming->GetOffsetX(), m_shapeCreditsProgramming->GetOffsetY());
            break;
        }
        case 4:
        {
            m_renderer.Render2DPicture(m_shapeCreditsArt->GetPicture(), m_shapeCreditsArt->GetOffsetX(), m_shapeCreditsArt->GetOffsetY());
            break;
        }
        case 5:
        {
            m_renderer.Render2DPicture(m_shapeCreditsQA->GetPicture(), m_shapeCreditsQA->GetOffsetX(), m_shapeCreditsQA->GetOffsetY());
            break;
        }
        case 6:
        {
            m_renderer.Render2DPicture(m_shapeCreditsDesign->GetPicture(), m_shapeCreditsDesign->GetOffsetX(), m_shapeCreditsDesign->GetOffsetY());
            break;
        }
        default:
            break;
        }

    }
}

void IntroViewApocalypse::DrawRequestDifficultyLevel(bool showGameControllerHint)
{
    m_renderer.Render2DPicture(m_shapeSelectDifficulty->GetPicture(), m_shapeSelectDifficulty->GetOffsetX(), m_shapeSelectDifficulty->GetOffsetY());

    if (showGameControllerHint)
    {
        m_gameControllerHint.DrawDifficultySelection();
    }
}

void IntroViewApocalypse::DrawNoviceSelected()
{
    m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
    m_renderer.Render2DPicture(m_shapeNovice->GetPicture(), 16, 192);
}

void IntroViewApocalypse::DrawWarriorSelected()
{
    m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
    m_renderer.Render2DPicture(m_shapeWarrior->GetPicture(), 16, 192);
}

void IntroViewApocalypse::DrawStandBeforeGate()
{
    m_renderer.Render2DPicture(m_shapeStandBeforeGate->GetPicture(), 0, 0);
}

void IntroViewApocalypse::DrawBackForMore()
{
    m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
    m_renderer.Render2DPicture(m_shapeBackForMore->GetPicture(), 64, 192);
}

bool IntroViewApocalypse::CanShowBackForMore()
{
    return true;
}