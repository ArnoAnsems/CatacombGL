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

#include "IntroViewArmageddon.h"

IntroViewArmageddon::IntroViewArmageddon(IRenderer& renderer, const std::string& path, Logging* logging) :
    IIntroView(renderer, logging)
{
    m_shapeEntering = new Shape(renderer, m_logging);
    const std::string shp8 = path + "SHP8.ARM";
    m_shapeEntering->LoadFromFile(shp8.c_str());

    m_shapePresents = new Shape(renderer, m_logging);
    const std::string shp14 = path + "SHP14.ARM";
    m_shapePresents->LoadFromFile(shp14.c_str());

    m_shapeSoftdisk = new Shape(renderer, m_logging);
    const std::string shp1 = path + "SHP1.ARM";
    m_shapeSoftdisk->LoadFromFile(shp1.c_str());

    m_shapeTitle = new Shape(renderer, m_logging);
    const std::string shp2 = path + "SHP2.ARM";
    m_shapeTitle->LoadFromFile(shp2.c_str());

    m_shapeCreditsProgramming = new Shape(renderer, m_logging);
    const std::string shp3 = path + "SHP3.ARM";
    m_shapeCreditsProgramming->LoadFromFile(shp3.c_str());

    m_shapeCreditsArt = new Shape(renderer, m_logging);
    const std::string shp4 = path + "SHP4.ARM";
    m_shapeCreditsArt->LoadFromFile(shp4.c_str());

    m_shapeCreditsQA = new Shape(renderer, m_logging);
    const std::string shp5 = path + "SHP5.ARM";
    m_shapeCreditsQA->LoadFromFile(shp5.c_str());

    m_shapeCreditsDesign = new Shape(renderer, m_logging);
    const std::string shp6 = path + "SHP6.ARM";
    m_shapeCreditsDesign->LoadFromFile(shp6.c_str());

    m_shapeSelectDifficulty = new Shape(renderer, m_logging);
    const std::string shp10 = path + "SHP10.ARM";
    m_shapeSelectDifficulty->LoadFromFile(shp10.c_str());

    m_shapeConfirmDifficulty = new Shape(renderer, m_logging);
    const std::string shp9 = path + "SHP9.ARM";
    m_shapeConfirmDifficulty->LoadFromFile(shp9.c_str());

    m_shapeNovice = new Shape(renderer, m_logging);
    const std::string shp11 = path + "SHP11.ARM";
    m_shapeNovice->LoadFromFile(shp11.c_str());

    m_shapeWarrior = new Shape(renderer, m_logging);
    const std::string shp12 = path + "SHP12.ARM";
    m_shapeWarrior->LoadFromFile(shp12.c_str());

    m_shapeStandBeforeGate = new Shape(renderer, m_logging);
    const std::string shp7 = path + "SHP7.ARM";
    m_shapeStandBeforeGate->LoadFromFile(shp7.c_str());
}

IntroViewArmageddon::~IntroViewArmageddon()
{
    if (m_shapeEntering != NULL)
    {
        delete m_shapeEntering;
        m_shapeEntering = NULL;
    }

    if (m_shapePresents != NULL)
    {
        delete m_shapePresents;
        m_shapePresents = NULL;
    }

    if (m_shapeSoftdisk != NULL)
    {
        delete m_shapeSoftdisk;
        m_shapeSoftdisk = NULL;
    }

    if (m_shapeTitle != NULL)
    {
        delete m_shapeTitle;
        m_shapeTitle = NULL;
    }

    if (m_shapeCreditsProgramming != NULL)
    {
        delete m_shapeCreditsProgramming;
        m_shapeCreditsProgramming = NULL;
    }

    if (m_shapeCreditsArt != NULL)
    {
        delete m_shapeCreditsArt;
        m_shapeCreditsArt = NULL;
    }

    if (m_shapeCreditsQA != NULL)
    {
        delete m_shapeCreditsQA;
        m_shapeCreditsQA = NULL;
    }

    if (m_shapeCreditsDesign != NULL)
    {
        delete m_shapeCreditsDesign;
        m_shapeCreditsDesign = NULL;
    }

    if (m_shapeSelectDifficulty != NULL)
    {
        delete m_shapeSelectDifficulty;
        m_shapeSelectDifficulty = NULL;
    }

    if (m_shapeConfirmDifficulty != NULL)
    {
        delete m_shapeConfirmDifficulty;
        m_shapeConfirmDifficulty = NULL;
    }

    if (m_shapeNovice != NULL)
    {
        delete m_shapeNovice;
        m_shapeNovice = NULL;
    }

    if (m_shapeWarrior != NULL)
    {
        delete m_shapeWarrior;
        m_shapeWarrior = NULL;
    }

    if (m_shapeStandBeforeGate != NULL)
    {
        delete m_shapeStandBeforeGate;
        m_shapeStandBeforeGate = NULL;
    }
}

void IntroViewArmageddon::DrawIntroduction(const uint32_t timeStamp)
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

void IntroViewArmageddon::DrawRequestDifficultyLevel()
{
    m_renderer.Render2DPicture(m_shapeSelectDifficulty->GetPicture(), m_shapeSelectDifficulty->GetOffsetX(), m_shapeSelectDifficulty->GetOffsetY());
}

void IntroViewArmageddon::DrawNoviceSelected()
{
    m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
    m_renderer.Render2DPicture(m_shapeNovice->GetPicture(), 16, 192);
}

void IntroViewArmageddon::DrawWarriorSelected()
{
    m_renderer.Render2DPicture(m_shapeConfirmDifficulty->GetPicture(), 0, 0);
    m_renderer.Render2DPicture(m_shapeWarrior->GetPicture(), 16, 192);
}

void IntroViewArmageddon::DrawStandBeforeGate()
{
    m_renderer.Render2DPicture(m_shapeStandBeforeGate->GetPicture(), 0, 0);
}