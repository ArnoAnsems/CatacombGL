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

// obclass definitions as seen in C3_DEF.H
constexpr uint16_t obclassNothing = 0;
constexpr uint16_t obclassPlayer = 1;
constexpr uint16_t obclassBonus = 2;
constexpr uint16_t obclassOrc = 3;
constexpr uint16_t obclassBat = 4;
constexpr uint16_t obclassSkeleton = 5;
constexpr uint16_t obclassTroll = 6;
constexpr uint16_t obclassDemon = 7;
constexpr uint16_t obclassMage = 8;
constexpr uint16_t obclassPlayerShot = 9;
constexpr uint16_t obclassBigPlayerShot = 10;
constexpr uint16_t obclassMageShot = 11;
constexpr uint16_t obclassInert = 12;
constexpr uint16_t obclassBounce = 13;
constexpr uint16_t obclassGrelminar = 14;
constexpr uint16_t obclassGate = 15;

// State pointers as conveniently documented in ReflectionHLE (statetype_ptr_conversion.c).
constexpr uint16_t s_pshot1[] = { 0x149A, 0x1776 };
constexpr uint16_t s_pshot2[] = { 0x14A4, 0x1780 };
constexpr uint16_t s_shotexplode[] = { 0x14AE, 0x178A };
constexpr uint16_t s_bigpshot1[] = { 0x14B8, 0x1794 };
constexpr uint16_t s_bigpshot2[] = { 0x14C2, 0x179E };
constexpr uint16_t s_player[] = { 0x14CC, 0x17A8 };
constexpr uint16_t s_boltbonus[] = { 0x17C4, 0x17C4 };
constexpr uint16_t s_boltbonus2[] = { 0x17CE, 0x17CE };
constexpr uint16_t s_nukebonus[] = { 0x17D8, 0x17D8 };
constexpr uint16_t s_nukebonus2[] = { 0x17E2, 0x17E2 };
constexpr uint16_t s_potionbonus[] = { 0x17EC, 0x17EC };
constexpr uint16_t s_rkeybonus[] = { 0x17F6, 0x17F6 };
constexpr uint16_t s_ykeybonus[] = { 0x1800, 0x1800 };
constexpr uint16_t s_gkeybonus[] = { 0x180A, 0x180A };
constexpr uint16_t s_bkeybonus[] = { 0x1814, 0x1814 };
constexpr uint16_t s_scrollbonus[] = { 0x181E, 0x181E };
constexpr uint16_t s_chestbonus[] = { 0x1828, 0x1828 };
constexpr uint16_t s_goalbonus[] = { 0x1832, 0x1832 };
constexpr uint16_t s_walldie1[] = { 0x183C, 0x183C };
constexpr uint16_t s_walldie2[] = { 0x1846, 0x1846 };
constexpr uint16_t s_walldie3[] = { 0x1850, 0x1850 };
constexpr uint16_t s_walldie4[] = { 0x185A, 0x185A };
constexpr uint16_t s_walldie5[] = { 0x1864, 0x1864 };
constexpr uint16_t s_walldie6[] = { 0x186E, 0x186E };
constexpr uint16_t s_gate1[] = { 0x159C, 0x1878 };
constexpr uint16_t s_gate2[] = { 0x15A6, 0x1882 };
constexpr uint16_t s_gate3[] = { 0x15B0, 0x188C };
constexpr uint16_t s_gate4[] = { 0x15BA, 0x1896 };
constexpr uint16_t s_fgate1[] = { 0x15C4, 0x18A0 };
constexpr uint16_t s_fgate2[] = { 0x15CE, 0x18AA };
constexpr uint16_t s_fgate3[] = { 0x15D8, 0x18B4 };
constexpr uint16_t s_fgate4[] = { 0x15E2, 0x18BE };
constexpr uint16_t s_trollpause[] = { 0x15EC, 0x18C8 };
constexpr uint16_t s_troll1[] = { 0x15F6, 0x18D2 };
constexpr uint16_t s_troll2[] = { 0x1600, 0x18DC };
constexpr uint16_t s_troll3[] = { 0x160A, 0x18E6 };
constexpr uint16_t s_troll4[] = { 0x1614, 0x18F0 };
constexpr uint16_t s_trollattack1[] = { 0x161E, 0x18FA };
constexpr uint16_t s_trollattack2[] = { 0x1628, 0x1904 };
constexpr uint16_t s_trollattack3[] = { 0x1632, 0x190E };
constexpr uint16_t s_trollouch[] = { 0x163C, 0x1918 };
constexpr uint16_t s_trolldie1[] = { 0x1646, 0x1922 };
constexpr uint16_t s_trolldie2[] = { 0x1650, 0x192C };
constexpr uint16_t s_trolldie3[] = { 0x1936, 0x1936 };
constexpr uint16_t s_orcpause[] = { 0x1664, 0x1940 };
constexpr uint16_t s_orc1[] = { 0x166E, 0x194A };
constexpr uint16_t s_orc2[] = { 0x1678, 0x1954 };
constexpr uint16_t s_orc3[] = { 0x1682, 0x195E };
constexpr uint16_t s_orc4[] = { 0x168C, 0x1968 };
constexpr uint16_t s_orcattack1[] = { 0x1696, 0x1972 };
constexpr uint16_t s_orcattack2[] = { 0x16A0, 0x197C };
constexpr uint16_t s_orcattack3[] = { 0x16AA, 0x1986 };
constexpr uint16_t s_orcouch[] = { 0x16B4, 0x1990 };
constexpr uint16_t s_orcdie1[] = { 0x16BE, 0x199A };
constexpr uint16_t s_orcdie2[] = { 0x16C8, 0x19A4 };
constexpr uint16_t s_orcdie3[] = { 0x19AE, 0x19AE };
constexpr uint16_t s_demonpause[] = { 0x16DC, 0x19B8 };
constexpr uint16_t s_demon1[] = { 0x16E6, 0x19C2 };
constexpr uint16_t s_demon2[] = { 0x16F0, 0x19CC };
constexpr uint16_t s_demon3[] = { 0x16FA, 0x19D6 };
constexpr uint16_t s_demon4[] = { 0x1704, 0x19E0 };
constexpr uint16_t s_demonattack1[] = { 0x170E, 0x19EA };
constexpr uint16_t s_demonattack2[] = { 0x1718, 0x19F4 };
constexpr uint16_t s_demonattack3[] = { 0x1722, 0x19FE };
constexpr uint16_t s_demonouch[] = { 0x172C, 0x1A08 };
constexpr uint16_t s_demondie1[] = { 0x1736, 0x1A12 };
constexpr uint16_t s_demondie2[] = { 0x1740, 0x1A1C };
constexpr uint16_t s_demondie3[] = { 0x1A26, 0x1A26 };
constexpr uint16_t s_mshot1[] = { 0x1754, 0x1A30 };
constexpr uint16_t s_mshot2[] = { 0x175E, 0x1A3A };
constexpr uint16_t s_magepause[] = { 0x1768, 0x1A44 };
constexpr uint16_t s_mage1[] = { 0x1772, 0x1A4E };
constexpr uint16_t s_mage2[] = { 0x177C, 0x1A58 };
constexpr uint16_t s_mageattack1[] = { 0x1786, 0x1A62 };
constexpr uint16_t s_mageattack2[] = { 0x1790, 0x1A6C };
constexpr uint16_t s_mageattack3[] = { 0x179A, 0x1A76 };
constexpr uint16_t s_mageouch[] = { 0x17A4, 0x1A80 };
constexpr uint16_t s_magedie1[] = { 0x17AE, 0x1A8A };
constexpr uint16_t s_magedie2[] = { 0x1A94, 0x1A94 };
constexpr uint16_t s_grelpause[] = { 0x17C2, 0x1A9E };
constexpr uint16_t s_grel1[] = { 0x17CC, 0x1AA8 };
constexpr uint16_t s_grel2[] = { 0x17D6, 0x1AB2 };
constexpr uint16_t s_grelattack1[] = { 0x17E0, 0x1ABC };
constexpr uint16_t s_grelattack2[] = { 0x17EA, 0x1AC6 };
constexpr uint16_t s_grelattack3[] = { 0x17F4, 0x1AD0 };
constexpr uint16_t s_grelouch[] = { 0x17FE, 0x1ADA };
constexpr uint16_t s_greldie1[] = { 0x1808, 0x1AE4 };
constexpr uint16_t s_greldie2[] = { 0x1812, 0x1AEE };
constexpr uint16_t s_greldie3[] = { 0x181C, 0x1AF8 };
constexpr uint16_t s_greldie4[] = { 0x1826, 0x1B02 };
constexpr uint16_t s_greldie5[] = { 0x1830, 0x1B0C };
constexpr uint16_t s_greldie6[] = { 0x1B16, 0x1B16 };
constexpr uint16_t s_bat1[] = { 0x1844, 0x1B20 };
constexpr uint16_t s_bat2[] = { 0x184E, 0x1B2A };
constexpr uint16_t s_bat3[] = { 0x1858, 0x1B34 };
constexpr uint16_t s_bat4[] = { 0x1862, 0x1B3E };
constexpr uint16_t s_batpast[] = { 0x186C, 0x1B48 };
constexpr uint16_t s_batdie1[] = { 0x1876, 0x1B52 };
constexpr uint16_t s_batdie2[] = { 0x1880, 0x1B5C };
constexpr uint16_t s_bounce1[] = { 0x188A, 0x1B66 };
constexpr uint16_t s_bounce2[] = { 0x1894, 0x1B70 };

