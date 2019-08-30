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

//
// EgaGraphCatacomb3D
//
// Data structures required for reading the Catacomb 3-D EGA graphics repository.
//
#pragma once

#include "..\Engine\EgaGraph.h"

const huffmanTable egaDictionaryCatacomb3D =
{
    { 210, 178 },
    { 180, 150 },
    { 214, 218 },
    { 173, 37 },
    { 256, 146 },
    { 73, 74 },
    { 163, 169 },
    { 165, 137 },
    { 154, 181 },
    { 77, 145 },
    { 53, 211 },
    { 45, 91 },
    { 172, 90 },
    { 106, 139 },
    { 141, 257 },
    { 89, 209 },
    { 122, 166 },
    { 258, 38 },
    { 164, 149 },
    { 92, 203 },
    { 212, 161 },
    { 182, 75 },
    { 259, 233 },
    { 133, 157 },
    { 66, 189 },
    { 52, 86 },
    { 260, 174 },
    { 261, 262 },
    { 94, 118 },
    { 26, 155 },
    { 201, 22 },
    { 18, 237 },
    { 263, 44 },
    { 54, 264 },
    { 151, 265 },
    { 185, 50 },
    { 202, 266 },
    { 183, 147 },
    { 148, 46 },
    { 197, 217 },
    { 41, 167 },
    { 36, 267 },
    { 268, 104 },
    { 269, 43 },
    { 270, 100 },
    { 271, 35 },
    { 70, 272 },
    { 72, 221 },
    { 220, 109 },
    { 58, 273 },
    { 88, 194 },
    { 274, 188 },
    { 177, 186 },
    { 275, 82 },
    { 229, 222 },
    { 93, 276 },
    { 277, 278 },
    { 83, 279 },
    { 280, 281 },
    { 282, 33 },
    { 283, 284 },
    { 184, 285 },
    { 215, 286 },
    { 228, 113 },
    { 142, 98 },
    { 287, 288 },
    { 289, 19 },
    { 81, 125 },
    { 290, 69 },
    { 291, 61 },
    { 67, 132 },
    { 292, 293 },
    { 226, 144 },
    { 59, 294 },
    { 295, 296 },
    { 117, 39 },
    { 297, 71 },
    { 205, 298 },
    { 299, 300 },
    { 301, 158 },
    { 236, 20 },
    { 302, 29 },
    { 242, 303 },
    { 130, 304 },
    { 213, 200 },
    { 305, 306 },
    { 190, 136 },
    { 244, 171 },
    { 40, 162 },
    { 208, 235 },
    { 121, 65 },
    { 307, 308 },
    { 123, 176 },
    { 49, 309 },
    { 230, 310 },
    { 17, 311 },
    { 187, 107 },
    { 312, 140 },
    { 153, 313 },
    { 134, 314 },
    { 232, 234 },
    { 315, 175 },
    { 27, 79 },
    { 108, 316 },
    { 9, 138 },
    { 11, 119 },
    { 317, 196 },
    { 318, 238 },
    { 319, 47 },
    { 320, 216 },
    { 103, 321 },
    { 97, 246 },
    { 322, 55 },
    { 323, 324 },
    { 34, 325 },
    { 198, 25 },
    { 245, 326 },
    { 87, 327 },
    { 152, 328 },
    { 23, 329 },
    { 330, 179 },
    { 331, 332 },
    { 333, 156 },
    { 102, 21 },
    { 42, 126 },
    { 84, 334 },
    { 135, 335 },
    { 336, 337 },
    { 110, 206 },
    { 129, 338 },
    { 250, 199 },
    { 339, 14 },
    { 340, 341 },
    { 193, 225 },
    { 342, 30 },
    { 343, 344 },
    { 57, 51 },
    { 345, 120 },
    { 116, 168 },
    { 80, 346 },
    { 114, 347 },
    { 68, 56 },
    { 76, 348 },
    { 124, 349 },
    { 95, 28 },
    { 350, 227 },
    { 351, 101 },
    { 352, 131 },
    { 62, 353 },
    { 204, 354 },
    { 355, 356 },
    { 357, 241 },
    { 358, 359 },
    { 360, 361 },
    { 60, 362 },
    { 363, 364 },
    { 365, 366 },
    { 367, 368 },
    { 160, 195 },
    { 369, 78 },
    { 105, 370 },
    { 371, 99 },
    { 372, 373 },
    { 115, 374 },
    { 375, 12 },
    { 376, 5 },
    { 13, 377 },
    { 378, 379 },
    { 380, 143 },
    { 381, 207 },
    { 382, 383 },
    { 223, 384 },
    { 385, 48 },
    { 386, 387 },
    { 24, 388 },
    { 389, 249 },
    { 10, 390 },
    { 159, 391 },
    { 392, 393 },
    { 243, 6 },
    { 394, 111 },
    { 395, 396 },
    { 397, 398 },
    { 399, 231 },
    { 247, 400 },
    { 401, 402 },
    { 403, 219 },
    { 404, 239 },
    { 405, 406 },
    { 407, 408 },
    { 409, 410 },
    { 191, 411 },
    { 16, 412 },
    { 96, 413 },
    { 414, 415 },
    { 416, 417 },
    { 418, 419 },
    { 420, 2 },
    { 421, 422 },
    { 112, 253 },
    { 423, 424 },
    { 425, 64 },
    { 426, 427 },
    { 428, 429 },
    { 430, 431 },
    { 432, 4 },
    { 433, 434 },
    { 435, 436 },
    { 251, 437 },
    { 438, 439 },
    { 8, 440 },
    { 441, 442 },
    { 443, 444 },
    { 445, 446 },
    { 447, 448 },
    { 449, 450 },
    { 451, 452 },
    { 252, 453 },
    { 63, 454 },
    { 31, 15 },
    { 455, 32 },
    { 240, 248 },
    { 192, 224 },
    { 456, 457 },
    { 458, 127 },
    { 459, 460 },
    { 461, 7 },
    { 462, 463 },
    { 464, 1 },
    { 3, 465 },
    { 466, 467 },
    { 468, 469 },
    { 128, 254 },
    { 470, 85 },
    { 471, 170 },
    { 472, 473 },
    { 474, 475 },
    { 476, 477 },
    { 478, 479 },
    { 480, 481 },
    { 482, 483 },
    { 484, 485 },
    { 486, 487 },
    { 488, 489 },
    { 490, 491 },
    { 492, 493 },
    { 494, 495 },
    { 496, 497 },
    { 498, 499 },
    { 500, 501 },
    { 502, 503 },
    { 504, 505 },
    { 506, 507 },
    { 255, 508 },
    { 0, 509 },
    { 0, 0 }
};

