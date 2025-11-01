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

#include "SavedGameConverterApocalypse.h"
#include "DecorateAll.h"

// classtypes definitions as seen in DEF.H
constexpr uint16_t nothing = 0;
constexpr uint16_t playerobj = 1;
constexpr uint16_t bonusobj = 2;
constexpr uint16_t aquamanobj = 3;
constexpr uint16_t wizardobj = 4;
constexpr uint16_t trollobj = 5;
constexpr uint16_t wshotobj = 6;
constexpr uint16_t blobobj = 7;
constexpr uint16_t bshotobj = 8;
constexpr uint16_t fmageobj = 9;
constexpr uint16_t ramboneobj = 10;
constexpr uint16_t robotankobj = 11;
constexpr uint16_t rtshotobj = 12;
constexpr uint16_t stompyobj = 13;
constexpr uint16_t syshotobj = 14;
constexpr uint16_t fmshotobj = 15;
constexpr uint16_t rbshotobj = 16;
constexpr uint16_t bugobj = 17;
constexpr uint16_t bgshotobj = 18;
constexpr uint16_t cyborgdemonobj = 19;
constexpr uint16_t invisdudeobj = 20;
constexpr uint16_t bounceobj = 21;
constexpr uint16_t eyeobj = 22;
constexpr uint16_t reyeobj = 23;
constexpr uint16_t rayobj = 24;
constexpr uint16_t rshotobj = 25;
constexpr uint16_t headobj = 26;
constexpr uint16_t hshotobj = 27;
constexpr uint16_t demonobj = 28;
constexpr uint16_t pshotobj = 29;
constexpr uint16_t grelmobj = 30;
constexpr uint16_t freezeobj = 31;
constexpr uint16_t solidobj = 32;
constexpr uint16_t inertobj = 33;
constexpr uint16_t gateobj = 34;
constexpr uint16_t realsolidobj = 35;
constexpr uint16_t expobj = 36;
constexpr uint16_t bigpshotobj = 37;
constexpr uint16_t eshotobj = 38;
constexpr uint16_t gshotobj = 39;