SavedGameConverterCatacomb3D::SavedGameConverterCatacomb3D(const GameId gameId) :
    m_gameId(gameId)
{
}

SavedGameConverterCatacomb3D::~SavedGameConverterCatacomb3D()
{
}

const uint16_t SavedGameConverterCatacomb3D::GetActorIdOfBonus(const uint16_t state16, const int16_t temp1) const
{
    const uint8_t i = GetGameIndex();
    const uint16_t actorId =
        (state16 == s_boltbonus[i] || state16 == s_boltbonus2[i]) ? actorIdBonusBolt :
        (state16 == s_nukebonus[i] || state16 == s_nukebonus2[i]) ? actorIdBonusNuke :
        (state16 == s_potionbonus[i]) ? actorIdBonusPotion :
        (state16 == s_rkeybonus[i]) ? actorIdBonusKeyRed :
        (state16 == s_ykeybonus[i]) ? actorIdBonusKeyYellow :
        (state16 == s_gkeybonus[i]) ? actorIdBonusKeyGreen :
        (state16 == s_bkeybonus[i]) ? actorIdBonusKeyBlue :
        (state16 == s_scrollbonus[i] && temp1 == 7) ? actorIdBonusScroll1 :
        (state16 == s_scrollbonus[i] && temp1 == 8) ? actorIdBonusScroll2 :
        (state16 == s_scrollbonus[i] && temp1 == 9) ? actorIdBonusScroll3 :
        (state16 == s_scrollbonus[i] && temp1 == 10) ? actorIdBonusScroll4 :
        (state16 == s_scrollbonus[i] && temp1 == 11) ? actorIdBonusScroll5 :
        (state16 == s_scrollbonus[i] && temp1 == 12) ? actorIdBonusScroll6 :
        (state16 == s_scrollbonus[i] && temp1 == 13) ? actorIdBonusScroll7 :
        (state16 == s_scrollbonus[i] && temp1 == 14) ? actorIdBonusScroll8 :
        (state16 == s_chestbonus[i]) ? actorIdBonusChest :
        (state16 == s_goalbonus[i]) ? actorIdGrelminar :
        0;

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
        const uint8_t i = GetGameIndex();
        actorId =
            (state16 == s_gate1[i] || state16 == s_fgate1[i]) ? actorIdWarpPortal1 :
            (state16 == s_gate2[i] || state16 == s_fgate2[i]) ? actorIdWarpPortal2 :
            (state16 == s_gate3[i] || state16 == s_fgate3[i]) ? actorIdWarpPortal3 :
            (state16 == s_gate4[i] || state16 == s_fgate4[i]) ? actorIdWarpPortal4 :
            0;
    }
    return actorId;
}

