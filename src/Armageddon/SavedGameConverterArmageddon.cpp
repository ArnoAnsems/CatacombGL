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

#include "SavedGameConverterArmageddon.h"
#include "DecorateAll.h"

// classtypes definitions as seen in DEF.H
constexpr uint16_t nothing = 0;
constexpr uint16_t playerobj = 1;
constexpr uint16_t bonusobj = 2;
constexpr uint16_t succubusobj = 3;
constexpr uint16_t batobj = 4;
constexpr uint16_t skeletonobj = 5;
constexpr uint16_t fatdemonobj = 6;
constexpr uint16_t godessobj = 7;
constexpr uint16_t mageobj = 8;
constexpr uint16_t pshotobj = 9;
constexpr uint16_t bigpshotobj = 10;
constexpr uint16_t mshotobj = 11;
constexpr uint16_t inertobj = 12;
constexpr uint16_t bounceobj = 13;
constexpr uint16_t grelmobj = 14;
constexpr uint16_t sshotobj = 15;
constexpr uint16_t gateobj = 16;
constexpr uint16_t zombieobj = 17;
constexpr uint16_t antobj = 18;
constexpr uint16_t wetobj = 19;
constexpr uint16_t expobj = 20;
constexpr uint16_t eyeobj = 21;
constexpr uint16_t wallskelobj = 22;
constexpr uint16_t eshotobj = 23;
constexpr uint16_t treeobj = 24;
constexpr uint16_t gshotobj = 25;
constexpr uint16_t reddemonobj = 26;
constexpr uint16_t freezeobj = 27;
constexpr uint16_t solidobj = 28;
constexpr uint16_t cloudobj = 29;
constexpr uint16_t dshotobj = 30;
constexpr uint16_t hbunnyobj = 31;
constexpr uint16_t bunnyobj = 32;
constexpr uint16_t realsolidobj = 33;