const std::vector<int32_t> egaGraphOffsetsCatacomb3D =
{
    0,  // 0
    379,
    393,
    424,
    1683,
    2632,
    2825,
    3095,
    3288,
    3476,
    3769,
    4120,
    4337,
    4637,
    4940,
    5194,
    5454,
    5709,
    5862,
    5968,
    6315,
    25288,
    40549,
    54890,
    65238,
    75705,
    78631,
    80471,
    81964,
    87247,
    93222,
    98412,
    102832,
    107662,
    112967,
    117766,
    123681,
    123700,
    123880,
    124514,
    125346,
    126231,
    127331,
    128448,
    129560,
    130647,
    131709,
    132805,
    133916,
    135044,
    136142,
    137259,
    138357,
    139440,
    140505,
    141596,
    142695,
    143805,
    150613,
    150628,
    151159,
    151718,
    152259,
    152813,
    153278,
    153778,
    154312,
    154760,
    155135,
    155475,
    156316,
    157120,
    157919,
    158744,
    159562,
    160571,
    161620,
    162577,
    163458,
    164086,
    164646,
    165505,
    166331,
    167145,
    167983,
    168200,
    168416,
    168615,
    168816,
    169051,
    169271,
    169507,
    169739,
    169961,
    170279,
    170788,
    170943,
    171098,
    171390,
    171697,
    172557,  // 100
    173396,
    174247,
    175092,
    176018,
    177060,
    177938,
    178847,
    179855,
    180587,
    181190,
    181946,
    182701,
    183204,
    183943,
    184333,
    184685,
    185057,
    185445,
    185817,
    186184,
    186529,
    186808,
    187879,
    188951,
    189758,
    190544,
    191231,
    191747,
    192292,
    192871,
    193414,
    193846,
    194908,
    194919,
    195705,
    196604,
    197508,
    199037,
    200068,
    201065,
    202547,
    203466,
    204330,
    205496,
    207204,
    207993,
    209322,
    210037,
    210734,
    211339,
    212042,
    212899,
    213768,
    214784,
    215801,
    216658,
    217527,
    218392,
    219264,
    220609,
    223640,
    224858,
    226376,
    226405,
    226437,
    226477,
    227799,
    228084,
    228101,
    228186,
    228293,
    228365,
    228483,
    228552,
    228594,
    228653,
    228761,
    228895,
    228983,
    229113,
    229205,
    229275,
    229362,
    229458,
    229562,
    -1,
    -1,
    229651,
    229752,
    -1,
    -1,
    229834,
    229954,
    -1,
    -1,
    230055,
    230155,
    -1,
    -1,
    230236,  // 200
    230337,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,  // 300
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    230419,
    230466,
    230549,
    230635,
    230707,
    230799,
    230873,
    230944,
    231023,
    231116,
    231199,
    231274,
    231365,
    231434,
    231528,
    231622,
    231714,
    231793,
    231882,
    231965,
    232039,
    232103,
    232192,
    232276,
    232369,
    232461,
    232537,
    232609,
    232656,
    232703,
    232750,
    232797,
    232844,
    232891,
    232938,
    -1,
    -1,
    232989,
    233142,
    233276,
    233425,
    233570,
    233688,
    233756,
    233854,
    233940,
    234049,
    234136,
    234223,
    234378,
    234532,
    234689,
    234848,  // 400
    234998,
    235155,
    235305,
    235465,
    235626,
    235721,
    235834,
    235941,
    236095,
    236143,
    236242,
    236319,
    236462,
    236577,
    236691,
    236844,
    236999,
    237149,
    -1,
    237305,
    237432,
    237551,
    237619,
    237719,
    237812,
    237938,
    238058,
    238126,
    238228,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    238321,  // 456
    238704,
    239073,
    239582,
    240062,
    240569,
    241051,
    241540,
    242067,
    242552,
    242994,
    243486,
    243998,
    244530,
    245043,
    245523,
    246031,
    246533,
    247049,
    247569,
    248166,
    252502, // 477
    256899  // EOF
};