// State pointers as conveniently documented in ReflectionHLE (statetype_ptr_conversion.c).
constexpr uint32_t s_pshot1 = 0x1A6A0000;
constexpr uint32_t s_pshot2 = 0x1A6B0000;
constexpr uint32_t s_pshot_exp1 = 0x1A6C0000;
constexpr uint32_t s_pshot_exp2 = 0x1A6D0000;
constexpr uint32_t s_pshot_exp3 = 0x1A6E0000;
constexpr uint32_t s_player = 0x1A6F0000;
constexpr uint32_t s_explode = 0x1A700000;
constexpr uint32_t s_boltbonus = 0x1A710000;
constexpr uint32_t s_boltbonus2 = 0x1A720000;
constexpr uint32_t s_boltbonus3 = 0x1A730000;
constexpr uint32_t s_nukebonus = 0x1A740000;
constexpr uint32_t s_nukebonus2 = 0x1A750000;
constexpr uint32_t s_nukebonus3 = 0x1A760000;
constexpr uint32_t s_potionbonus = 0x1A770000;
constexpr uint32_t s_rkeybonus = 0x1A780000;
constexpr uint32_t s_ykeybonus = 0x1A790000;
constexpr uint32_t s_gkeybonus = 0x1A7A0000;
constexpr uint32_t s_bkeybonus = 0x1A7B0000;
constexpr uint32_t s_chestbonus = 0x1A7C0000;
constexpr uint32_t s_oldchestbonus = 0x1A7D0000;
constexpr uint32_t s_waterchestbonus1 = 0x1A7E0000;
constexpr uint32_t s_waterchestbonus2 = 0x1A7F0000;
constexpr uint32_t s_waterchestbonus3 = 0x1A800000;
constexpr uint32_t s_rgem1bonus = 0x1A810000;
constexpr uint32_t s_ygem1bonus = 0x1A820000;
constexpr uint32_t s_ggem1bonus = 0x1A830000;
constexpr uint32_t s_bgem1bonus = 0x1A840000;
constexpr uint32_t s_pgem1bonus = 0x1A850000;
constexpr uint32_t s_bonus_die = 0x1A860000;
constexpr uint32_t s_ftimebonus = 0x1A870000;
constexpr uint32_t s_ftimebonus2 = 0x1A880000;
constexpr uint32_t s_walldie1 = 0x1A890000;
constexpr uint32_t s_walldie2 = 0x1A8A0000;
constexpr uint32_t s_walldie3 = 0x1A8B0000;
constexpr uint32_t s_walldie4 = 0x1A8C0000;
constexpr uint32_t s_walldie5 = 0x1A8D0000;
constexpr uint32_t s_walldie6 = 0x1A8E0000;
constexpr uint32_t s_portal_wait = 0x1A8F0000;
constexpr uint32_t s_portal1 = 0x1A900000;
constexpr uint32_t s_portal2 = 0x1A910000;
constexpr uint32_t s_portal3 = 0x1A920000;
constexpr uint32_t s_portal4 = 0x1A930000;
constexpr uint32_t s_portal5 = 0x1A940000;
constexpr uint32_t s_portal6 = 0x1A950000;
constexpr uint32_t s_aqua_under1 = 0x1A960000;
constexpr uint32_t s_aqua_under2 = 0x1A970000;
constexpr uint32_t s_aqua_under3 = 0x1A980000;
constexpr uint32_t s_aqua_left = 0x1A990000;
constexpr uint32_t s_aqua_right = 0x1A9A0000;
constexpr uint32_t s_aqua_rise1 = 0x1A9B0000;
constexpr uint32_t s_aqua_rise2 = 0x1A9C0000;
constexpr uint32_t s_aqua_sink1 = 0x1A9D0000;
constexpr uint32_t s_aqua_sink2 = 0x1A9E0000;
constexpr uint32_t s_aqua_walk1 = 0x1A9F0000;
constexpr uint32_t s_aqua_walk2 = 0x1AA00000;
constexpr uint32_t s_aqua_attack1 = 0x1AA10000;
constexpr uint32_t s_aqua_attack2 = 0x1AA20000;
constexpr uint32_t s_aqua_die1 = 0x1AA30000;
constexpr uint32_t s_aqua_die2 = 0x1AA40000;
constexpr uint32_t s_aqua_die3 = 0x1AA50000;
constexpr uint32_t s_aqua_die4 = 0x1AA60000;
constexpr uint32_t s_aqua_die5 = 0x1AA70000;
constexpr uint32_t s_aqua_die6 = 0x1AA80000;
constexpr uint32_t s_aqua_die7 = 0x1AA90000;
constexpr uint32_t s_wizard_walk1 = 0x1AAA0000;
constexpr uint32_t s_wizard_walk2 = 0x1AAB0000;
constexpr uint32_t s_wizard_walk3 = 0x1AAC0000;
constexpr uint32_t s_wizard_walk4 = 0x1AAD0000;
constexpr uint32_t s_wizard_attack1 = 0x1AAE0000;
constexpr uint32_t s_wizard_attack2 = 0x1AAF0000;
constexpr uint32_t s_wizard_ouch = 0x1AB00000;
constexpr uint32_t s_wizard_die1 = 0x1AB10000;
constexpr uint32_t s_wizard_die2 = 0x1AB20000;
constexpr uint32_t s_wizard_die3 = 0x1AB30000;
constexpr uint32_t s_wizard_die4 = 0x1AB40000;
constexpr uint32_t s_wizard_shoot1 = 0x1AB50000;
constexpr uint32_t s_wizard_shoot2 = 0x1AB60000;
constexpr uint32_t s_wizard_shoot3 = 0x1AB70000;
constexpr uint32_t s_wizard_shot1 = 0x1AB80000;
constexpr uint32_t s_wizard_shot2 = 0x1AB90000;
constexpr uint32_t s_ray_under = 0x1ABA0000;
constexpr uint32_t s_ray_rise = 0x1ABB0000;
constexpr uint32_t s_ray_sink = 0x1ABC0000;
constexpr uint32_t s_ray_fly1 = 0x1ABD0000;
constexpr uint32_t s_ray_fly2 = 0x1ABE0000;
constexpr uint32_t s_ray_fly3 = 0x1ABF0000;
constexpr uint32_t s_ray_fly4 = 0x1AC00000;
constexpr uint32_t s_ray_attack1 = 0x1AC10000;
constexpr uint32_t s_ray_attack2 = 0x1AC20000;
constexpr uint32_t s_ray_attack3 = 0x1AC30000;
constexpr uint32_t s_ray_die1 = 0x1AC40000;
constexpr uint32_t s_ray_die2 = 0x1AC50000;
constexpr uint32_t s_ray_shot1 = 0x1AC60000;
constexpr uint32_t s_ray_shot2 = 0x1AC70000;
constexpr uint32_t s_blob_gnd1 = 0x1AC80000;
constexpr uint32_t s_blob_gnd2 = 0x1AC90000;
constexpr uint32_t s_blob_rise1 = 0x1ACA0000;
constexpr uint32_t s_blob_rise2 = 0x1ACB0000;
constexpr uint32_t s_blob_sink1 = 0x1ACC0000;
constexpr uint32_t s_blob_sink2 = 0x1ACD0000;
constexpr uint32_t s_blob_walk1 = 0x1ACE0000;
constexpr uint32_t s_blob_walk2 = 0x1ACF0000;
constexpr uint32_t s_blob_walk3 = 0x1AD00000;
constexpr uint32_t s_blob_ouch = 0x1AD10000;
constexpr uint32_t s_blob_die1 = 0x1AD20000;
constexpr uint32_t s_blob_die2 = 0x1AD30000;
constexpr uint32_t s_blob_die3 = 0x1AD40000;
constexpr uint32_t s_blob_shot1 = 0x1AD50000;
constexpr uint32_t s_blob_shot2 = 0x1AD60000;
constexpr uint32_t s_skel_1 = 0x1AD70000;
constexpr uint32_t s_skel_2 = 0x1AD80000;
constexpr uint32_t s_skel_3 = 0x1AD90000;
constexpr uint32_t s_skel_4 = 0x1ADA0000;
constexpr uint32_t s_skel_attack1 = 0x1ADB0000;
constexpr uint32_t s_skel_attack2 = 0x1ADC0000;
constexpr uint32_t s_skel_attack3 = 0x1ADD0000;
constexpr uint32_t s_skel_attack4 = 0x1ADE0000;
constexpr uint32_t s_skel_ouch = 0x1ADF0000;
constexpr uint32_t s_skel_die1 = 0x1AE00000;
constexpr uint32_t s_skel_die2 = 0x1AE10000;
constexpr uint32_t s_skel_die3 = 0x1AE20000;
constexpr uint32_t s_skel_shot1 = 0x1AE30000;
constexpr uint32_t s_skel_shot2 = 0x1AE40000;
constexpr uint32_t s_fmage1 = 0x1AE50000;
constexpr uint32_t s_fmage2 = 0x1AE60000;
constexpr uint32_t s_fmage3 = 0x1AE70000;
constexpr uint32_t s_fmageattack1 = 0x1AE80000;
constexpr uint32_t s_fmageattack2 = 0x1AE90000;
constexpr uint32_t s_fmageattack3 = 0x1AEA0000;
constexpr uint32_t s_fmageouch = 0x1AEB0000;
constexpr uint32_t s_fmagedie1 = 0x1AEC0000;
constexpr uint32_t s_fmagedie2 = 0x1AED0000;
constexpr uint32_t s_fmagedie3 = 0x1AEE0000;
constexpr uint32_t s_fmshot1 = 0x1AEF0000;
constexpr uint32_t s_fmshot2 = 0x1AF00000;
constexpr uint32_t s_robotank_walk1 = 0x1AF10000;
constexpr uint32_t s_robotank_walk2 = 0x1AF20000;
constexpr uint32_t s_robotank_walk3 = 0x1AF30000;
constexpr uint32_t s_robotank_walk4 = 0x1AF40000;
constexpr uint32_t s_robotank_attack1 = 0x1AF50000;
constexpr uint32_t s_robotank_attack2 = 0x1AF60000;
constexpr uint32_t s_robotank_attack3 = 0x1AF70000;
constexpr uint32_t s_robotank_attack4 = 0x1AF80000;
constexpr uint32_t s_robotank_death1 = 0x1AF90000;
constexpr uint32_t s_robotank_death2 = 0x1AFA0000;
constexpr uint32_t s_robotank_death3 = 0x1AFB0000;
constexpr uint32_t s_robotank_death4 = 0x1AFC0000;
constexpr uint32_t s_robotank_death5 = 0x1AFD0000;
constexpr uint32_t s_robotank_shot1 = 0x1AFE0000;
constexpr uint32_t s_robotank_shot2 = 0x1AFF0000;
constexpr uint32_t s_stompy_walk1 = 0x1B000000;
constexpr uint32_t s_stompy_walk2 = 0x1B010000;
constexpr uint32_t s_stompy_walk3 = 0x1B020000;
constexpr uint32_t s_stompy_walk4 = 0x1B030000;
constexpr uint32_t s_stompy_attack1 = 0x1B040000;
constexpr uint32_t s_stompy_attack2 = 0x1B050000;
constexpr uint32_t s_stompy_attack3 = 0x1B060000;
constexpr uint32_t s_stompy_attack4 = 0x1B070000;
constexpr uint32_t s_stompy_ouch = 0x1B080000;
constexpr uint32_t s_stompy_death1 = 0x1B090000;
constexpr uint32_t s_stompy_death2 = 0x1B0A0000;
constexpr uint32_t s_stompy_death3 = 0x1B0B0000;
constexpr uint32_t s_stompy_death4 = 0x1B0C0000;
constexpr uint32_t s_stompy_shot1 = 0x1B0D0000;
constexpr uint32_t s_stompy_shot2 = 0x1B0E0000;
constexpr uint32_t s_stompy_shot3 = 0x1B0F0000;
constexpr uint32_t s_stompy_shot4 = 0x1B100000;
constexpr uint32_t s_stompy_shot5 = 0x1B110000;
constexpr uint32_t s_bug_walk1 = 0x1B120000;
constexpr uint32_t s_bug_walk2 = 0x1B130000;
constexpr uint32_t s_bug_walk3 = 0x1B140000;
constexpr uint32_t s_bug_attack1 = 0x1B150000;
constexpr uint32_t s_bug_attack2 = 0x1B160000;
constexpr uint32_t s_bug_attack3 = 0x1B170000;
constexpr uint32_t s_bug_attack4 = 0x1B180000;
constexpr uint32_t s_bug_ouch = 0x1B190000;
constexpr uint32_t s_bug_death1 = 0x1B1A0000;
constexpr uint32_t s_bug_death2 = 0x1B1B0000;
constexpr uint32_t s_bug_shot1 = 0x1B1C0000;
constexpr uint32_t s_bug_shot2 = 0x1B1D0000;
constexpr uint32_t s_eye_pause = 0x1B1E0000;
constexpr uint32_t s_eye_1 = 0x1B1F0000;
constexpr uint32_t s_eye_2 = 0x1B200000;
constexpr uint32_t s_eye_3 = 0x1B210000;
constexpr uint32_t s_eye_4 = 0x1B220000;
constexpr uint32_t s_eye_shootplayer_1 = 0x1B230000;
constexpr uint32_t s_eye_shootplayer_2 = 0x1B240000;
constexpr uint32_t s_eye_ouch = 0x1B250000;
constexpr uint32_t s_eye_ouch2 = 0x1B260000;
constexpr uint32_t s_eye_die1 = 0x1B270000;
constexpr uint32_t s_eye_die2 = 0x1B280000;
constexpr uint32_t s_eye_die3 = 0x1B290000;
constexpr uint32_t s_eye_die4 = 0x1B2A0000;
constexpr uint32_t s_eshot1 = 0x1B2B0000;
constexpr uint32_t s_eshot2 = 0x1B2C0000;
constexpr uint32_t s_reye_1 = 0x1B2D0000;
constexpr uint32_t s_reye_2 = 0x1B2E0000;
constexpr uint32_t s_reye_3 = 0x1B2F0000;
constexpr uint32_t s_reye_4 = 0x1B300000;
constexpr uint32_t s_reye_ouch = 0x1B310000;
constexpr uint32_t s_reye_ouch2 = 0x1B320000;
constexpr uint32_t s_reye_die1 = 0x1B330000;
constexpr uint32_t s_reye_die2 = 0x1B340000;
constexpr uint32_t s_reye_die3 = 0x1B350000;
constexpr uint32_t s_reye_die4 = 0x1B360000;
constexpr uint32_t s_head = 0x1B370000;
constexpr uint32_t s_head_shot1 = 0x1B380000;
constexpr uint32_t s_head_shot2 = 0x1B390000;
constexpr uint32_t s_demonpause = 0x1B3A0000;
constexpr uint32_t s_demon1 = 0x1B3B0000;
constexpr uint32_t s_demon2 = 0x1B3C0000;
constexpr uint32_t s_demon3 = 0x1B3D0000;
constexpr uint32_t s_demon4 = 0x1B3E0000;
constexpr uint32_t s_demonattack1 = 0x1B3F0000;
constexpr uint32_t s_demonattack2 = 0x1B400000;
constexpr uint32_t s_demonattack3 = 0x1B410000;
constexpr uint32_t s_demonouch = 0x1B420000;
constexpr uint32_t s_demondie1 = 0x1B430000;
constexpr uint32_t s_demondie2 = 0x1B440000;
constexpr uint32_t s_demondie3 = 0x1B450000;
constexpr uint32_t s_trollpause = 0x1B460000;
constexpr uint32_t s_troll1 = 0x1B470000;
constexpr uint32_t s_troll2 = 0x1B480000;
constexpr uint32_t s_troll3 = 0x1B490000;
constexpr uint32_t s_troll4 = 0x1B4A0000;
constexpr uint32_t s_trollattack1 = 0x1B4B0000;
constexpr uint32_t s_trollattack2 = 0x1B4C0000;
constexpr uint32_t s_trollouch = 0x1B4D0000;
constexpr uint32_t s_trolldie1 = 0x1B4E0000;
constexpr uint32_t s_trolldie2 = 0x1B4F0000;
constexpr uint32_t s_trolldie3 = 0x1B500000;
constexpr uint32_t s_cyborg_demon1 = 0x1B510000;
constexpr uint32_t s_cyborg_demon2 = 0x1B520000;
constexpr uint32_t s_cyborg_demon3 = 0x1B530000;
constexpr uint32_t s_cyborg_demon4 = 0x1B540000;
constexpr uint32_t s_cyborg_demonattack1 = 0x1B550000;
constexpr uint32_t s_cyborg_demonattack2 = 0x1B560000;
constexpr uint32_t s_cyborg_demonattack3 = 0x1B570000;
constexpr uint32_t s_cyborg_demonouch = 0x1B580000;
constexpr uint32_t s_cyborg_demondie1 = 0x1B590000;
constexpr uint32_t s_cyborg_demondie2 = 0x1B5A0000;
constexpr uint32_t s_cyborg_demondie3 = 0x1B5B0000;
constexpr uint32_t s_invis_fizz1 = 0x1B5C0000;
constexpr uint32_t s_invis_fizz2 = 0x1B5D0000;
constexpr uint32_t s_invis_fizz3 = 0x1B5E0000;
constexpr uint32_t s_invis_walk = 0x1B5F0000;
constexpr uint32_t s_invis_attack = 0x1B600000;
constexpr uint32_t s_invis_pause = 0x1B610000;
constexpr uint32_t s_invis_flash1 = 0x1B620000;
constexpr uint32_t s_invis_flash2 = 0x1B630000;
constexpr uint32_t s_invis_flash3 = 0x1B640000;
constexpr uint32_t s_invis_death1 = 0x1B650000;
constexpr uint32_t s_invis_death2 = 0x1B660000;
constexpr uint32_t s_invis_death3 = 0x1B670000;
constexpr uint32_t s_bounce1 = 0x1B680000;
constexpr uint32_t s_bounce2 = 0x1B690000;
constexpr uint32_t s_grelpause = 0x1B6A0000;
constexpr uint32_t s_grel1 = 0x1B6B0000;
constexpr uint32_t s_grel2 = 0x1B6C0000;
constexpr uint32_t s_grelattack3 = 0x1B6D0000;
constexpr uint32_t s_grelouch = 0x1B6E0000;
constexpr uint32_t s_greldie1 = 0x1B6F0000;
constexpr uint32_t s_greldie2 = 0x1B700000;
constexpr uint32_t s_greldie3 = 0x1B710000;
constexpr uint32_t s_greldie4 = 0x1B720000;
constexpr uint32_t s_greldie5 = 0x1B730000;
constexpr uint32_t s_greldie5a = 0x1B740000;
constexpr uint32_t s_greldie6 = 0x1B750000;
constexpr uint32_t s_gshot1 = 0x1B760000;
constexpr uint32_t s_column1 = 0x1B770000;
constexpr uint32_t s_column2 = 0x1B780000;
constexpr uint32_t s_column3 = 0x1B790000;
constexpr uint32_t s_column4 = 0x1B7A0000;
constexpr uint32_t s_column5 = 0x1B7B0000;
constexpr uint32_t s_ffire_pot = 0x1B7C0000;
constexpr uint32_t s_ofire_pot1 = 0x1B7D0000;
constexpr uint32_t s_ofire_pot2 = 0x1B7E0000;
constexpr uint32_t s_tomb1 = 0x1B7F0000;
constexpr uint32_t s_tomb2 = 0x1B800000;
constexpr uint32_t s_force_field_1 = 0x1B810000;
constexpr uint32_t s_force_field_2 = 0x1B820000;
constexpr uint32_t s_force_field_3 = 0x1B830000;
constexpr uint32_t s_force_field_4 = 0x1B840000;
constexpr uint32_t s_force_field_die = 0x1B850000;
constexpr uint32_t s_force_field_die1 = 0x1B860000;
constexpr uint32_t s_invis_wall_control = 0x1B870000;