// State pointers as conveniently documented in ReflectionHLE (statetype_ptr_conversion.c).
constexpr uint32_t s_pshot1 = 0x1A530000;
constexpr uint32_t s_pshot2 = 0x1A540000;
constexpr uint32_t s_pshot_exp1 = 0x1A550000;
constexpr uint32_t s_pshot_exp2 = 0x1A560000;
constexpr uint32_t s_pshot_exp3 = 0x1A570000;
constexpr uint32_t s_player = 0x1A580000;
constexpr uint32_t s_explode = 0x1A590000;
constexpr uint32_t s_boltbonus = 0x1A5A0000;
constexpr uint32_t s_boltbonus2 = 0x1A5B0000;
constexpr uint32_t s_boltbonus3 = 0x1A5C0000;
constexpr uint32_t s_nukebonus = 0x1A5D0000;
constexpr uint32_t s_nukebonus2 = 0x1A5E0000;
constexpr uint32_t s_nukebonus3 = 0x1A5F0000;
constexpr uint32_t s_potionbonus = 0x1A600000;
constexpr uint32_t s_rkeybonus = 0x1A610000;
constexpr uint32_t s_ykeybonus = 0x1A620000;
constexpr uint32_t s_gkeybonus = 0x1A630000;
constexpr uint32_t s_bkeybonus = 0x1A640000;
constexpr uint32_t s_chestbonus = 0x1A650000;
constexpr uint32_t s_waterchestbonus1 = 0x1A660000;
constexpr uint32_t s_waterchestbonus2 = 0x1A670000;
constexpr uint32_t s_rgem1bonus = 0x1A680000;
constexpr uint32_t s_ygem1bonus = 0x1A690000;
constexpr uint32_t s_ggem1bonus = 0x1A6A0000;
constexpr uint32_t s_bgem1bonus = 0x1A6B0000;
constexpr uint32_t s_pgem1bonus = 0x1A6C0000;
constexpr uint32_t s_rgem2bonus = 0x1A6D0000;
constexpr uint32_t s_ygem2bonus = 0x1A6E0000;
constexpr uint32_t s_ggem2bonus = 0x1A6F0000;
constexpr uint32_t s_bgem2bonus = 0x1A700000;
constexpr uint32_t s_pgem2bonus = 0x1A710000;
constexpr uint32_t s_bonus_die =  0x1A720000;
constexpr uint32_t s_tombs0 = 0x1A730000;
constexpr uint32_t s_tombs1 = 0x1A740000;
constexpr uint32_t s_tombs2 = 0x1A750000;
constexpr uint32_t s_ftimebonus = 0x1A760000;
constexpr uint32_t s_ftimebonus2 = 0x1A770000;
constexpr uint32_t s_walldie1 = 0x1A780000;
constexpr uint32_t s_walldie2 = 0x1A790000;
constexpr uint32_t s_walldie3 = 0x1A7A0000;
constexpr uint32_t s_walldie4 = 0x1A7B0000;
constexpr uint32_t s_walldie5 = 0x1A7C0000;
constexpr uint32_t s_walldie6 = 0x1A7D0000;
constexpr uint32_t s_obj_gate1 = 0x1A7E0000;
constexpr uint32_t s_obj_gate2 = 0x1A7F0000;
constexpr uint32_t s_obj_gate3 = 0x1A800000;
constexpr uint32_t s_obj_gate4 = 0x1A810000;
constexpr uint32_t s_anthill = 0x1A820000;
constexpr uint32_t s_fatdemon_pause = 0x1A830000;
constexpr uint32_t s_fatdemon_walk1 = 0x1A840000;
constexpr uint32_t s_fatdemon_walk2 = 0x1A850000;
constexpr uint32_t s_fatdemon_walk3 = 0x1A860000;
constexpr uint32_t s_fatdemon_walk4 = 0x1A870000;
constexpr uint32_t s_fatdemon_attack1 = 0x1A880000;
constexpr uint32_t s_fatdemon_attack2 = 0x1A890000;
constexpr uint32_t s_fatdemon_ouch = 0x1A8A0000;
constexpr uint32_t s_fatdemon_blowup1 = 0x1A8B0000;
constexpr uint32_t s_fatdemon_blowup2 = 0x1A8C0000;
constexpr uint32_t s_fatdemon_blowup3 = 0x1A8D0000;
constexpr uint32_t s_fatdemon_blowup4 = 0x1A8E0000;
constexpr uint32_t s_fatdemon_blowup5 = 0x1A8F0000;
constexpr uint32_t s_fatdemon_blowup6 = 0x1A900000;
constexpr uint32_t s_fatdemon_blowup7 = 0x1A910000;
constexpr uint32_t s_fatdemon_explode = 0x1A920000;
constexpr uint32_t s_fatdemon_feet = 0x1A930000;
constexpr uint32_t s_wet_bubbles1 = 0x1A940000;
constexpr uint32_t s_wet_bubbles2 = 0x1A950000;
constexpr uint32_t s_wet_bubbles3 = 0x1A960000;
constexpr uint32_t s_wet_peek = 0x1A970000;
constexpr uint32_t s_wet_rise1 = 0x1A980000;
constexpr uint32_t s_wet_rise3 = 0x1A990000;
constexpr uint32_t s_wet_rise4 = 0x1A9A0000;
constexpr uint32_t s_wet_rise5 = 0x1A9B0000;
constexpr uint32_t s_wet_sink1 = 0x1A9C0000;
constexpr uint32_t s_wet_sink2 = 0x1A9D0000;
constexpr uint32_t s_wet_sink3 = 0x1A9E0000;
constexpr uint32_t s_wet_walk1 = 0x1A9F0000;
constexpr uint32_t s_wet_walk2 = 0x1AA00000;
constexpr uint32_t s_wet_walk3 = 0x1AA10000;
constexpr uint32_t s_wet_walk4 = 0x1AA20000;
constexpr uint32_t s_wet_attack1 = 0x1AA30000;
constexpr uint32_t s_wet_attack2 = 0x1AA40000;
constexpr uint32_t s_wet_attack3 = 0x1AA50000;
constexpr uint32_t s_wet_attack4 = 0x1AA60000;
constexpr uint32_t s_wet_ouch = 0x1AA70000;
constexpr uint32_t s_wet_die1 = 0x1AA80000;
constexpr uint32_t s_wet_die2 = 0x1AA90000;
constexpr uint32_t s_wet_die3 = 0x1AAA0000;
constexpr uint32_t s_wet_die4 = 0x1AAB0000;
constexpr uint32_t s_wet_die5 = 0x1AAC0000;
constexpr uint32_t s_dragon_shot1 = 0x1AAD0000;
constexpr uint32_t s_dragon_shot2 = 0x1AAE0000;
constexpr uint32_t s_wallskel = 0x1AAF0000;
constexpr uint32_t s_wallskel2 = 0x1AB00000;
constexpr uint32_t s_skel_pause = 0x1AB10000;
constexpr uint32_t s_skel_1 = 0x1AB20000;
constexpr uint32_t s_skel_2 = 0x1AB30000;
constexpr uint32_t s_skel_3 = 0x1AB40000;
constexpr uint32_t s_skel_4 = 0x1AB50000;
constexpr uint32_t s_skel_attack1 = 0x1AB60000;
constexpr uint32_t s_skel_attack2 = 0x1AB70000;
constexpr uint32_t s_skel_attack3 = 0x1AB80000;
constexpr uint32_t s_skel_ouch = 0x1AB90000;
constexpr uint32_t s_skel_die1 = 0x1ABA0000;
constexpr uint32_t s_skel_die2 = 0x1ABB0000;
constexpr uint32_t s_skel_die3 = 0x1ABC0000;
constexpr uint32_t s_eye_pause = 0x1ABD0000;
constexpr uint32_t s_eye_1 = 0x1ABE0000;
constexpr uint32_t s_eye_2 = 0x1ABF0000;
constexpr uint32_t s_eye_3 = 0x1AC00000;
constexpr uint32_t s_eye_4 = 0x1AC10000;
constexpr uint32_t s_eye_shootplayer_1 = 0x1AC20000;
constexpr uint32_t s_eye_shootplayer_2 = 0x1AC30000;
constexpr uint32_t s_eye_ouch = 0x1AC40000;
constexpr uint32_t s_eye_ouch2 = 0x1AC50000;
constexpr uint32_t s_eye_die1 = 0x1AC60000;
constexpr uint32_t s_eye_die2 = 0x1AC70000;
constexpr uint32_t s_eye_die3 = 0x1AC80000;
constexpr uint32_t s_eshot1 = 0x1AC90000;
constexpr uint32_t s_eshot2 = 0x1ACA0000;
constexpr uint32_t s_succubus_pause = 0x1ACB0000;
constexpr uint32_t s_succubus_walk1 = 0x1ACC0000;
constexpr uint32_t s_succubus_walk2 = 0x1ACD0000;
constexpr uint32_t s_succubus_walk3 = 0x1ACE0000;
constexpr uint32_t s_succubus_walk4 = 0x1ACF0000;
constexpr uint32_t s_succubus_attack1 = 0x1AD00000;
constexpr uint32_t s_succubus_attack2 = 0x1AD10000;
constexpr uint32_t s_succubus_attack3 = 0x1AD20000;
constexpr uint32_t s_succubus_ouch = 0x1AD30000;
constexpr uint32_t s_succubus_death1 = 0x1AD40000;
constexpr uint32_t s_succubus_death2 = 0x1AD50000;
constexpr uint32_t s_succubus_shot1 = 0x1AD60000;
constexpr uint32_t s_magepause = 0x1AD70000;
constexpr uint32_t s_mage1 = 0x1AD80000;
constexpr uint32_t s_mage2 = 0x1AD90000;
constexpr uint32_t s_mageattack3 = 0x1ADA0000;
constexpr uint32_t s_mageouch = 0x1ADB0000;
constexpr uint32_t s_magedie1 = 0x1ADC0000;
constexpr uint32_t s_magedie2 = 0x1ADD0000;
constexpr uint32_t s_mshot1 = 0x1ADE0000;
constexpr uint32_t s_mshot2 = 0x1ADF0000;
constexpr uint32_t s_bunny_left1 = 0x1AE00000;
constexpr uint32_t s_bunny_left2 = 0x1AE10000;
constexpr uint32_t s_bunny_left3 = 0x1AE20000;
constexpr uint32_t s_bunny_right1 = 0x1AE30000;
constexpr uint32_t s_bunny_right2 = 0x1AE40000;
constexpr uint32_t s_bunny_right3 = 0x1AE50000;
constexpr uint32_t s_bunny_meta1 = 0x1AE60000;
constexpr uint32_t s_bunny_meta2 = 0x1AE70000;
constexpr uint32_t s_bunny_walk1 = 0x1AE80000;
constexpr uint32_t s_bunny_walk2 = 0x1AE90000;
constexpr uint32_t s_bunny_attack1 = 0x1AEA0000;
constexpr uint32_t s_bunny_attack2 = 0x1AEB0000;
constexpr uint32_t s_bunny_ouch = 0x1AEC0000;
constexpr uint32_t s_bunny_pause = 0x1AED0000;
constexpr uint32_t s_bunny_death1 = 0x1AEE0000;
constexpr uint32_t s_bunny_death2 = 0x1AEF0000;
constexpr uint32_t s_bunny_death3 = 0x1AF00000;
constexpr uint32_t s_red_demonpause = 0x1AF10000;
constexpr uint32_t s_red_demon1 = 0x1AF20000;
constexpr uint32_t s_red_demon2 = 0x1AF30000;
constexpr uint32_t s_red_demon3 = 0x1AF40000;
constexpr uint32_t s_red_demon4 = 0x1AF50000;
constexpr uint32_t s_red_demonattack1 = 0x1AF60000;
constexpr uint32_t s_red_demonattack2 = 0x1AF70000;
constexpr uint32_t s_red_demonattack3 = 0x1AF80000;
constexpr uint32_t s_red_demonouch = 0x1AF90000;
constexpr uint32_t s_red_demondie1 = 0x1AFA0000;
constexpr uint32_t s_red_demondie2 = 0x1AFB0000;
constexpr uint32_t s_red_demondie3 = 0x1AFC0000;
constexpr uint32_t s_red_demondie4 = 0x1AFD0000;
constexpr uint32_t s_grelpause = 0x1AFE0000;
constexpr uint32_t s_grel1 = 0x1AFF0000;
constexpr uint32_t s_grel2 = 0x1B000000;
constexpr uint32_t s_grelattack3 = 0x1B010000;
constexpr uint32_t s_grelouch = 0x1B020000;
constexpr uint32_t s_greldie1 = 0x1B030000;
constexpr uint32_t s_greldie2 = 0x1B040000;
constexpr uint32_t s_greldie3 = 0x1B050000;
constexpr uint32_t s_greldie4 = 0x1B060000;
constexpr uint32_t s_greldie5 = 0x1B070000;
constexpr uint32_t s_greldie5a = 0x1B080000;
constexpr uint32_t s_greldie6 = 0x1B090000;
constexpr uint32_t s_gshot1 = 0x1B0A0000;
constexpr uint32_t s_bat1 = 0x1B0B0000;
constexpr uint32_t s_bat2 = 0x1B0C0000;
constexpr uint32_t s_bat3 = 0x1B0D0000;
constexpr uint32_t s_bat4 = 0x1B0E0000;
constexpr uint32_t s_batpast = 0x1B0F0000;
constexpr uint32_t s_batdie1 = 0x1B100000;
constexpr uint32_t s_batdie2 = 0x1B110000;
constexpr uint32_t s_godesspause = 0x1B120000;
constexpr uint32_t s_godess_statue1 = 0x1B130000;
constexpr uint32_t s_godess_statue2 = 0x1B140000;
constexpr uint32_t s_godess1 = 0x1B150000;
constexpr uint32_t s_godess2 = 0x1B160000;
constexpr uint32_t s_godess3 = 0x1B170000;
constexpr uint32_t s_godessattack1 = 0x1B180000;
constexpr uint32_t s_godessattack2 = 0x1B190000;
constexpr uint32_t s_godessattack3 = 0x1B1A0000;
constexpr uint32_t s_godessouch = 0x1B1B0000;
constexpr uint32_t s_godessdie1 = 0x1B1C0000;
constexpr uint32_t s_godessdie2 = 0x1B1D0000;
constexpr uint32_t s_ant_wait = 0x1B1E0000;
constexpr uint32_t s_ant_egg = 0x1B1F0000;
constexpr uint32_t s_ant_walk1 = 0x1B200000;
constexpr uint32_t s_ant_walk2 = 0x1B210000;
constexpr uint32_t s_ant_walk3 = 0x1B220000;
constexpr uint32_t s_ant_attack1 = 0x1B230000;
constexpr uint32_t s_ant_pause = 0x1B240000;
constexpr uint32_t s_ant_ouch = 0x1B250000;
constexpr uint32_t s_ant_die1 = 0x1B260000;
constexpr uint32_t s_ant_die2 = 0x1B270000;
constexpr uint32_t s_ant_die3 = 0x1B280000;
constexpr uint32_t s_zombie_risen = 0x1B290000;
constexpr uint32_t s_zombie_pause = 0x1B2A0000;
constexpr uint32_t s_zombie_inground = 0x1B2B0000;
constexpr uint32_t s_zombie_rise1 = 0x1B2C0000;
constexpr uint32_t s_zombie_rise2 = 0x1B2D0000;
constexpr uint32_t s_zombie_rise3 = 0x1B2E0000;
constexpr uint32_t s_zombie_rise4 = 0x1B2F0000;
constexpr uint32_t s_zombie_alive1 = 0x1B300000;
constexpr uint32_t s_zombie_alive2 = 0x1B310000;
constexpr uint32_t s_zombie_alive3 = 0x1B320000;
constexpr uint32_t s_zombie_death1 = 0x1B330000;
constexpr uint32_t s_zombie_death2 = 0x1B340000;
constexpr uint32_t s_zombie_death3 = 0x1B350000;
constexpr uint32_t s_zombie_attack = 0x1B360000;
constexpr uint32_t s_zombie_ouch = 0x1B370000;
constexpr uint32_t s_tree_pause = 0x1B380000;
constexpr uint32_t s_tree_idle = 0x1B390000;
constexpr uint32_t s_tree_awakening1 = 0x1B3A0000;
constexpr uint32_t s_tree_awakening2 = 0x1B3B0000;
constexpr uint32_t s_tree_walk1 = 0x1B3C0000;
constexpr uint32_t s_tree_walk2 = 0x1B3D0000;
constexpr uint32_t s_tree_death1 = 0x1B3E0000;
constexpr uint32_t s_tree_death2 = 0x1B3F0000;
constexpr uint32_t s_tree_death3 = 0x1B400000;
constexpr uint32_t s_tree_death4 = 0x1B410000;
constexpr uint32_t s_tree_death5 = 0x1B420000;
constexpr uint32_t s_tree_attack1 = 0x1B430000;
constexpr uint32_t s_tree_attack2 = 0x1B440000;
constexpr uint32_t s_tree_attack3 = 0x1B450000;
constexpr uint32_t s_tree_ouch = 0x1B460000;
constexpr uint32_t s_arch_1 = 0x1B470000;
constexpr uint32_t s_arch_2 = 0x1B480000;
constexpr uint32_t s_arch_3 = 0x1B490000;
constexpr uint32_t s_arch_4 = 0x1B4A0000;
constexpr uint32_t s_arch_5 = 0x1B4B0000;
constexpr uint32_t s_arch_6 = 0x1B4C0000;
constexpr uint32_t s_arch_7 = 0x1B4D0000;
constexpr uint32_t s_arch_8 = 0x1B4E0000;
constexpr uint32_t s_arch_9 = 0x1B4F0000;
constexpr uint32_t s_arch_10 = 0x1B500000;
constexpr uint32_t s_arch_11 = 0x1B510000;
constexpr uint32_t s_arch_12 = 0x1B520000;
constexpr uint32_t s_arch_13 = 0x1B530000;
constexpr uint32_t s_column = 0x1B540000;
constexpr uint32_t s_sulphur_gas_1 = 0x1B550000;
constexpr uint32_t s_sulphur_gas_2 = 0x1B560000;
constexpr uint32_t s_sulphur_gas_3 = 0x1B570000;
constexpr uint32_t s_fire_pot_1 = 0x1B580000;
constexpr uint32_t s_fire_pot_2 = 0x1B590000;
constexpr uint32_t s_fountain = 0x1B5A0000;
constexpr uint32_t s_force_field_1 = 0x1B5B0000;
constexpr uint32_t s_force_field_2 = 0x1B5C0000;
constexpr uint32_t s_force_field_3 = 0x1B5D0000;
constexpr uint32_t s_force_field_4 = 0x1B5E0000;
constexpr uint32_t s_force_field_die = 0x1B5F0000;
constexpr uint32_t s_force_field_die1 = 0x1B600000;
constexpr uint32_t s_skeleton_hanging = 0x1B610000;

