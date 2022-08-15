// Copyright (C) 2022 Arno Ansems
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

#include "SavedGameConverterCatacomb3D.h"
#include "DecorateAll.h"

static const DosFormatConfig dosFormatConfigCatacomb3D =
{
    "", // gameName
    "", // saveVersion
    {
        HeaderItemSignature, HeaderItemOldTest, HeaderItemPresent, HeaderItemName,
        HeaderItemDifficulty, HeaderItemMapOn, HeaderItemBolts, HeaderItemNukes, HeaderItemPotions,
        HeaderItemKeys, HeaderItemScrolls, HeaderItemScore, HeaderItemBody, HeaderItemShotpower
    },   // headerItems
    {
        ObjectActive, ObjectTiccount, ObjectObclass, ObjectState16, ObjectShootable, ObjectTileObject,
        ObjectDistance, ObjectDir, ObjectX, ObjectY, ObjectTileX, ObjectTileY, ObjectViewX, ObjectViewHeight,
        ObjectAngle, ObjectHitpoints, ObjectSpeed, ObjectSize, ObjectXL, ObjectXH, ObjectYL, ObjectYH,
        ObjectTemp1, ObjectTemp2, ObjectNext, ObjectPrev
    }   // objectItems
};

// obclass definitions as seen in C3_DEF.H
const uint16_t obclassNothing = 0;
const uint16_t obclassPlayer = 1;
const uint16_t obclassBonus = 2;
const uint16_t obclassOrc = 3;
const uint16_t obclassBat = 4;
const uint16_t obclassSkeleton = 5;
const uint16_t obclassTroll = 6;
const uint16_t obclassDemon = 7;
const uint16_t obclassMage = 8;
const uint16_t obclassPlayerShot = 9;
const uint16_t obclassBigPlayerShot = 10;
const uint16_t obclassMageShot = 11;
const uint16_t obclassInert = 12;
const uint16_t obclassBounce = 13;
const uint16_t obclassGrelminar = 14;
const uint16_t obclassGate = 15;