SavedGameConverterApocalypse::SavedGameConverterApocalypse(const uint32_t farPointerOffset) :
    m_farPointerOffset(farPointerOffset)
{
}

const uint16_t SavedGameConverterApocalypse::GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
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
    case aquamanobj:
        actorId = actorIdMonsterAquaMan;
        break;
    case wizardobj:
        actorId = actorIdMonsterWizard;
        break;
    case trollobj:
        actorId = actorIdMonsterTroll;
        break;
    case wshotobj:
        actorId = actorIdProjectileWizardShot;
        break;
    case blobobj:
        actorId = actorIdMonsterBlob;
        break;
    case bshotobj:
        actorId = actorIdProjectileBlobShot;
        break;
    case fmageobj:
        actorId = actorIdMonsterAndroidMage;
        break;
    case ramboneobj:
        actorId = actorIdMonsterSkeleton;
        break;
    case robotankobj:
        actorId = actorIdMonsterRoboTank;
        break;
    case rtshotobj:
        actorId = actorIdProjectileRoboTankShot;
        break;
    case stompyobj:
        actorId = actorIdMonsterStompy;
        break;
    case syshotobj:
        actorId = actorIdProjectileStompyShot;
        break;
    case fmshotobj:
        actorId = actorIdProjectileAndroidMageShot;
        break;
    case rbshotobj:
        actorId = actorIdProjectileSkeletonShot;
        break;
    case bugobj:
        actorId = actorIdMonsterBug;
        break;
    case bgshotobj:
        actorId = actorIdProjectileBugShot;
        break;
    case cyborgdemonobj:
        actorId = actorIdMonsterCyborgDemon;
        break;
    case invisdudeobj:
        actorId = actorIdMonsterInvisDude;
        break;
    case bounceobj:
        actorId = actorIdMonsterBounce;
        break;
    case eyeobj:
        actorId = actorIdMonsterShooterEye;
        break;
    case reyeobj:
        actorId = actorIdMonsterRunningEye;
        break;
    case rayobj:
        actorId = actorIdMonsterRay;
        break;
    case rshotobj:
        actorId = actorIdProjectileRayShot;
        break;
    case headobj:
        actorId = actorIdMonsterTimeLord;
        break;
    case hshotobj:
        actorId = actorIdProjectileTimeLordShot;
        break;
    case demonobj:
        actorId = actorIdMonsterDemon;
        break;
    case pshotobj:
        actorId = actorIdProjectilePlayerShot;
        break;
    case grelmobj:
        actorId = actorIdMonsterNemesis;
        break;
    case freezeobj:
        actorId = actorIdBonusFreezeTime;
        break;
    case solidobj:
        actorId = GetActorIdOfSolid(dosObject.state32);
        break;
    case inertobj:
        actorId = GetActorIdOfInert(dosObject.state32);
        break;
    case gateobj:
        actorId = actorIdPortal;
        break;
    case realsolidobj:
        actorId = GetActorIdOfRealSolid(dosObject.state32);
        break;
    case expobj:
        actorId = GetActorIdOfExplosion(dosObject.state32);
        break;
    case bigpshotobj:
        actorId = actorIdProjectilePlayerBigShot;
        break;
    case eshotobj:
        actorId = actorIdProjectileEyeShot;
        break;
    case gshotobj:
        actorId = actorIdProjectileNemesisShot;
        break;
    }
    return actorId;
}