const uint16_t SavedGameConverterArmageddon::GetActorIdOfGate(const uint32_t state32, const int16_t temp1) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    uint16_t actorId = 0;
    if (localState32 == s_anthill)
    {
        actorId = actorIdWarpAntHill;
    }
    else if (localState32 == s_obj_gate1 || localState32 == s_obj_gate2 || localState32 == s_obj_gate3 || localState32 == s_obj_gate4)
    {
        actorId =
            (temp1 == 1) ? actorIdWarpPortal1 :
            (temp1 == 2) ? actorIdWarpPortal2 :
            (temp1 == 3) ? actorIdWarpPortal3 :
            (temp1 == 4) ? actorIdWarpPortal4 :
            (temp1 == 5) ? actorIdWarpPortal5 :
            0;
    }

    return actorId;
}

const uint16_t SavedGameConverterArmageddon::GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    uint16_t actorId = 0;
    switch (dosObject.obclass)
    {
    case nothing:
        actorId = actorIdNone;
        break;
    case playerobj:
        actorId = actorIdPlayer;
        break;
    case bonusobj:
        actorId = GetActorIdOfBonus(dosObject.state32);
        break;
    case succubusobj:
        actorId = actorIdMonsterSuccubus;
        break;
    case batobj:
        actorId = actorIdMonsterBat;
        break;
    case skeletonobj:
        actorId = actorIdMonsterSkeleton;
        break;
    case fatdemonobj:
        actorId = actorIdMonsterWretchedPox;
        break;
    case godessobj:
        actorId = actorIdMonsterViper;
        break;
    case mageobj:
        actorId = actorIdMonsterMage;
        break;
    case pshotobj:
        actorId = actorIdProjectilePlayerShot;
        break;
    case bigpshotobj:
        actorId = actorIdProjectilePlayerBigShot;
        break;
    case mshotobj:
        actorId = actorIdProjectileMageShot;
        break;
    case inertobj:
        actorId = GetActorIdOfInert(dosObject.state32);
        break;
    case bounceobj:
        // Not used in Catacomb Armageddon
        break;
    case grelmobj:
        actorId = actorIdMonsterNemesis;
        break;
    case sshotobj:
        actorId = actorIdProjectileSuccubusShot;
        break;
    case gateobj:
        actorId = GetActorIdOfGate(dosObject.state32, dosObject.temp1);
        break;
    case zombieobj:
        actorId = actorIdMonsterZombie;
        break;
    case antobj:
        actorId = actorIdMonsterAnt;
        break;
    case wetobj:
        actorId = actorIdMonsterDragon;
        break;
    case expobj:
        actorId = GetActorIdOfExplosion(dosObject.state32);
        break;
    case eyeobj:
        actorId = actorIdMonsterEye;
        break;
    case wallskelobj:
        actorId = actorIdMonsterWallSkeleton;
        break;
    case eshotobj:
        actorId = actorIdProjectileEyeShot;
        break;
    case treeobj:
        actorId = actorIdMonsterTree;
        break;
    case gshotobj:
        actorId = actorIdProjectileNemesisShot;
        break;
    case reddemonobj:
        actorId = actorIdMonsterRedDemon;
        break;
    case freezeobj:
        actorId = actorIdBonusFreezeTime;
        break;
    case solidobj:
        actorId = GetActorIdOfSolid(dosObject.state32);
        break;
    case cloudobj:
        // Not used in Catacomb Armageddon
        break;
    case dshotobj:
        actorId = actorIdProjectileDragonShot;
        break;
    case hbunnyobj:
    case bunnyobj:
        actorId = actorIdMonsterBunny;
        break;
    case realsolidobj:
        actorId = GetActorIdOfRealSolid(dosObject.state32);
        break;
    }
    return actorId;
}

