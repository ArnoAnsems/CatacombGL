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

namespace fs = std::filesystem;

IntroViewApocalypse::IntroViewApocalypse(IRenderer& renderer, const fs::path& path) :
    IIntroView(renderer)
{
    m_shapeEntering = new Shape(renderer);
    const fs::path shp8 = path / "SHP8.APC";
    m_shapeEntering->LoadFromFile(shp8);

    m_shapePresents = new Shape(renderer);
    const fs::path shp14 = path / "SHP14.APC";
    m_shapePresents->LoadFromFile(shp14);

    m_shapeSoftdisk = new Shape(renderer);
    const fs::path shp1 = path / "SHP1.APC";
    m_shapeSoftdisk->LoadFromFile(shp1);

    m_shapeTitle = new Shape(renderer);
    const fs::path shp2 = path / "SHP2.APC";
    m_shapeTitle->LoadFromFile(shp2);

    m_shapeCreditsProgramming = new Shape(renderer);
    const fs::path shp3 = path / "SHP3.APC";
    m_shapeCreditsProgramming->LoadFromFile(shp3);

    m_shapeCreditsArt = new Shape(renderer);
    const fs::path shp4 = path / "SHP4.APC";
    m_shapeCreditsArt->LoadFromFile(shp4);

    m_shapeCreditsQA = new Shape(renderer);
    const fs::path shp5 = path / "SHP5.APC";
    m_shapeCreditsQA->LoadFromFile(shp5);

    m_shapeCreditsDesign = new Shape(renderer);
    const fs::path shp6 = path / "SHP6.APC";
    m_shapeCreditsDesign->LoadFromFile(shp6);

    m_shapeSelectDifficulty = new Shape(renderer);
    const fs::path shp10 = path / "SHP10.APC";
    m_shapeSelectDifficulty->LoadFromFile(shp10);

    m_shapeConfirmDifficulty = new Shape(renderer);
    const fs::path shp9 = path / "SHP9.APC";
    m_shapeConfirmDifficulty->LoadFromFile(shp9);

    m_shapeNovice = new Shape(renderer);
    const fs::path shp11 = path / "SHP11.APC";
    m_shapeNovice->LoadFromFile(shp11);

    m_shapeWarrior = new Shape(renderer);
    const fs::path shp12 = path / "SHP12.APC";
    m_shapeWarrior->LoadFromFile(shp12);

    m_shapeStandBeforeGate = new Shape(renderer);
    const fs::path shp7 = path / "SHP7.APC";
    m_shapeStandBeforeGate->LoadFromFile(shp7);
}

IntroViewApocalypse::~IntroViewApocalypse()
{
    delete m_shapeEntering;
    m_shapeEntering = nullptr;
    delete m_shapePresents;
    m_shapePresents = nullptr;
    delete m_shapeSoftdisk;
    m_shapeSoftdisk = nullptr;
    delete m_shapeTitle;
    m_shapeTitle = nullptr;
    delete m_shapeCreditsProgramming;
    m_shapeCreditsProgramming = nullptr;
    delete m_shapeCreditsArt;
    m_shapeCreditsArt = nullptr;
    delete m_shapeCreditsQA;
    m_shapeCreditsQA = nullptr;
    delete m_shapeCreditsDesign;
    m_shapeCreditsDesign = nullptr;
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

void IntroViewApocalypse::DrawRequestDifficultyLevel()
{
    m_renderer.Render2DPicture(m_shapeSelectDifficulty->GetPicture(), m_shapeSelectDifficulty->GetOffsetX(), m_shapeSelectDifficulty->GetOffsetY());
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