const uint16_t SavedGameConverterCatacomb3D::GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    uint16_t actorId = 0;
    switch (dosObject.obclass)
    {
    case obclassPlayer:
        actorId = actorIdPlayer;
        break;
    case obclassBonus:
        actorId = GetActorIdOfBonus(dosObject.state16, dosObject.temp1);
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
        actorId = GetActorIdOfInert(dosObject.state16);
        break;
    case obclassBounce:
        actorId = actorIdMonsterBounce;
        break;
    case obclassGrelminar:
        actorId = actorIdMonsterNemesis;
        break;
    case obclassGate:
        actorId = GetActorIdOfGate(dosObject.state16, dosObject.temp1);
        break;
    }
    return actorId;
}

const bool SavedGameConverterCatacomb3D::IsInertObject(const uint16_t obclass) const
{
    return obclass == obclassInert;
}

const uint16_t SavedGameConverterCatacomb3D::GetActorIdOfInert(const uint16_t state16) const
{
    const uint8_t i = GetGameIndex();
    const uint16_t actorId =
        (state16 == s_walldie1[i] || state16 == s_walldie2[i] || state16 == s_walldie3[i] || state16 == s_walldie4[i] || state16 == s_walldie5[i] || state16 == s_walldie6[i]) ? actorIdExplodingWall :
        (state16 == s_trolldie1[i] || state16 == s_trolldie2[i] || state16 == s_trolldie3[i]) ? actorIdMonsterTroll :
        (state16 == s_orcdie1[i] || state16 == s_orcdie2[i] || state16 == s_orcdie3[i]) ? actorIdMonsterOrc :
        (state16 == s_demondie1[i] || state16 == s_demondie2[i] || state16 == s_demondie3[i]) ? actorIdMonsterDemon :
        (state16 == s_magedie1[i] || state16 == s_magedie2[i]) ? actorIdMonsterMage :
        (state16 == s_greldie1[i] || state16 == s_greldie2[i] || state16 == s_greldie3[i] || state16 == s_greldie4[i] || state16 == s_greldie5[i] || state16 == s_greldie6[i]) ? actorIdMonsterNemesis :
        (state16 == s_batdie1[i] || state16 == s_batdie2[i]) ? actorIdMonsterBat :
        0;
    return actorId;
}