const uint16_t SavedGameConverterArmageddon::GetActorIdOfBonus(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    const uint16_t actorId =
        (localState32 == s_boltbonus || localState32 == s_boltbonus2 || localState32 == s_boltbonus3) ? actorIdBonusBolt :
        (localState32 == s_nukebonus || localState32 == s_nukebonus2 || localState32 == s_nukebonus3) ? actorIdBonusNuke :
        (localState32 == s_potionbonus) ? actorIdBonusPotion :
        (localState32 == s_rkeybonus) ? actorIdBonusKeyRed :
        (localState32 == s_ykeybonus) ? actorIdBonusKeyYellow :
        (localState32 == s_gkeybonus) ? actorIdBonusKeyGreen :
        (localState32 == s_bkeybonus) ? actorIdBonusKeyBlue :
        (localState32 == s_chestbonus) ? actorIdBonusChest :
        (localState32 == s_waterchestbonus1 || localState32 == s_waterchestbonus2) ? actorIdBonusWaterChest :
        (localState32 == s_bonus_die) ? actorIdBonusExplosion :
        (localState32 == s_ftimebonus || localState32 == s_ftimebonus2) ? actorIdBonusFreezeTime :
        (localState32 == s_rgem1bonus || localState32 == s_rgem2bonus) ? actorIdBonusGemRed :
        (localState32 == s_ygem1bonus || localState32 == s_ygem2bonus) ? actorIdBonusGemYellow :
        (localState32 == s_ggem1bonus || localState32 == s_ggem2bonus) ? actorIdBonusGemGreen :
        (localState32 == s_bgem1bonus || localState32 == s_bgem2bonus) ? actorIdBonusGemBlue :
        (localState32 == s_pgem1bonus || localState32 == s_pgem2bonus) ? actorIdBonusGemPurple :
        0;
    return actorId;
}