typedef enum {
    CP_MAINMENUPIC = 5,
    CP_NEWGAMEMENUPIC,           // 6
    CP_LOADMENUPIC,              // 7
    CP_SAVEMENUPIC,              // 8
    CP_CONFIGMENUPIC,            // 9
    CP_SOUNDMENUPIC,             // 10
    CP_MUSICMENUPIC,             // 11
    CP_KEYBOARDMENUPIC,          // 12
    CP_KEYMOVEMENTPIC,           // 13
    CP_KEYBUTTONPIC,             // 14
    CP_JOYSTICKMENUPIC,          // 15
    CP_OPTIONSMENUPIC,           // 16
    CP_PADDLEWARPIC,             // 17
    CP_QUITPIC,                  // 18
    CP_JOYSTICKPIC,              // 19
    CP_MENUSCREENPIC,            // 20
    TITLEPIC,                    // 21
    CREDITSPIC,                  // 22
    HIGHSCORESPIC,               // 23
    FINALEPIC,                   // 24
    STATUSPIC,                   // 25
    SIDEBARSPIC,                 // 26
    SCROLLTOPPIC,                // 27
    SCROLL1PIC,                  // 28
    SCROLL2PIC,                  // 29
    SCROLL3PIC,                  // 30
    SCROLL4PIC,                  // 31
    SCROLL5PIC,                  // 32
    SCROLL6PIC,                  // 33
    SCROLL7PIC,                  // 34
    SCROLL8PIC,                  // 35
    FIRSTLATCHPIC,               // 36
    NOSHOTPOWERPIC,              // 37
    SHOTPOWERPIC,                // 38
    NOBODYPIC,                   // 39
    BODYPIC,                     // 40
    COMPAS1PIC,                  // 41
    COMPAS2PIC,                  // 42
    COMPAS3PIC,                  // 43
    COMPAS4PIC,                  // 44
    COMPAS5PIC,                  // 45
    COMPAS6PIC,                  // 46
    COMPAS7PIC,                  // 47
    COMPAS8PIC,                  // 48
    COMPAS9PIC,                  // 49
    COMPAS10PIC,                 // 50
    COMPAS11PIC,                 // 51
    COMPAS12PIC,                 // 52
    COMPAS13PIC,                 // 53
    COMPAS14PIC,                 // 54
    COMPAS15PIC,                 // 55
    COMPAS16PIC,                 // 56
    DEADPIC,                     // 57
    FIRSTSCALEPIC,               // 58
    ORC1PIC,                     // 59
    ORC2PIC,                     // 60
    ORC3PIC,                     // 61
    ORC4PIC,                     // 62
    ORCATTACK1PIC,               // 63
    ORCATTACK2PIC,               // 64
    ORCOUCHPIC,                  // 65
    ORCDIE1PIC,                  // 66
    ORCDIE2PIC,                  // 67
    ORCDIE3PIC,                  // 68
    TROLL1PIC,                   // 69
    TROLL2PIC,                   // 70
    TROLL3PIC,                   // 71
    TROLL4PIC,                   // 72
    TROLLOUCHPIC,                // 73
    TROLLATTACK1PIC,             // 74
    TROLLATTACK2PIC,             // 75
    TROLLATTACK3PIC,             // 76
    TROLLDIE1PIC,                // 77
    TROLLDIE2PIC,                // 78
    TROLLDIE3PIC,                // 79
    WARP1PIC,                    // 80
    WARP2PIC,                    // 81
    WARP3PIC,                    // 82
    WARP4PIC,                    // 83
    BOLTOBJPIC,                  // 84
    BOLTOBJ2PIC,                 // 85
    NUKEOBJPIC,                  // 86
    NUKEOBJ2PIC,                 // 87
    POTIONOBJPIC,                // 88
    RKEYOBJPIC,                  // 89
    YKEYOBJPIC,                  // 90
    GKEYOBJPIC,                  // 91
    BKEYOBJPIC,                  // 92
    SCROLLOBJPIC,                // 93
    CHESTOBJPIC,                 // 94
    PSHOT1PIC,                   // 95
    PSHOT2PIC,                   // 96
    BIGPSHOT1PIC,                // 97
    BIGPSHOT2PIC,                // 98
    DEMON1PIC,                   // 99
    DEMON2PIC,                   // 100
    DEMON3PIC,                   // 101
    DEMON4PIC,                   // 102
    DEMONATTACK1PIC,             // 103
    DEMONATTACK2PIC,             // 104
    DEMONATTACK3PIC,             // 105
    DEMONOUCHPIC,                // 106
    DEMONDIE1PIC,                // 107
    DEMONDIE2PIC,                // 108
    DEMONDIE3PIC,                // 109
    MAGE1PIC,                    // 110
    MAGE2PIC,                    // 111
    MAGEOUCHPIC,                 // 112
    MAGEATTACKPIC,               // 113
    MAGEDIE1PIC,                 // 114
    MAGEDIE2PIC,                 // 115
    BAT1PIC,                     // 116
    BAT2PIC,                     // 117
    BAT3PIC,                     // 118
    BAT4PIC,                     // 119
    BATDIE1PIC,                  // 120
    BATDIE2PIC,                  // 121
    GREL1PIC,                    // 122
    GREL2PIC,                    // 123
    GRELATTACKPIC,               // 124
    GRELHITPIC,                  // 125
    GRELDIE1PIC,                 // 126
    GRELDIE2PIC,                 // 127
    GRELDIE3PIC,                 // 128
    GRELDIE4PIC,                 // 129
    GRELDIE5PIC,                 // 130
    GRELDIE6PIC,                 // 131
    NEMESISPIC,                  // 132
    FIRSTWALLPIC,                // 133
    EXPWALL1PIC,                 // 134
    EXPWALL2PIC,                 // 135
    EXPWALL3PIC,                 // 136
    WALL1LPIC,                   // 137
    WALL1DPIC,                   // 138
    WALL2DPIC,                   // 139
    WALL2LPIC,                   // 140
    WALL3DPIC,                   // 141
    WALL3LPIC,                   // 142
    WALL4DPIC,                   // 143
    WALL4LPIC,                   // 144
    WALL5DPIC,                   // 145
    WALL5LPIC,                   // 146
    WALL6DPIC,                   // 147
    WALL6LPIC,                   // 148
    WALL7DPIC,                   // 149
    WALL7LPIC,                   // 150
    RDOOR1PIC,                   // 151
    RDOOR2PIC,                   // 152
    YDOOR1PIC,                   // 153
    YDOOR2PIC,                   // 154
    GDOOR1PIC,                   // 155
    GDOOR2PIC,                   // 156
    BDOOR1PIC,                   // 157
    BDOOR2PIC,                   // 158
    ENTERPLAQUEPIC,              // 159

    CP_MENUMASKPICM = 160,
    HAND1PICM,                   // 161
    HAND2PICM,                   // 162

    // Lump Start
    PADDLESPR = 163,
    BALLSPR,                     // 164
    BALL1PIXELTOTHERIGHTSPR,     // 165

    LEVEL1TEXT = 456,
    LEVEL2TEXT,                  // 457
    LEVEL3TEXT,                  // 458
    LEVEL4TEXT,                  // 459
    LEVEL5TEXT,                  // 460
    LEVEL6TEXT,                  // 461
    LEVEL7TEXT,                  // 462
    LEVEL8TEXT,                  // 463
    LEVEL9TEXT,                  // 464
    LEVEL10TEXT,                 // 465
    LEVEL11TEXT,                 // 466
    LEVEL12TEXT,                 // 467
    LEVEL13TEXT,                 // 468
    LEVEL14TEXT,                 // 469
    LEVEL15TEXT,                 // 470
    LEVEL16TEXT,                 // 471
    LEVEL17TEXT,                 // 472
    LEVEL18TEXT,                 // 473
    LEVEL19TEXT,                 // 474
    LEVEL20TEXT,                 // 475
    OUTOFMEM,                    // 476
    PIRACY,                      // 477
    ENUMEND
} egaGraphicsCatacomb3D;

const egaGraphStaticData egaGraphCatacomb3D =
{
    "EGAGRAPH.C3D",
    egaGraphOffsetsCatacomb3D,
    egaDictionaryCatacomb3D,
    FINALEPIC,
    FIRSTSCALEPIC,
    FIRSTWALLPIC,
    HAND1PICM,
    PADDLESPR,
    CP_MENUMASKPICM,
    LEVEL1TEXT,
    LEVEL20TEXT
};