const DecorateStateId SavedGameConverterCatacomb3D::GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    DecorateStateId stateId = StateIdWalk;
    const uint16_t state16 = dosObject.state16;
    const uint8_t i = GetGameIndex();

    if (
        state16 == s_orc1[i] ||
        state16 == s_orc2[i] ||
        state16 == s_orc3[i] ||
        state16 == s_orc4[i] ||
        state16 == s_troll1[i] ||
        state16 == s_troll2[i] ||
        state16 == s_troll3[i] ||
        state16 == s_troll4[i] ||
        state16 == s_demon1[i] ||
        state16 == s_demon2[i] ||
        state16 == s_demon3[i] ||
        state16 == s_demon4[i] ||
        state16 == s_grel1[i] ||
        state16 == s_grel2[i] ||
        state16 == s_bat1[i] ||
        state16 == s_bat2[i] ||
        state16 == s_bat3[i] ||
        state16 == s_bat4[i] ||
        state16 == s_mage1[i] ||
        state16 == s_mage2[i] ||
        state16 == s_player[i]
        )
    {
        stateId = StateIdWalk;
    }
    else if (
        state16 == s_trollattack1[i] ||
        state16 == s_trollattack2[i] ||
        state16 == s_trollattack3[i] ||
        state16 == s_orcattack1[i] ||
        state16 == s_orcattack2[i] ||
        state16 == s_orcattack3[i] ||
        state16 == s_demonattack1[i] ||
        state16 == s_demonattack2[i] ||
        state16 == s_demonattack3[i] ||
        state16 == s_batpast[i] ||
        state16 == s_mageattack1[i] ||
        state16 == s_mageattack2[i] ||
        state16 == s_mageattack3[i] ||
        state16 == s_grelattack1[i] ||
        state16 == s_grelattack2[i] ||
        state16 == s_grelattack3[i] ||
        state16 == s_trollpause[i] ||
        state16 == s_trollpause[i] ||
        state16 == s_demonpause[i] ||
        state16 == s_magepause[i] ||
        state16 == s_grelpause[i]
        )
    {
        stateId = StateIdAttack;
    }
    else if (
        state16 == s_trollouch[i] ||
        state16 == s_orcouch[i] ||
        state16 == s_demonouch[i] ||
        state16 == s_mageouch[i] ||
        state16 == s_grelouch[i]
        )
    {
        stateId = StateIdPain;
    }
    else if (
        state16 == s_orcdie1[i] ||
        state16 == s_orcdie2[i] ||
        state16 == s_trolldie1[i] ||
        state16 == s_trolldie2[i] ||
        state16 == s_demondie1[i] ||
        state16 == s_demondie2[i] ||
        state16 == s_magedie1[i] ||
        state16 == s_greldie1[i] ||
        state16 == s_greldie2[i] ||
        state16 == s_greldie3[i] ||
        state16 == s_greldie4[i] ||
        state16 == s_greldie5[i] ||
        state16 == s_batdie1[i] ||
        state16 == s_batdie2[i] ||
        state16 == s_shotexplode[i]
        )
    {
        stateId = StateIdDying;
    }
    else if (
        state16 == s_walldie1[i] ||
        state16 == s_walldie2[i] ||
        state16 == s_walldie3[i] ||
        state16 == s_walldie4[i] ||
        state16 == s_walldie5[i] ||
        state16 == s_walldie6[i] ||
        state16 == s_orcdie3[i] ||
        state16 == s_trolldie3[i] ||
        state16 == s_demondie3[i] ||
        state16 == s_magedie2[i] ||
        state16 == s_greldie6[i]
        )
    {
        stateId = StateIdDead;
    }
    else if (
        state16 == s_pshot1[i] ||
        state16 == s_pshot2[i] ||
        state16 == s_bigpshot1[i] ||
        state16 == s_bigpshot2[i] ||
        state16 == s_mshot1[i] ||
        state16 == s_mshot2[i]
        )
    {
        stateId = StateIdProjectileFly;
    }
    else if (
        state16 == s_boltbonus[i] ||
        state16 == s_boltbonus2[i] ||
        state16 == s_nukebonus[i] ||
        state16 == s_nukebonus2[i] ||
        state16 == s_potionbonus[i] ||
        state16 == s_rkeybonus[i] ||
        state16 == s_ykeybonus[i] ||
        state16 == s_gkeybonus[i] ||
        state16 == s_bkeybonus[i] ||
        state16 == s_scrollbonus[i] ||
        state16 == s_chestbonus[i] ||
        state16 == s_goalbonus[i] ||
        state16 == s_gate1[i] ||
        state16 == s_gate2[i] ||
        state16 == s_gate3[i] ||
        state16 == s_gate4[i] ||
        state16 == s_fgate1[i] ||
        state16 == s_fgate2[i] ||
        state16 == s_fgate3[i] ||
        state16 == s_fgate4[i]
        )
    {
        stateId = StateIdWaitForPickup;
    }

    return stateId;
}