const uint16_t SavedGameConverterArmageddon::GetActorIdOfSolid(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    const uint16_t actorId =
        (localState32 == s_force_field_1 || localState32 == s_force_field_2 || localState32 == s_force_field_3 ||
         localState32 == s_force_field_4 || localState32 == s_force_field_die || localState32 == s_force_field_die1) ? actorIdForceField :
        (localState32 == s_skeleton_hanging) ? actorIdMonsterSkeletonHanging :
        0;
    return actorId;
}

const uint16_t SavedGameConverterArmageddon::GetActorIdOfRealSolid(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    const uint16_t actorId =
        (localState32 == s_godess_statue1 || localState32 == s_godess_statue2) ? actorIdMonsterViper :
        (localState32 == s_tombs0) ? actorIdTomb1 :
        (localState32 == s_tombs1) ? actorIdTomb2 :
        (localState32 == s_tombs2) ? actorIdTomb3 :
        (localState32 == s_tree_idle) ? actorIdMonsterTree :
        (localState32 == s_column) ? actorIdColumn :
        (localState32 == s_sulphur_gas_1 || localState32 == s_sulphur_gas_2 || localState32 == s_sulphur_gas_3) ? actorIdSulphurGas :
        (localState32 == s_fire_pot_1 || localState32 == s_fire_pot_2) ? actorIdFirePot :
        (localState32 == s_fountain) ? actorIdFountain :
        0;
    return actorId;
}

const uint16_t SavedGameConverterArmageddon::GetActorIdOfExplosion(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    const uint16_t actorId =
        (localState32 == s_pshot_exp1 || localState32 == s_pshot_exp2 || localState32 == s_pshot_exp3) ? actorIdProjectilePlayerShot :
        (localState32 == s_explode) ? actorIdBonusExplosion :
        0;
    return actorId;
}

const uint16_t SavedGameConverterArmageddon::GetActorIdOfInert(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    uint16_t actorId = 0;
    switch (localState32)
    {
    case s_pshot_exp1:
    case s_pshot_exp2:
    case s_pshot_exp3:
        actorId = actorIdProjectilePlayerShot;
        break;
    case s_bonus_die:
        actorId = actorIdBonusBolt;
        break;
    case s_walldie1:
    case s_walldie2:
    case s_walldie3:
    case s_walldie4:
    case s_walldie5:
    case s_walldie6:
        actorId = actorIdExplodingWall;
        break;
    case s_fatdemon_blowup1:
    case s_fatdemon_blowup2:
    case s_fatdemon_blowup3:
    case s_fatdemon_blowup4:
    case s_fatdemon_blowup5:
    case s_fatdemon_blowup6:
    case s_fatdemon_blowup7:
    case s_fatdemon_explode:
    case s_fatdemon_feet:
        actorId = actorIdMonsterWretchedPox;
        break;
    case s_wet_die1:
    case s_wet_die2:
    case s_wet_die3:
    case s_wet_die4:
    case s_wet_die5:
        actorId = actorIdMonsterDragon;
        break;
    case s_skel_die1:
    case s_skel_die2:
    case s_skel_die3:
        actorId = actorIdMonsterSkeleton;
        break;
    case s_eye_die1:
    case s_eye_die2:
    case s_eye_die3:
        actorId = actorIdMonsterEye;
        break;
    case s_succubus_death1:
    case s_succubus_death2:
        actorId = actorIdMonsterSuccubus;
        break;
    case s_magedie1:
    case s_magedie2:
        actorId = actorIdMonsterMage;
        break;
    case s_bunny_death1:
    case s_bunny_death2:
    case s_bunny_death3:
        actorId = actorIdMonsterBunny;
        break;
    case s_red_demondie1:
    case s_red_demondie2:
    case s_red_demondie3:
    case s_red_demondie4:
        actorId = actorIdMonsterRedDemon;
        break;
    case s_greldie1:
    case s_greldie2:
    case s_greldie3:
    case s_greldie4:
    case s_greldie5:
    case s_greldie5a:
    case s_greldie6:
        actorId = actorIdMonsterNemesis;
        break;
    case s_batdie1:
    case s_batdie2:
        actorId = actorIdMonsterBat;
        break;
    case s_godessdie1:
    case s_godessdie2:
        actorId = actorIdMonsterViper;
        break;
    case s_ant_die1:
    case s_ant_die2:
    case s_ant_die3:
        actorId = actorIdMonsterAnt;
        break;
    case s_zombie_death1:
    case s_zombie_death2:
    case s_zombie_death3:
        actorId = actorIdMonsterZombie;
        break;
    case s_tree_death1:
    case s_tree_death2:
    case s_tree_death3:
    case s_tree_death4:
    case s_tree_death5:
        actorId = actorIdMonsterTree;
        break;
    }
    return actorId;
}