const uint16_t SavedGameConverterApocalypse::GetActorIdOfBonus(const uint32_t state32) const
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
        (localState32 == s_oldchestbonus) ? actorIdBonusOldChest :
        (localState32 == s_waterchestbonus1 || localState32 == s_waterchestbonus2 || localState32 == s_waterchestbonus3) ? actorIdBonusWaterChest :
        (localState32 == s_bonus_die) ? actorIdBonusExplosion :
        (localState32 == s_ftimebonus || localState32 == s_ftimebonus2) ? actorIdBonusFreezeTime :
        (localState32 == s_rgem1bonus) ? actorIdBonusGemRed :
        (localState32 == s_ygem1bonus) ? actorIdBonusGemYellow :
        (localState32 == s_ggem1bonus) ? actorIdBonusGemGreen :
        (localState32 == s_bgem1bonus) ? actorIdBonusGemBlue :
        (localState32 == s_pgem1bonus) ? actorIdBonusGemPurple :
        0;
    return actorId;
}

const uint16_t SavedGameConverterApocalypse::GetActorIdOfSolid(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    const uint16_t actorId =
        (localState32 == s_force_field_1 || localState32 == s_force_field_2 || localState32 == s_force_field_3 || localState32 == s_force_field_4) ? actorIdForceField :
        (localState32 == s_invis_wall_control) ? actorIdFakeWall :
        0;
    return actorId;
}