const uint16_t SavedGameConverterCatacomb3D::GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    const uint16_t state16 = dosObject.state16;
    const uint8_t i = GetGameIndex();

    uint16_t animationFrame = 0;
    if (
        state16 == s_orc1[i] ||
        state16 == s_troll1[i] ||
        state16 == s_demon1[i] ||
        state16 == s_grel1[i] ||
        state16 == s_bat1[i] ||
        state16 == s_mage1[i] ||
        state16 == s_bounce1[i] ||
        state16 == s_trollattack1[i] ||
        state16 == s_orcattack1[i] ||
        state16 == s_orcattack2[i] ||
        state16 == s_demonattack1[i] ||
        state16 == s_batpast[i] ||
        state16 == s_mageattack1[i] ||
        state16 == s_grelattack1[i] ||
        state16 == s_trollouch[i] ||
        state16 == s_bounce1[i] ||
        state16 == s_orcouch[i] ||
        state16 == s_demonouch[i] ||
        state16 == s_mageouch[i] ||
        state16 == s_grelouch[i] ||
        state16 == s_player[i] ||
        state16 == s_orcdie1[i] ||
        state16 == s_trolldie1[i] ||
        state16 == s_demondie1[i] ||
        state16 == s_magedie1[i] ||
        state16 == s_greldie1[i] ||
        state16 == s_batdie1[i] ||
        state16 == s_magedie2[i] ||
        state16 == s_orcdie3[i] ||
        state16 == s_trolldie3[i] ||
        state16 == s_demondie3[i] ||
        state16 == s_greldie6[i] ||
        state16 == s_shotexplode[i] ||
        state16 == s_walldie1[i] ||
        state16 == s_walldie2[i] ||
        state16 == s_pshot1[i] ||
        state16 == s_bigpshot1[i] ||
        state16 == s_mshot1[i] ||
        state16 == s_boltbonus[i] ||
        state16 == s_nukebonus[i] ||
        state16 == s_potionbonus[i] ||
        state16 == s_rkeybonus[i] ||
        state16 == s_ykeybonus[i] ||
        state16 == s_gkeybonus[i] ||
        state16 == s_bkeybonus[i] ||
        state16 == s_scrollbonus[i] ||
        state16 == s_chestbonus[i] ||
        state16 == s_goalbonus[i] ||
        state16 == s_gate1[i] ||
        state16 == s_fgate1[i]
        )
    {
        animationFrame = 0u;
    }
    else if (
        state16 == s_orc2[i] ||
        state16 == s_troll2[i] ||
        state16 == s_demon2[i] ||
        state16 == s_grel2[i] ||
        state16 == s_bat2[i] ||
        state16 == s_mage2[i] ||
        state16 == s_bounce2[i] ||
        state16 == s_trollattack2[i] ||
        state16 == s_trollattack3[i] ||
        state16 == s_demonattack2[i] ||
        state16 == s_mageattack2[i] ||
        state16 == s_mageattack3[i] ||
        state16 == s_grelattack2[i] ||
        state16 == s_grelattack3[i] ||
        state16 == s_orcdie2[i] ||
        state16 == s_trolldie2[i] ||
        state16 == s_demondie2[i] ||
        state16 == s_greldie2[i] ||
        state16 == s_batdie2[i] ||
        state16 == s_walldie3[i] ||
        state16 == s_walldie4[i] ||
        state16 == s_pshot2[i] ||
        state16 == s_bigpshot2[i] ||
        state16 == s_mshot2[i] ||
        state16 == s_boltbonus2[i] ||
        state16 == s_nukebonus2[i] ||
        state16 == s_gate2[i] ||
        state16 == s_fgate2[i]
        )
    {
        animationFrame = 1u;
    }
    else if (
        state16 == s_orc3[i] ||
        state16 == s_orcpause[i] ||
        state16 == s_troll3[i] ||
        state16 == s_demon3[i] ||
        state16 == s_bat3[i] ||
        state16 == s_trollpause[i] ||
        state16 == s_orcattack3[i] ||
        state16 == s_demonattack3[i] ||
        state16 == s_magepause[i] ||
        state16 == s_grelpause[i] ||
        state16 == s_greldie3[i] ||
        state16 == s_walldie5[i] ||
        state16 == s_walldie6[i] ||
        state16 == s_gate3[i] ||
        state16 == s_fgate3[i]
        )
    {
        animationFrame = 2u;
    }
    else if (
        state16 == s_orc4[i] ||
        state16 == s_troll4[i] ||
        state16 == s_demon4[i] ||
        state16 == s_bat4[i] ||
        state16 == s_demonpause[i] ||
        state16 == s_greldie4[i] ||
        state16 == s_gate4[i] ||
        state16 == s_fgate4[i]
        )
    {
            animationFrame = 3u;
    }
    else if (
        state16 == s_greldie5[i]
        )
    {
        animationFrame = 4u;
    }
    return animationFrame;
}

const uint8_t SavedGameConverterCatacomb3D::GetGameIndex() const
{
    return (m_gameId == GameId::Catacomb3Dv100) ? 0 : 1;
}

void SavedGameConverterCatacomb3D::SetFarPointerOffset(const uint32_t /*playerState32*/)
{
    // No far pointer offset used.
}