const DecorateStateId SavedGameConverterArmageddon::GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    DecorateStateId stateId = StateIdWalk;
    switch (dosObject.state32 - m_farPointerOffset)
    {
    case s_player:
    case s_fatdemon_walk1:
    case s_fatdemon_walk2:
    case s_fatdemon_walk3:
    case s_fatdemon_walk4:
    case s_wet_walk1:
    case s_wet_walk2:
    case s_wet_walk3:
    case s_wet_walk4:
    case s_skel_1:
    case s_skel_2:
    case s_skel_3:
    case s_skel_4:
    case s_eye_1:
    case s_eye_2:
    case s_eye_3:
    case s_eye_4:
    case s_succubus_walk1:
    case s_succubus_walk2:
    case s_succubus_walk3:
    case s_succubus_walk4:
    case s_mage1:
    case s_mage2:
    case s_bunny_walk1:
    case s_bunny_walk2:
    case s_red_demon1:
    case s_red_demon2:
    case s_red_demon3:
    case s_red_demon4:
    case s_grel1:
    case s_grel2:
    case s_bat1:
    case s_bat2:
    case s_bat3:
    case s_bat4:
    case s_godess1:
    case s_godess2:
    case s_godess3:
    case s_ant_walk1:
    case s_ant_walk2:
    case s_ant_walk3:
    case s_zombie_alive1:
    case s_zombie_alive2:
    case s_zombie_alive3:
    case s_tree_walk1:
    case s_tree_walk2:
        stateId = StateIdWalk;
        break;
    case s_fatdemon_pause:
    case s_fatdemon_attack1:
    case s_fatdemon_attack2:
    case s_wet_attack1:
    case s_wet_attack2:
    case s_wet_attack3:
    case s_wet_attack4:
    case s_skel_pause:
    case s_skel_attack1:
    case s_skel_attack2:
    case s_skel_attack3:
    case s_eye_pause:
    case s_eye_shootplayer_1:
    case s_eye_shootplayer_2:
    case s_succubus_pause:
    case s_succubus_attack1:
    case s_succubus_attack2:
    case s_succubus_attack3:
    case s_magepause:
    case s_mageattack3:
    case s_bunny_attack1:
    case s_bunny_attack2:
    case s_bunny_pause:
    case s_red_demonpause:
    case s_red_demonattack1:
    case s_red_demonattack2:
    case s_red_demonattack3:
    case s_grelpause:
    case s_grelattack3:
    case s_batpast:
    case s_godesspause:
    case s_godessattack1:
    case s_godessattack2:
    case s_godessattack3:
    case s_ant_attack1:
    case s_ant_pause:
    case s_zombie_pause:
    case s_zombie_attack:
    case s_tree_pause:
    case s_tree_attack1:
    case s_tree_attack2:
    case s_tree_attack3:
        stateId = StateIdAttack;
        break;
    case s_fatdemon_ouch:
    case s_wet_ouch:
    case s_skel_ouch:
    case s_eye_ouch:
    case s_eye_ouch2:
    case s_succubus_ouch:
    case s_mageouch:
    case s_bunny_ouch:
    case s_red_demonouch:
    case s_grelouch:
    case s_godessouch:
    case s_ant_ouch:
    case s_zombie_ouch:
    case s_tree_ouch:
        stateId = StateIdPain;
        break;
    case s_explode:
    case s_bonus_die:
    case s_pshot_exp1:
    case s_pshot_exp2:
    case s_pshot_exp3:
    case s_fatdemon_blowup1:
    case s_fatdemon_blowup2:
    case s_fatdemon_blowup3:
    case s_fatdemon_blowup4:
    case s_fatdemon_blowup5:
    case s_fatdemon_blowup6:
    case s_fatdemon_blowup7:
    case s_fatdemon_explode:
    case s_wet_die1:
    case s_wet_die2:
    case s_wet_die3:
    case s_wet_die4:
    case s_wet_die5:
    case s_skel_die1:
    case s_skel_die2:
    case s_eye_die1:
    case s_eye_die2:
    case s_eye_die3:
    case s_succubus_death1:
    case s_magedie1:
    case s_bunny_death1:
    case s_bunny_death2:
    case s_red_demondie1:
    case s_red_demondie2:
    case s_red_demondie3:
    case s_greldie1:
    case s_greldie2:
    case s_greldie3:
    case s_greldie4:
    case s_greldie5:
    case s_greldie5a:
    case s_batdie1:
    case s_batdie2:
    case s_godessdie1:
    case s_ant_die1:
    case s_zombie_death1:
    case s_zombie_death2:
    case s_tree_death1:
    case s_tree_death2:
    case s_force_field_die:
    case s_force_field_die1:
        stateId = StateIdDying;
        break;
    case s_walldie1:
    case s_walldie2:
    case s_walldie3:
    case s_walldie4:
    case s_walldie5:
    case s_walldie6:
    case s_fatdemon_feet:
    case s_skel_die3:
    case s_succubus_death2:
    case s_magedie2:
    case s_bunny_death3:
    case s_red_demondie4:
    case s_greldie6:
    case s_godessdie2:
    case s_ant_die2:
    case s_ant_die3:
    case s_zombie_death3:
    case s_tree_death3:
    case s_tree_death4:
    case s_tree_death5:
        stateId = StateIdDead;
        break;
    case s_wet_bubbles1:
    case s_wet_bubbles2:
    case s_wet_bubbles3:
    case s_wallskel:
    case s_godess_statue1:
    case s_ant_wait:
    case s_zombie_inground:
    case s_tree_idle:
    case s_skeleton_hanging:
    case s_sulphur_gas_1:
    case s_sulphur_gas_2:
    case s_sulphur_gas_3:
        stateId = StateIdHidden;
        break;
    case s_wet_rise1:
    case s_wet_rise3:
    case s_wet_rise4:
    case s_wet_rise5:
    case s_wallskel2:
    case s_bunny_meta1:
    case s_bunny_meta2:
    case s_godess_statue2:
    case s_ant_egg:
    case s_zombie_risen:
    case s_zombie_rise1:
    case s_zombie_rise2:
    case s_zombie_rise3:
    case s_zombie_rise4:
    case s_tree_awakening1:
    case s_tree_awakening2:
        stateId = StateIdRise;
        break;
    case s_wet_sink1:
    case s_wet_sink2:
    case s_wet_sink3:
        stateId = StateIdSink;
        break;
    case s_wet_peek:
    case s_bunny_right1:
    case s_bunny_right2:
    case s_bunny_right3:
        stateId = StateIdPeek;
        break;
    case s_pshot1:
    case s_pshot2:
    case s_dragon_shot1:
    case s_dragon_shot2:
    case s_eshot1:
    case s_eshot2:
    case s_succubus_shot1:
    case s_mshot1:
    case s_mshot2:
    case s_gshot1:
        stateId = StateIdProjectileFly;
        break;
    case s_obj_gate1:
    case s_obj_gate2:
    case s_obj_gate3:
    case s_obj_gate4:
    case s_anthill:
    case s_boltbonus:
    case s_boltbonus2:
    case s_boltbonus3:
    case s_nukebonus:
    case s_nukebonus2:
    case s_nukebonus3:
    case s_potionbonus:
    case s_rkeybonus:
    case s_ykeybonus:
    case s_gkeybonus:
    case s_bkeybonus:
    case s_chestbonus:
    case s_waterchestbonus1:
    case s_waterchestbonus2:
    case s_rgem1bonus:
    case s_ygem1bonus:
    case s_ggem1bonus:
    case s_bgem1bonus:
    case s_pgem1bonus:
    case s_rgem2bonus:
    case s_ygem2bonus:
    case s_ggem2bonus:
    case s_bgem2bonus:
    case s_pgem2bonus:
    case s_ftimebonus:
    case s_ftimebonus2:
        stateId = StateIdWaitForPickup;
        break;
    case s_tombs0:
    case s_tombs1:
    case s_tombs2:
    case s_column:
    case s_fire_pot_1:
    case s_fire_pot_2:
    case s_fountain:
        stateId = StateIdDecoration;
        break;
    case s_arch_1:
    case s_arch_2:
    case s_arch_3:
    case s_arch_4:
    case s_arch_5:
    case s_arch_6:
    case s_arch_7:
    case s_arch_8:
    case s_arch_9:
    case s_arch_10:
    case s_arch_11:
    case s_arch_12:
    case s_arch_13:
    case s_force_field_1:
    case s_force_field_2:
    case s_force_field_3:
    case s_force_field_4:
        stateId = StateIdArch;
        break;
    case s_bunny_left1:
    case s_bunny_left2:
    case s_bunny_left3:
        stateId = StateIdPeekAlternative;
        break;
    }
    return stateId;
}