const uint16_t SavedGameConverterApocalypse::GetActorIdOfExplosion(const uint32_t state32) const
{
    const uint32_t localState32 = state32 - m_farPointerOffset;
    const uint16_t actorId =
        (localState32 == s_pshot_exp1 || localState32 == s_pshot_exp2 || localState32 == s_pshot_exp3) ? actorIdProjectilePlayerShot :
        (localState32 == s_explode) ? actorIdBonusExplosion :
        0;
    return actorId;
}

const uint16_t SavedGameConverterApocalypse::GetActorIdOfInert(const uint32_t state32) const
{
    uint16_t actorId = 0;
    switch (state32 - m_farPointerOffset)
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
    case s_skel_die1:
    case s_skel_die2:
    case s_skel_die3:
        actorId = actorIdMonsterSkeleton;
        break;
    case s_eye_die1:
    case s_eye_die2:
    case s_eye_die3:
    case s_eye_die4:
        actorId = actorIdMonsterShooterEye;
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
    case s_aqua_die1:
    case s_aqua_die2:
    case s_aqua_die3:
    case s_aqua_die4:
    case s_aqua_die5:
    case s_aqua_die6:
    case s_aqua_die7:
        actorId = actorIdMonsterAquaMan;
        break;
    case s_wizard_die1:
    case s_wizard_die2:
    case s_wizard_die3:
    case s_wizard_die4:
        actorId = actorIdMonsterWizard;
        break;
    case s_ray_die1:
    case s_ray_die2:
        actorId = actorIdMonsterRay;
        break;
    case s_blob_die1:
    case s_blob_die2:
    case s_blob_die3:
        actorId = actorIdMonsterBlob;
        break;
    case s_fmagedie1:
    case s_fmagedie2:
    case s_fmagedie3:
        actorId = actorIdMonsterAndroidMage;
        break;
    case s_robotank_death1:
    case s_robotank_death2:
    case s_robotank_death3:
    case s_robotank_death4:
    case s_robotank_death5:
        actorId = actorIdMonsterRoboTank;
        break;
    case s_stompy_death1:
    case s_stompy_death2:
    case s_stompy_death3:
    case s_stompy_death4:
        actorId = actorIdMonsterStompy;
        break;
    case s_bug_death1:
    case s_bug_death2:
        actorId = actorIdMonsterBug;
        break;
    case s_reye_die1:
    case s_reye_die2:
    case s_reye_die3:
    case s_reye_die4:
        actorId = actorIdMonsterRunningEye;
        break;
    case s_demondie1:
    case s_demondie2:
    case s_demondie3:
        actorId = actorIdMonsterDemon;
        break;
    case s_trolldie1:
    case s_trolldie2:
    case s_trolldie3:
        actorId = actorIdMonsterTroll;
        break;
    case s_cyborg_demonouch:
    case s_cyborg_demondie1:
    case s_cyborg_demondie2:
    case s_cyborg_demondie3:
        actorId = actorIdMonsterCyborgDemon;
        break;
    case s_invis_death1:
    case s_invis_death2:
    case s_invis_death3:
        actorId = actorIdMonsterInvisDude;
        break;
    case s_force_field_die:
    case s_force_field_die1:
        actorId = actorIdForceField;
        break;
    }
    return actorId;
}