// State pointers as conveniently documented in Reflection Keen (statetype_ptr_conversion.c).
const uint16_t s_pshot1 = 0x1776;
const uint16_t s_pshot2 = 0x1780;
const uint16_t s_shotexplode = 0x178A;
const uint16_t s_bigpshot1 = 0x1794;
const uint16_t s_bigpshot2 = 0x179E;
const uint16_t s_player = 0x17A8;
const uint16_t s_boltbonus = 0x17C4;
const uint16_t s_boltbonus2 = 0x17CE;
const uint16_t s_nukebonus = 0x17D8;
const uint16_t s_nukebonus2 = 0x17E2;
const uint16_t s_potionbonus = 0x17EC;
const uint16_t s_rkeybonus = 0x17F6;
const uint16_t s_ykeybonus = 0x1800;
const uint16_t s_gkeybonus = 0x180A;
const uint16_t s_bkeybonus = 0x1814;
const uint16_t s_scrollbonus = 0x181E;
const uint16_t s_chestbonus = 0x1828;
const uint16_t s_goalbonus = 0x1832;
const uint16_t s_walldie1 = 0x183C;
const uint16_t s_walldie2 = 0x1846;
const uint16_t s_walldie3 = 0x1850;
const uint16_t s_walldie4 = 0x185A;
const uint16_t s_walldie5 = 0x1864;
const uint16_t s_walldie6 = 0x186E;
const uint16_t s_gate1 = 0x1878;
const uint16_t s_gate2 = 0x1882;
const uint16_t s_gate3 = 0x188C;
const uint16_t s_gate4 = 0x1896;
const uint16_t s_fgate1 = 0x18A0;
const uint16_t s_fgate2 = 0x18AA;
const uint16_t s_fgate3 = 0x18B4;
const uint16_t s_fgate4 = 0x18BE;
const uint16_t s_trollpause = 0x18C8;
const uint16_t s_troll1 = 0x18D2;
const uint16_t s_troll2 = 0x18DC;
const uint16_t s_troll3 = 0x18E6;
const uint16_t s_troll4 = 0x18F0;
const uint16_t s_trollattack1 = 0x18FA;
const uint16_t s_trollattack2 = 0x1904;
const uint16_t s_trollattack3 = 0x190E;
const uint16_t s_trollouch = 0x1918;
const uint16_t s_trolldie1 = 0x1922;
const uint16_t s_trolldie2 = 0x192C;
const uint16_t s_trolldie3 = 0x1936;
const uint16_t s_orcpause = 0x1940;
const uint16_t s_orc1 = 0x194A;
const uint16_t s_orc2 = 0x1954;
const uint16_t s_orc3 = 0x195E;
const uint16_t s_orc4 = 0x1968;
const uint16_t s_orcattack1 = 0x1972;
const uint16_t s_orcattack2 = 0x197C;
const uint16_t s_orcattack3 = 0x1986;
const uint16_t s_orcouch = 0x1990;
const uint16_t s_orcdie1 = 0x199A;
const uint16_t s_orcdie2 = 0x19A4;
const uint16_t s_orcdie3 = 0x19AE;
const uint16_t s_demonpause = 0x19B8;
const uint16_t s_demon1 = 0x19C2;
const uint16_t s_demon2 = 0x19CC;
const uint16_t s_demon3 = 0x19D6;
const uint16_t s_demon4 = 0x19E0;
const uint16_t s_demonattack1 = 0x19EA;
const uint16_t s_demonattack2 = 0x19F4;
const uint16_t s_demonattack3 = 0x19FE;
const uint16_t s_demonouch = 0x1A08;
const uint16_t s_demondie1 = 0x1A12;
const uint16_t s_demondie2 = 0x1A1C;
const uint16_t s_demondie3 = 0x1A26;
const uint16_t s_mshot1 = 0x1A30;
const uint16_t s_mshot2 = 0x1A3A;
const uint16_t s_magepause = 0x1A44;
const uint16_t s_mage1 = 0x1A4E;
const uint16_t s_mage2 = 0x1A58;
const uint16_t s_mageattack1 = 0x1A62;
const uint16_t s_mageattack2 = 0x1A6C;
const uint16_t s_mageattack3 = 0x1A76;
const uint16_t s_mageouch = 0x1A80;
const uint16_t s_magedie1 = 0x1A8A;
const uint16_t s_magedie2 = 0x1A94;
const uint16_t s_grelpause = 0x1A9E;
const uint16_t s_grel1 = 0x1AA8;
const uint16_t s_grel2 = 0x1AB2;
const uint16_t s_grelattack1 = 0x1ABC;
const uint16_t s_grelattack2 = 0x1AC6;
const uint16_t s_grelattack3 = 0x1AD0;
const uint16_t s_grelouch = 0x1ADA;
const uint16_t s_greldie1 = 0x1AE4;
const uint16_t s_greldie2 = 0x1AEE;
const uint16_t s_greldie3 = 0x1AF8;
const uint16_t s_greldie4 = 0x1B02;
const uint16_t s_greldie5 = 0x1B0C;
const uint16_t s_greldie6 = 0x1B16;
const uint16_t s_bat1 = 0x1B20;
const uint16_t s_bat2 = 0x1B2A;
const uint16_t s_bat3 = 0x1B34;
const uint16_t s_bat4 = 0x1B3E;
const uint16_t s_batpast = 0x1B48;
const uint16_t s_batdie1 = 0x1B52;
const uint16_t s_batdie2 = 0x1B5C;
const uint16_t s_bounce1 = 0x1B66;
const uint16_t s_bounce2 = 0x1B70;

SavedGameConverterCatacomb3D::SavedGameConverterCatacomb3D()
{
}

SavedGameConverterCatacomb3D::~SavedGameConverterCatacomb3D()
{
}

const DosFormatConfig& SavedGameConverterCatacomb3D::GetDosFormatConfig() const
{
    return dosFormatConfigCatacomb3D;
}