const uint16_t SavedGameConverterArmageddon::GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    uint16_t animationFrame = 0;
    switch (dosObject.state32 - m_farPointerOffset)
    {
    case s_pshot1:
    case s_pshot_exp1:
    case s_player:
    case s_explode:
    case s_boltbonus:
    case s_nukebonus:
    case s_potionbonus:
    case s_rkeybonus:
    case s_ykeybonus:
    case s_gkeybonus:
    case s_bkeybonus:
    case s_chestbonus:
    case s_waterchestbonus1:
    case s_rgem1bonus:
    case s_ygem1bonus:
    case s_ggem1bonus:
    case s_bgem1bonus:
    case s_pgem1bonus:
    case s_bonus_die:
    case s_tombs0:
    case s_tombs1:
    case s_tombs2:
    case s_ftimebonus:
    case s_walldie1:
    case s_walldie2:
    case s_obj_gate1:
    case s_anthill:
    case s_fatdemon_walk1:
    case s_fatdemon_attack1:
    case s_fatdemon_ouch:
    case s_fatdemon_blowup1:
    case s_fatdemon_feet:
    case s_wet_bubbles1:
    case s_wet_peek:
    case s_wet_rise1:
    case s_wet_sink1:
    case s_wet_walk1:
    case s_wet_attack1:
    case s_wet_ouch:
    case s_wet_die1:
    case s_dragon_shot1:
    case s_wallskel:
    case s_wallskel2:
    case s_skel_1:
    case s_skel_attack1:
    case s_skel_ouch:
    case s_skel_die1:
    case s_skel_die3:
    case s_eye_1:
    case s_eye_shootplayer_1:
    case s_eye_shootplayer_2:
    case s_eye_ouch:
    case s_eye_pause:
    case s_eye_die1:
    case s_eshot1:
    case s_succubus_walk1:
    case s_succubus_attack1:
    case s_succubus_ouch:
    case s_succubus_death1:
    case s_succubus_death2:
    case s_succubus_shot1:
    case s_mage1:
    case s_mageattack3:
    case s_mageouch:
    case s_magedie1:
    case s_magedie2:
    case s_mshot1:
    case s_bunny_left1:
    case s_bunny_right1:
    case s_bunny_meta1:
    case s_bunny_walk1:
    case s_bunny_attack1:
    case s_bunny_ouch:
    case s_bunny_death1:
    case s_bunny_death3:
    case s_red_demon1:
    case s_red_demonattack1:
    case s_red_demonouch:
    case s_red_demondie1:
    case s_red_demondie4:
    case s_grel1:
    case s_grelattack3:
    case s_grelouch:
    case s_greldie1:
    case s_greldie6:
    case s_gshot1:
    case s_bat1:
    case s_batpast:
    case s_batdie1:
    case s_godess_statue1:
    case s_godess_statue2:
    case s_godess1:
    case s_godessattack1:
    case s_godessouch:
    case s_godessdie1:
    case s_godessdie2:
    case s_ant_wait:
    case s_ant_egg:
    case s_ant_walk1:
    case s_ant_attack1:
    case s_ant_ouch:
    case s_ant_die1:
    case s_ant_die2:
    case s_zombie_risen:
    case s_zombie_inground:
    case s_zombie_rise1:
    case s_zombie_alive1:
    case s_zombie_death1:
    case s_zombie_death3:
    case s_zombie_attack:
    case s_zombie_ouch:
    case s_tree_idle:
    case s_tree_awakening1:
    case s_tree_awakening2:
    case s_tree_walk1:
    case s_tree_death1:
    case s_tree_attack1:
    case s_tree_ouch:
    case s_arch_1:
    case s_arch_2:
    case s_arch_3:
    case s_arch_4:
    case s_arch_5:
    case s_arch_6:
    case s_arch_7:
    case s_arch_8:
    case s_arch_9:
    case s_arch_10:
    case s_arch_11:
    case s_arch_12:
    case s_arch_13:
    case s_column:
    case s_sulphur_gas_1:
    case s_fire_pot_1:
    case s_fountain:
    case s_force_field_1:
    case s_force_field_die:
    case s_force_field_die1:
    case s_skeleton_hanging:
        animationFrame = 0u;
        break;
    case s_pshot2:
    case s_pshot_exp2:
    case s_boltbonus2:
    case s_nukebonus2:
    case s_waterchestbonus2:
    case s_rgem2bonus:
    case s_ygem2bonus:
    case s_ggem2bonus:
    case s_bgem2bonus:
    case s_pgem2bonus:
    case s_ftimebonus2:
    case s_walldie3:
    case s_walldie4:
    case s_obj_gate2:
    case s_fatdemon_walk2:
    case s_fatdemon_attack2:
    case s_fatdemon_blowup2:
    case s_wet_bubbles2:
    case s_wet_rise3:
    case s_wet_sink2:
    case s_wet_walk2:
    case s_wet_attack2:
    case s_wet_die2:
    case s_dragon_shot2:
    case s_skel_2:
    case s_skel_attack2:
    case s_skel_die2:
    case s_eye_2:
    case s_eye_ouch2:
    case s_eye_die2:
    case s_eshot2:
    case s_succubus_pause:
    case s_succubus_walk2:
    case s_succubus_attack2:
    case s_magepause:
    case s_mage2:
    case s_mshot2:
    case s_bunny_left2:
    case s_bunny_right2:
    case s_bunny_meta2:
    case s_bunny_walk2:
    case s_bunny_attack2:
    case s_bunny_pause:
    case s_bunny_death2:
    case s_red_demon2:
    case s_red_demonattack2:
    case s_red_demondie2:
    case s_grelpause:
    case s_grel2:
    case s_greldie2:
    case s_bat2:
    case s_batdie2:
    case s_godess2:
    case s_godessattack2:
    case s_ant_walk2:
    case s_ant_pause:
    case s_ant_die3:
    case s_zombie_pause:
    case s_zombie_rise2:
    case s_zombie_alive2:
    case s_zombie_death2:
    case s_tree_walk2:
    case s_tree_death2:
    case s_tree_death4:
    case s_tree_attack2:
    case s_sulphur_gas_2:
    case s_fire_pot_2:
    case s_force_field_2:
        animationFrame = 1u;
        break;
    case s_pshot_exp3:
    case s_boltbonus3:
    case s_nukebonus3:
    case s_walldie5:
    case s_walldie6:
    case s_obj_gate3:
    case s_fatdemon_pause:
    case s_fatdemon_walk3:
    case s_fatdemon_blowup3:
    case s_wet_bubbles3:
    case s_wet_rise4:
    case s_wet_sink3:
    case s_wet_walk3:
    case s_wet_attack3:
    case s_wet_attack4:
    case s_wet_die3:
    case s_skel_3:
    case s_skel_attack3:
    case s_eye_3:
    case s_eye_die3:
    case s_succubus_walk3:
    case s_succubus_attack3:
    case s_bunny_left3:
    case s_bunny_right3:
    case s_red_demon3:
    case s_red_demonattack3:
    case s_red_demonpause:
    case s_red_demondie3:
    case s_greldie3:
    case s_bat3:
    case s_godess3:
    case s_godessattack3:
    case s_ant_walk3:
    case s_zombie_rise3:
    case s_zombie_alive3:
    case s_tree_death3:
    case s_tree_death5:
    case s_tree_attack3:
    case s_tree_pause:
    case s_sulphur_gas_3:
    case s_force_field_3:
        animationFrame = 2u;
        break;
    case s_obj_gate4:
    case s_fatdemon_walk4:
    case s_fatdemon_blowup4:
    case s_wet_rise5:
    case s_wet_walk4:
    case s_wet_die4:
    case s_skel_pause:
    case s_skel_4:
    case s_eye_4:
    case s_succubus_walk4:
    case s_red_demon4:
    case s_greldie4:
    case s_bat4:
    case s_godesspause:
    case s_zombie_rise4:
    case s_force_field_4:
        animationFrame = 3u;
        break;
    case s_fatdemon_blowup5:
    case s_wet_die5:
    case s_greldie5:
        animationFrame = 4u;
        break;
    case s_fatdemon_blowup6:
    case s_greldie5a:
        animationFrame = 5u;
        break;
    case s_fatdemon_blowup7:
        animationFrame = 6u;
        break;
    case s_fatdemon_explode:
        animationFrame = 7u;
        break;
    }
    return animationFrame;
}

const bool SavedGameConverterArmageddon::IsInertObject(const uint16_t obclass) const
{
    return obclass == inertobj;
}

void SavedGameConverterArmageddon::SetFarPointerOffset(const uint32_t playerState32)
{
    m_farPointerOffset = playerState32 - s_player;
}