const uint16_t SavedGameConverterApocalypse::GetActorIdOfRealSolid(const uint32_t state32) const
{
    uint16_t actorId = actorIdNone;
    switch (state32 - m_farPointerOffset)
    {
    case s_head:
        actorId = actorIdMonsterTimeLord;
        break;
    case s_column1:
        actorId = actorIdColumn1;
        break;
    case s_column2:
        actorId = actorIdColumn2;
        break;
    case s_column3:
        actorId = actorIdColumn3;
        break;
    case s_column4:
        actorId = actorIdColumn4;
        break;
    case s_column5:
        actorId = actorIdColumn5;
        break;
    case s_ffire_pot:
        actorId = actorIdFutureFirePot;
        break;
    case s_ofire_pot1:
    case s_ofire_pot2:
        actorId = actorIdOldFirePot;
        break;
    case s_tomb1:
        actorId = actorIdTomb1;
        break;
    case s_tomb2:
        actorId = actorIdTomb2;
        break;
    }
    return actorId;
}

const DecorateStateId SavedGameConverterApocalypse::GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
{
    DecorateStateId stateId = StateIdWalk;
    switch (dosObject.state32 - m_farPointerOffset)
    {
    case s_player:
    case s_aqua_walk1:
    case s_aqua_walk2:
    case s_wizard_walk1:
    case s_wizard_walk2:
    case s_wizard_walk3:
    case s_wizard_walk4:
    case s_ray_fly1:
    case s_ray_fly2:
    case s_ray_fly3:
    case s_ray_fly4:
    case s_blob_walk1:
    case s_blob_walk2:
    case s_blob_walk3:
    case s_skel_1:
    case s_skel_2:
    case s_skel_3:
    case s_skel_4:
    case s_fmage1:
    case s_fmage2:
    case s_fmage3:
    case s_robotank_walk1:
    case s_robotank_walk2:
    case s_robotank_walk3:
    case s_robotank_walk4:
    case s_stompy_walk1:
    case s_stompy_walk2:
    case s_stompy_walk3:
    case s_stompy_walk4:
    case s_bug_walk1:
    case s_bug_walk2:
    case s_bug_walk3:
    case s_eye_1:
    case s_eye_2:
    case s_eye_3:
    case s_eye_4:
    case s_reye_1:
    case s_reye_2:
    case s_reye_3:
    case s_reye_4:
    case s_head:
    case s_demon1:
    case s_demon2:
    case s_demon3:
    case s_demon4:
    case s_troll1:
    case s_troll2:
    case s_troll3:
    case s_troll4:
    case s_cyborg_demon1:
    case s_cyborg_demon2:
    case s_cyborg_demon3:
    case s_cyborg_demon4:
    case s_invis_walk:
    case s_grel1:
    case s_grel2:
    case s_bounce1:
    case s_bounce2:
        stateId = StateIdWalk;
        break;
    case s_aqua_attack1:
    case s_aqua_attack2:
    case s_wizard_attack1:
    case s_wizard_attack2:
    case s_wizard_shoot1:
    case s_wizard_shoot2:
    case s_wizard_shoot3:
    case s_ray_attack1:
    case s_ray_attack2:
    case s_ray_attack3:
    case s_skel_attack1:
    case s_skel_attack2:
    case s_skel_attack3:
    case s_skel_attack4:
    case s_fmageattack1:
    case s_fmageattack2:
    case s_fmageattack3:
    case s_robotank_attack1:
    case s_robotank_attack2:
    case s_robotank_attack3:
    case s_robotank_attack4:
    case s_stompy_attack1:
    case s_stompy_attack2:
    case s_stompy_attack3:
    case s_stompy_attack4:
    case s_bug_attack1:
    case s_bug_attack2:
    case s_bug_attack3:
    case s_bug_attack4:
    case s_eye_pause:
    case s_eye_shootplayer_1:
    case s_eye_shootplayer_2:
    case s_demonpause:
    case s_demonattack1:
    case s_demonattack2:
    case s_demonattack3:
    case s_trollpause:
    case s_trollattack1:
    case s_trollattack2:
    case s_cyborg_demonattack1:
    case s_cyborg_demonattack2:
    case s_cyborg_demonattack3:
    case s_invis_attack:
    case s_invis_pause:
    case s_grelpause:
    case s_grelattack3:
        stateId = StateIdAttack;
        break;
    case s_wizard_ouch:
    case s_blob_ouch:
    case s_skel_ouch:
    case s_fmageouch:
    case s_stompy_ouch:
    case s_bug_ouch:
    case s_eye_ouch:
    case s_eye_ouch2:
    case s_reye_ouch:
    case s_reye_ouch2:
    case s_demonouch:
    case s_trollouch:
    case s_invis_fizz1:
    case s_invis_fizz2:
    case s_invis_fizz3:
    case s_grelouch:
        stateId = StateIdPain;
        break;
    case s_pshot_exp1:
    case s_pshot_exp2:
    case s_pshot_exp3:
    case s_explode:
    case s_bonus_die:
    case s_aqua_die1:
    case s_aqua_die2:
    case s_aqua_die3:
    case s_aqua_die4:
    case s_aqua_die5:
    case s_aqua_die6:
    case s_aqua_die7:
    case s_wizard_die1:
    case s_wizard_die2:
    case s_wizard_die3:
    case s_ray_die1:
    case s_ray_die2:
    case s_blob_die1:
    case s_blob_die2:
    case s_blob_die3:
    case s_skel_die1:
    case s_skel_die2:
    case s_skel_die3:
    case s_fmagedie1:
    case s_fmagedie2:
    case s_robotank_death1:
    case s_robotank_death2:
    case s_robotank_death3:
    case s_robotank_death4:
    case s_stompy_death1:
    case s_stompy_death2:
    case s_stompy_death3:
    case s_stompy_death4:
    case s_bug_death1:
    case s_eye_die1:
    case s_eye_die2:
    case s_eye_die3:
    case s_reye_die1:
    case s_reye_die2:
    case s_reye_die3:
    case s_demondie1:
    case s_demondie2:
    case s_trolldie1:
    case s_trolldie2:
    case s_cyborg_demonouch:
    case s_cyborg_demondie1:
    case s_cyborg_demondie2:
    case s_invis_death1:
    case s_invis_death2:
    case s_greldie1:
    case s_greldie2:
    case s_greldie3:
    case s_greldie4:
    case s_greldie5:
    case s_greldie5a:
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
    case s_wizard_die4:
    case s_fmagedie3:
    case s_robotank_death5:
    case s_bug_death2:
    case s_eye_die4:
    case s_reye_die4:
    case s_demondie3:
    case s_trolldie3:
    case s_cyborg_demondie3:
    case s_invis_death3:
    case s_greldie6:
        stateId = StateIdDead;
        break;
    case s_portal_wait:
    case s_aqua_under1:
    case s_aqua_under2:
    case s_aqua_under3:
    case s_ray_under:
    case s_blob_gnd1:
    case s_blob_gnd2:
    case s_invis_wall_control:
        stateId = StateIdHidden;
        break;
    case s_aqua_rise1:
    case s_aqua_rise2:
    case s_ray_rise:
    case s_blob_rise1:
    case s_blob_rise2:
    case s_stompy_shot1:
    case s_stompy_shot2:
    case s_stompy_shot3:
        stateId = StateIdRise;
        break;
    case s_aqua_sink1:
    case s_aqua_sink2:
    case s_ray_sink:
    case s_blob_sink1:
    case s_blob_sink2:
        stateId = StateIdSink;
        break;
    case s_aqua_left:
    case s_invis_flash1:
    case s_invis_flash2:
    case s_invis_flash3:
        stateId = StateIdPeek;
        break;
    case s_wizard_shot1:
    case s_wizard_shot2:
    case s_ray_shot1:
    case s_ray_shot2:
    case s_blob_shot1:
    case s_blob_shot2:
    case s_skel_shot1:
    case s_skel_shot2:
    case s_fmshot1:
    case s_fmshot2:
    case s_robotank_shot1:
    case s_robotank_shot2:
    case s_stompy_shot4:
    case s_stompy_shot5:
    case s_bug_shot1:
    case s_bug_shot2:
    case s_eshot1:
    case s_eshot2:
    case s_head_shot1:
    case s_head_shot2:
    case s_pshot1:
    case s_pshot2:
    case s_gshot1:
        stateId = StateIdProjectileFly;
        break;
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
    case s_oldchestbonus:
    case s_waterchestbonus1:
    case s_waterchestbonus2:
    case s_waterchestbonus3:
    case s_rgem1bonus:
    case s_ygem1bonus:
    case s_ggem1bonus:
    case s_bgem1bonus:
    case s_pgem1bonus:
    case s_ftimebonus:
    case s_ftimebonus2:
    case s_portal1:
    case s_portal2:
    case s_portal3:
    case s_portal4:
    case s_portal5:
    case s_portal6:
        stateId = StateIdWaitForPickup;
        break;
        stateId = StateIdPickup;
        break;
    case s_column1:
    case s_column2:
    case s_column3:
    case s_column4:
    case s_column5:
    case s_ffire_pot:
    case s_ofire_pot1:
    case s_ofire_pot2:
    case s_tomb1:
    case s_tomb2:
        stateId = StateIdDecoration;
        break;
    case s_force_field_1:
    case s_force_field_2:
    case s_force_field_3:
    case s_force_field_4:
        stateId = StateIdArch;
        break;
    case s_aqua_right:
        stateId = StateIdPeekAlternative;
        break;
    }
    return stateId;
}
const uint16_t SavedGameConverterApocalypse::GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const
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
    case s_oldchestbonus:
    case s_waterchestbonus1:
    case s_rgem1bonus:
    case s_ygem1bonus:
    case s_ggem1bonus:
    case s_bgem1bonus:
    case s_pgem1bonus:
    case s_bonus_die:
    case s_ftimebonus:
    case s_walldie1:
    case s_walldie2:
    case s_portal_wait:
    case s_portal1:
    case s_aqua_under1:
    case s_aqua_left:
    case s_aqua_right:
    case s_aqua_rise1:
    case s_aqua_sink1:
    case s_aqua_walk1:
    case s_aqua_attack1:
    case s_aqua_die1:
    case s_wizard_walk1:
    case s_wizard_attack1:
    case s_wizard_ouch:
    case s_wizard_die1:
    case s_wizard_die4:
    case s_wizard_shoot1:
    case s_wizard_shot1:
    case s_ray_under:
    case s_ray_rise:
    case s_ray_sink:
    case s_ray_fly1:
    case s_ray_attack1:
    case s_ray_die1:
    case s_ray_shot1:
    case s_blob_gnd1:
    case s_blob_rise1:
    case s_blob_sink1:
    case s_blob_walk1:
    case s_blob_ouch:
    case s_blob_die1:
    case s_blob_shot1:
    case s_skel_1:
    case s_skel_attack1:
    case s_skel_ouch:
    case s_skel_die1:
    case s_skel_shot1:
    case s_fmage1:
    case s_fmageattack1:
    case s_fmageouch:
    case s_fmagedie1:
    case s_fmagedie3:
    case s_fmshot1:
    case s_robotank_walk1:
    case s_robotank_attack1:
    case s_robotank_death1:
    case s_robotank_death5:
    case s_robotank_shot1:
    case s_stompy_walk1:
    case s_stompy_attack1:
    case s_stompy_ouch:
    case s_stompy_death1:
    case s_stompy_death4:
    case s_stompy_shot1:
    case s_stompy_shot4:
    case s_bug_walk1:
    case s_bug_attack1:
    case s_bug_ouch:
    case s_bug_death1:
    case s_bug_death2:
    case s_bug_shot1:
    case s_eye_1:
    case s_eye_shootplayer_1:
    case s_eye_ouch:
    case s_eye_die1:
    case s_eye_die4:
    case s_eshot1:
    case s_reye_1:
    case s_reye_ouch:
    case s_reye_die1:
    case s_reye_die4:
    case s_head:
    case s_head_shot1:
    case s_demon1:
    case s_demonattack1:
    case s_demonouch:
    case s_demondie1:
    case s_demondie3:
    case s_troll1:
    case s_trollattack1:
    case s_trollouch:
    case s_trolldie1:
    case s_trolldie3:
    case s_cyborg_demon1:
    case s_cyborg_demonattack1:
    case s_cyborg_demonouch:
    case s_cyborg_demondie1:
    case s_cyborg_demondie3:
    case s_invis_fizz1:
    case s_invis_walk:
    case s_invis_attack:
    case s_invis_flash1:
    case s_invis_death1:
    case s_invis_death3:
    case s_bounce1:
    case s_grel1:
    case s_grelattack3:
    case s_grelouch:
    case s_greldie1:
    case s_greldie6:
    case s_gshot1:
    case s_column1:
    case s_column2:
    case s_column3:
    case s_column4:
    case s_column5:
    case s_ffire_pot:
    case s_ofire_pot1:
    case s_tomb1:
    case s_tomb2:
    case s_force_field_1:
    case s_force_field_die:
    case s_force_field_die1:
    case s_invis_wall_control:
       animationFrame = 0u;
       break;
    case s_pshot2:
    case s_pshot_exp2:
    case s_boltbonus2:
    case s_nukebonus2:
    case s_waterchestbonus2:
    case s_ftimebonus2:
    case s_portal2:
    case s_aqua_under2:
    case s_aqua_rise2:
    case s_aqua_sink2:
    case s_aqua_walk2:
    case s_aqua_attack2:
    case s_aqua_die2:
    case s_wizard_walk2:
    case s_wizard_attack2:
    case s_wizard_die2:
    case s_wizard_shoot2:
    case s_wizard_shoot3:
    case s_wizard_shot2:
    case s_ray_fly2:
    case s_ray_attack2:
    case s_ray_die2:
    case s_ray_shot2:
    case s_blob_gnd2:
    case s_blob_rise2:
    case s_blob_sink2:
    case s_blob_walk2:
    case s_blob_die2:
    case s_blob_shot2:
    case s_skel_2:
    case s_skel_attack2:
    case s_skel_die2:
    case s_skel_shot2:
    case s_fmage2:
    case s_fmageattack2:
    case s_fmagedie2:
    case s_fmshot2:
    case s_robotank_walk2:
    case s_robotank_attack2:
    case s_robotank_death2:
    case s_robotank_shot2:
    case s_stompy_walk2:
    case s_stompy_attack2:
    case s_stompy_death2:
    case s_stompy_shot2:
    case s_stompy_shot5:
    case s_bug_walk2:
    case s_bug_attack2:
    case s_bug_shot2:
    case s_eye_2:
    case s_eye_shootplayer_2:
    case s_eye_pause:
    case s_eye_ouch2:
    case s_eye_die2:
    case s_eshot2:
    case s_reye_2:
    case s_reye_ouch2:
    case s_reye_die2:
    case s_head_shot2:
    case s_demon2:
    case s_demonattack2:
    case s_demondie2:
    case s_troll2:
    case s_trollattack2:
    case s_trollpause:
    case s_trolldie2:
    case s_cyborg_demon2:
    case s_cyborg_demonattack2:
    case s_cyborg_demondie2:
    case s_invis_fizz2:
    case s_invis_pause:
    case s_invis_flash2:
    case s_invis_death2:
    case s_bounce2:
    case s_grelpause:
    case s_grel2:
    case s_greldie2:
    case s_ofire_pot2:
    case s_force_field_2:
    case s_walldie3:
    case s_walldie4:
       animationFrame = 1u;
       break;
    case s_pshot_exp3:
    case s_boltbonus3:
    case s_nukebonus3:
    case s_waterchestbonus3:
    case s_portal3:
    case s_aqua_under3:
    case s_aqua_die3:
    case s_wizard_walk3:
    case s_wizard_die3:
    case s_ray_fly3:
    case s_ray_attack3:
    case s_blob_walk3:
    case s_blob_die3:
    case s_skel_die3:
    case s_skel_3:
    case s_skel_attack3:
    case s_fmage3:
    case s_fmageattack3:
    case s_robotank_walk3:
    case s_robotank_attack3:
    case s_robotank_death3:
    case s_stompy_walk3:
    case s_stompy_attack3:
    case s_stompy_death3:
    case s_stompy_shot3:
    case s_bug_walk3:
    case s_bug_attack3:
    case s_eye_3:
    case s_eye_die3:
    case s_reye_3:
    case s_reye_die3:
    case s_demon3:
    case s_demonattack3:
    case s_demonpause:
    case s_troll3:
    case s_cyborg_demon3:
    case s_cyborg_demonattack3:
    case s_invis_fizz3:
    case s_invis_flash3:
    case s_greldie3:
    case s_force_field_3:
    case s_walldie5:
    case s_walldie6:
        animationFrame = 2u;
        break;
    case s_portal4:
    case s_aqua_die4:
    case s_wizard_walk4:
    case s_ray_fly4:
    case s_skel_4:
    case s_skel_attack4:
    case s_robotank_walk4:
    case s_robotank_attack4:
    case s_robotank_death4:
    case s_stompy_walk4:
    case s_stompy_attack4:
    case s_bug_attack4:
    case s_eye_4:
    case s_reye_4:
    case s_demon4:
    case s_troll4:
    case s_cyborg_demon4:
    case s_greldie4:
    case s_force_field_4:
        animationFrame = 3u;
        break;
    case s_portal5:
    case s_aqua_die5:
    case s_greldie5:
        animationFrame = 4u;
        break;
    case s_portal6:
    case s_aqua_die6:
    case s_greldie5a:
        animationFrame = 5u;
        break;
    case s_aqua_die7:
        animationFrame = 6u;
        break;
    }
    return animationFrame;
}

const bool SavedGameConverterApocalypse::IsInertObject(const uint16_t obclass) const
{
    return obclass == inertobj;
}