const uint16_t SavedGameConverterCatacomb3D::GetActorIdOfBonus(const uint16_t state16, const int16_t temp1) const
{
    uint16_t actorId = 0;
    switch (state16)
    {
    case s_boltbonus:
    case s_boltbonus2:
        actorId = actorIdBonusBolt;
        break;
    case s_nukebonus:
    case s_nukebonus2:
        actorId = actorIdBonusNuke;
        break;
    case s_potionbonus:
        actorId = actorIdBonusPotion;
        break;
    case s_rkeybonus:
        actorId = actorIdBonusKeyRed;
        break;
    case s_ykeybonus:
        actorId = actorIdBonusKeyYellow;
        break;
    case s_gkeybonus:
        actorId = actorIdBonusKeyGreen;
        break;
    case s_bkeybonus:
        actorId = actorIdBonusKeyBlue;
        break;
    case s_scrollbonus:
        actorId =
            (temp1 == 7) ? actorIdBonusScroll1 :
            (temp1 == 8) ? actorIdBonusScroll2 :
            (temp1 == 9) ? actorIdBonusScroll3 :
            (temp1 == 10) ? actorIdBonusScroll4 :
            (temp1 == 11) ? actorIdBonusScroll5 :
            (temp1 == 12) ? actorIdBonusScroll6 :
            (temp1 == 13) ? actorIdBonusScroll7 :
            actorIdBonusScroll8;
        break;
    case s_chestbonus:
        actorId = actorIdBonusChest;
        break;
    case s_goalbonus:
        actorId = actorIdGrelminar;
        break;
    }
    return actorId;
}

const uint16_t SavedGameConverterCatacomb3D::GetActorIdOfGate(const uint16_t state16, const int16_t temp1) const
{
    uint16_t actorId = 0;
    if (temp1 == 0)
    {
        actorId = actorIdWarpToLevel;
    }
    else
    {
        switch (state16)
        {
        case s_gate1:
        case s_fgate1:
            actorId = actorIdWarpPortal1;
            break;
        case s_gate2:
        case s_fgate2:
            actorId = actorIdWarpPortal2;
            break;
        case s_gate3:
        case s_fgate3:
            actorId = actorIdWarpPortal3;
            break;
        case s_gate4:
        case s_fgate4:
            actorId = actorIdWarpPortal4;
            break;
        }
    }
    return actorId;
}

const uint16_t SavedGameConverterCatacomb3D::GetActorId(
    const uint16_t obclass,
    const uint16_t state16,
    const uint32_t state32,
    const int16_t temp1) const
{
    uint16_t actorId = 0;
    switch (obclass)
    {
    case obclassPlayer:
        actorId = actorIdPlayer;
        break;
    case obclassBonus:
        actorId = GetActorIdOfBonus(state16, temp1);
        break;
    case obclassOrc:
        actorId = actorIdMonsterOrc;
        break;
    case obclassBat:
        actorId = actorIdMonsterBat;
        break;
    case obclassSkeleton:
        // Not used in Catacomb 3D.
        break;
    case obclassTroll:
        actorId = actorIdMonsterTroll;
        break;
    case obclassDemon:
        actorId = actorIdMonsterDemon;
        break;
    case obclassMage:
        actorId = actorIdMonsterMage;
        break;
    case obclassPlayerShot:
        actorId = actorIdProjectilePlayerShot;
        break;
    case obclassBigPlayerShot:
        actorId = actorIdProjectilePlayerBigShot;
        break;
    case obclassMageShot:
        actorId = actorIdProjectileMageShot;
        break;
    case obclassInert:
        // TODO
        break;
    case obclassBounce:
        actorId = actorIdMonsterBounce;
        break;
    case obclassGrelminar:
        actorId = actorIdMonsterNemesis;
        break;
    case obclassGate:
        actorId = GetActorIdOfGate(state16, temp1);
        break;
    }
    return actorId;
}

const bool SavedGameConverterCatacomb3D::IsInertObject(const uint16_t obclass) const
{
    return obclass == obclassInert;
}