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

//
// EgaGraphApocalypse
//
// Data structures required for reading the Catacomb Apocalypse EGA graphics repository.
//
#pragma once

#include "..\Engine\EgaGraph.h"

const huffmanTable egaDictionaryApocalypse =
{
    { 90, 150 },
    { 154, 210 },
    { 178, 75 },
    { 218, 89 },
    { 53, 166 },
    { 181, 157 },
    { 214, 106 },
    { 173, 133 },
    { 212, 172 },
    { 141, 165 },
    { 164, 180 },
    { 45, 149 },
    { 162, 182 },
    { 93, 138 },
    { 82, 139 },
    { 86, 137 },
    { 229, 37 },
    { 177, 118 },
    { 186, 94 },
    { 50, 151 },
    { 202, 42 },
    { 107, 185 },
    { 73, 69 },
    { 43, 83 },
    { 169, 81 },
    { 147, 70 },
    { 163, 41 },
    { 148, 161 },
    { 174, 256 },
    { 145, 91 },
    { 78, 257 },
    { 76, 258 },
    { 77, 201 },
    { 155, 211 },
    { 259, 233 },
    { 260, 38 },
    { 74, 153 },
    { 52, 117 },
    { 209, 146 },
    { 261, 98 },
    { 262, 263 },
    { 54, 84 },
    { 264, 265 },
    { 158, 167 },
    { 266, 213 },
    { 267, 26 },
    { 92, 268 },
    { 269, 205 },
    { 179, 87 },
    { 88, 171 },
    { 22, 270 },
    { 221, 271 },
    { 234, 215 },
    { 109, 217 },
    { 44, 203 },
    { 272, 273 },
    { 61, 100 },
    { 274, 58 },
    { 183, 275 },
    { 46, 66 },
    { 197, 276 },
    { 105, 142 },
    { 34, 277 },
    { 194, 184 },
    { 278, 279 },
    { 280, 110 },
    { 281, 57 },
    { 282, 71 },
    { 226, 235 },
    { 113, 283 },
    { 284, 189 },
    { 285, 49 },
    { 237, 121 },
    { 286, 168 },
    { 21, 287 },
    { 18, 35 },
    { 67, 288 },
    { 55, 187 },
    { 289, 236 },
    { 198, 290 },
    { 79, 59 },
    { 230, 245 },
    { 108, 291 },
    { 222, 292 },
    { 114, 293 },
    { 294, 134 },
    { 295, 29 },
    { 36, 242 },
    { 296, 68 },
    { 196, 297 },
    { 219, 298 },
    { 299, 300 },
    { 19, 301 },
    { 302, 51 },
    { 303, 102 },
    { 304, 305 },
    { 72, 200 },
    { 306, 156 },
    { 307, 25 },
    { 27, 116 },
    { 175, 39 },
    { 308, 176 },
    { 309, 216 },
    { 206, 33 },
    { 103, 310 },
    { 104, 244 },
    { 220, 311 },
    { 130, 312 },
    { 313, 65 },
    { 314, 123 },
    { 20, 228 },
    { 315, 115 },
    { 232, 97 },
    { 316, 317 },
    { 318, 47 },
    { 319, 17 },
    { 144, 238 },
    { 320, 190 },
    { 208, 321 },
    { 188, 101 },
    { 322, 80 },
    { 323, 324 },
    { 325, 250 },
    { 125, 326 },
    { 119, 111 },
    { 327, 328 },
    { 204, 329 },
    { 330, 246 },
    { 152, 331 },
    { 332, 195 },
    { 40, 136 },
    { 333, 334 },
    { 335, 225 },
    { 336, 337 },
    { 9, 23 },
    { 338, 11 },
    { 30, 339 },
    { 340, 341 },
    { 95, 342 },
    { 343, 344 },
    { 345, 346 },
    { 347, 348 },
    { 349, 350 },
    { 351, 13 },
    { 352, 353 },
    { 160, 122 },
    { 354, 355 },
    { 120, 241 },
    { 356, 357 },
    { 358, 359 },
    { 360, 361 },
    { 362, 131 },
    { 60, 363 },
    { 124, 135 },
    { 364, 365 },
    { 366, 367 },
    { 368, 199 },
    { 369, 227 },
    { 85, 170 },
    { 370, 129 },
    { 371, 10 },
    { 372, 126 },
    { 373, 143 },
    { 14, 374 },
    { 375, 193 },
    { 62, 376 },
    { 377, 378 },
    { 5, 379 },
    { 380, 381 },
    { 56, 382 },
    { 383, 384 },
    { 385, 159 },
    { 386, 387 },
    { 28, 388 },
    { 389, 207 },
    { 390, 391 },
    { 392, 393 },
    { 394, 395 },
    { 396, 112 },
    { 397, 231 },
    { 398, 399 },
    { 249, 400 },
    { 401, 48 },
    { 402, 96 },
    { 403, 404 },
    { 239, 405 },
    { 12, 406 },
    { 243, 407 },
    { 140, 408 },
    { 6, 409 },
    { 410, 99 },
    { 411, 412 },
    { 413, 251 },
    { 414, 415 },
    { 416, 417 },
    { 132, 418 },
    { 24, 419 },
    { 420, 421 },
    { 422, 423 },
    { 424, 425 },
    { 426, 427 },
    { 428, 429 },
    { 430, 431 },
    { 432, 191 },
    { 433, 434 },
    { 223, 435 },
    { 253, 436 },
    { 437, 4 },
    { 438, 439 },
    { 440, 240 },
    { 441, 442 },
    { 443, 444 },
    { 445, 2 },
    { 446, 447 },
    { 448, 449 },
    { 15, 450 },
    { 64, 451 },
    { 31, 248 },
    { 452, 453 },
    { 224, 454 },
    { 7, 455 },
    { 32, 247 },
    { 456, 8 },
    { 63, 457 },
    { 252, 458 },
    { 16, 192 },
    { 459, 460 },
    { 461, 3 },
    { 462, 463 },
    { 464, 465 },
    { 466, 467 },
    { 468, 469 },
    { 254, 470 },
    { 471, 127 },
    { 472, 473 },
    { 474, 1 },
    { 475, 128 },
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
    { 506, 255 },
    { 507, 0 },
    { 508, 509 },
    { 0, 0 }
};

const std::vector<int32_t> egaGraphOffsetsApocalypse =
{
    0,
    816,
    823,
    840,
    2098,
    17810,
    23881,
    24495,
    24508,
    25244,
    25989,
    26693,
    27342,
    27984,
    28381,
    28436,
    28491,
    28541,
    28593,
    28643,
    28656,
    28669,
    28947,
    29321,
    29706,
    30069,
    30454,
    31098,
    32041,
    33054,
    34138,
    35123,
    36162,
    37247,
    38402,
    39693,
    40460,
    40730,
    41142,
    41556,
    42040,
    42654,
    43301,
    43543,
    44190,
    44846,
    45574,
    46336,
    47027,
    47396,
    47653,
    47922,
    48181,
    48449,
    48664,
    48879,
    49098,
    49327,
    49551,
    50179,
    50466,
    50738,
    51220,
    51762,
    52221,
    53612,
    53905,
    54198,
    54487,
    54778,
    55076,
    55365,
    55787,
    56209,
    56754,
    57310,
    58085,
    58713,
    59281,
    59841,
    60623,
    61411,
    62211,
    62925,
    63488,
    64077,
    64701,
    65433,
    65741,
    66051,
    66360,
    66650,
    66929,
    67492,
    68051,
    68563,
    68935,
    69222,
    69494,
    70493,
    71549,
    72321,
    73093,
    73858,
    74788,
    75643,
    76154,
    76612,
    77043,
    78320,
    79572,
    80803,
    82111,
    83350,
    84520,
    85720,
    86911,
    87981,
    89230,
    90469,
    91930,
    93386,
    94563,
    95789,
    96646,
    97478,
    98328,
    99171,
    100013,
    100855,
    101689,
    102642,
    103727,
    104239,
    104490,
    104743,
    105803,
    106854,
    107914,
    108967,
    110013,
    111044,
    112156,
    113435,
    114387,
    115084,
    115619,
    116164,
    116328,
    116479,
    117349,
    118213,
    119063,
    119933,
    120793,
    121876,
    122989,
    123945,
    124686,
    125383,
    125995,
    126628,
    127253,
    127887,
    128264,
    128444,
    128626,
    129177,
    129780,
    130413,
    130965,
    131530,
    132056,
    133304,
    134625,
    135813,
    136899,
    137696,
    139022,
    140049,
    141043,
    142053,
    143005,
    143913,
    144905,
    145587,
    145774,
    145958,
    147324,
    148623,
    149989,
    151296,
    152722,
    154300,
    155686,
    156928,
    158091,
    158887,
    160033,
    161185,
    161939,
    162750,
    163483,
    164026,
    164639,
    165352,
    165981,
    166460,
    166771,
    167534,
    168335,
    169144,
    169400,
    169657,
    170315,
    171069,
    171895,
    172657,
    173448,
    173818,
    174207,
    174563,
    174937,
    175322,
    175682,
    176756,
    177765,
    178815,
    179840,
    180967,
    182230,
    183314,
    184398,
    185629,
    186537,
    187228,
    187241,
    188651,
    189982,
    191157,
    192083,
    192918,
    194145,
    195369,
    196209,
    196962,
    198048,
    198981,
    199908,
    200769,
    201680,
    202575,
    204126,
    204978,
    205773,
    207112,
    208191,
    209186,
    210073,
    211260,
    212713,
    214006,
    215545,
    216615,
    218006,
    219149,
    220595,
    221606,
    222890,
    223969,
    225402,
    226837,
    228045,
    229349,
    230516,
    231789,
    232887,
    233989,
    234976,
    236076,
    237129,
    238455,
    239766,
    241646,
    243404,
    245261,
    247005,
    248872,
    250622,
    252487,
    254214,
    255841,
    257178,
    258697,
    259997,
    261200,
    262428,
    263474,
    264484,
    265720,
    266819,
    267917,
    268857,
    270390,
    271826,
    273278,
    274662,
    276075,
    277440,
    278713,
    279744,
    281369,
    282942,
    284062,
    285076,
    286127,
    287043,
    288165,
    289238,
    290619,
    291816,
    293170,
    294459,
    295402,
    296345,
    297304,
    298153,
    298991,
    299847,
    300851,
    301857,
    302879,
    303823,
    304759,
    305716,
    306869,
    307981,
    309099,
    310231,
    311396,
    312399,
    313348,
    314438,
    315563,
    316702,
    317874,
    319069,
    320519,
    321666,
    323101,
    323937,
    324713,
    326121,
    327247,
    328073,
    329003,
    329977,
    330931,
    331955,
    332936,
    334304,
    335454,
    336932,
    338346,
    339675,
    340933,
    342452,
    343925,
    345754,
    347209,
    348812,
    348842,
    350478,
    350508,
    352137,
    352437,
    352470,
    352526,
    352582,
    352692,
    352818,
    352904,
    352988,
    353073,
    353154,
    353261,
    353370,
    353473,
    353555,
    353665,
    353775,
    353852,
    353956,
    354076,
    354143,
    354218,
    354327,
    354406,
    354506,
    354611,
    354678,
    354746,
    354806,
    354878,
    354984,
    355089,
    355209,
    355319,
    355375,
    355431,
    355524,
    355640,
    355708,
    355776,
    355846,
    355914,
    356032,
    356138,
    356277,
    356397,
    356520,
    356576,
    356687,
    356794,
    356909,
    357017,
    357144,
    357259,
    357315,
    357371,
    357487,
    357588,
    357721,
    357850,
    357965,
    358054,
    358138,
    358221,
    358322,
    358418,
    358513,
    358598,
    358681,
    358737,
    358852,
    358952,
    359087,
    359198,
    359254,
    359343,
    359427,
    359517,
    359608,
    359693,
    359776,
    359863,
    359919,
    360018,
    360108,
    360198,
    360315,
    360425,
    360536,
    360652,
    360708,
    360764,
    360820,
    360876,
    360932,
    360988,
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
    361087,
    361143,
    361224,
    361307,
    361384,
    361475,
    361553,
    361629,
    361711,
    361804,
    361886,
    361963,
    362053,
    362128,
    362221,
    362314,
    362405,
    362487,
    362572,
    362654,
    362729,
    362799,
    362882,
    362963,
    363053,
    363141,
    363217,
    363293,
    363349,
    363405,
    363461,
    363517,
    363573,
    363629,
    363685,
    -1,
    -1,
    363745,
    363886,
    364010,
    364149,
    364281,
    364394,
    364506,
    364617,
    364719,
    364825,
    364924,
    365030,
    365137,
    365207,
    365346,
    365487,
    365637,
    365768,
    365887,
    366029,
    366139,
    366247,
    366365,
    366477,
    366597,
    366713,
    366837,
    366987,
    367095,
    367235,
    367363,
    367465,
    367578,
    367619,
    367660,
    367701,
    367828,
    367967,
    368077,
    368176,
    368297,
    368415,
    368537,
    368670,
    368801,
    368928,
    369041,
    369189,
    369292,
    369366,
    369495,
    369634,
    369755,
    369890,
    369931,
    369972,
    370013,
    370054,
    370095,
    370136,
    370177,
    370218,
    370259,
    370300,
    370341,
    370382,
    370423,
    370464,
    370505,
    370546,
    370587,
    370628,
    371043,
    371520,
    371770,
    372359,
    372816,
    373317,
    373599,
    373937,
    374414,
    374778,
    375205,
    375512,
    376209,
    376369,
    377039,
    377783,
    377881,
    377979,
    382078
 };

typedef enum {
    FINALEPIC = 4,
    STATUSPIC,                   // 5
    FACE5PIC,                    // 6
    FIRSTLATCHPIC,               // 7
    FACE1PIC,                    // 8
    FACE2PIC,                    // 9
    FACE3PIC,                    // 10
    FACE4PIC,                    // 11
    RADAR_TOPPIC,                // 12
    RADAR_BOTTOMPIC,             // 13
    RADAR_RGEMPIC,               // 14
    RADAR_GGEMPIC,               // 15
    RADAR_BGEMPIC,               // 16
    RADAR_YGEMPIC,               // 17
    RADAR_PGEMPIC,               // 18
    FIRSTGROUNDPIC,              // 19
    FIRSTSCALEPIC,               // 20
    // Lump Start
    EYESTALKUNDER1PIC,           // 21
    EYESTALKUNDER2PIC,           // 22
    EYESTALKUNDER3PIC,           // 23
    EYESTALKUNDER4PIC,           // 24
    EYESTALKUNDER5PIC,           // 25
    EYESTALKRISE1PIC,            // 26
    EYESTALKRISE2PIC,            // 27
    EYESTALKWALK1PIC,            // 28
    EYESTALKWALK2PIC,            // 29
    EYESTALKATTACKPIC,           // 30
    EYESTALKDEATH1PIC,           // 31
    EYESTALKDEATH2PIC,           // 32
    EYESTALKDEATH3PIC,           // 33
    EYESTALKDEATH4PIC,           // 34
    EYESTALKDEATH5PIC,           // 35
    // Lump Start
    BLOB_SHOT1PIC,               // 36
    BLOBGND1PIC,                 // 37
    BLOBGND2PIC,                 // 38
    BLOBRISE1PIC,                // 39
    BLOBRISE2PIC,                // 40
    BLOBWALK1PIC,                // 41
    BLOB_SHOT2PIC,               // 42
    BLOBWALK2PIC,                // 43
    BLOBWALK3PIC,                // 44
    BLOBDEATH1PIC,               // 45
    BLOBDEATH2PIC,               // 46
    BLOBDEATH3PIC,               // 47
    // Lump Start
    POTIONOBJPIC,                // 48
    // Lump Start
    RKEYOBJPIC,                  // 49
    // Lump Start
    YKEYOBJPIC,                  // 50
    // Lump Start
    GKEYOBJPIC,                  // 51
    // Lump Start
    BKEYOBJPIC,                  // 52
    // Lump Start
    RGEM1PIC,                    // 53
    // Lump Start
    GGEM1PIC,                    // 54
    // Lump Start
    BGEM1PIC,                    // 55
    // Lump Start
    YGEM1PIC,                    // 56
    // Lump Start
    PGEM1PIC,                    // 57
    // Lump Start
    CHESTOBJPIC,                 // 58
    // Lump Start
    PSHOT1PIC,                   // 59
    PSHOT2PIC,                   // 60
    PSHOT_EXP1PIC,               // 61
    PSHOT_EXP2PIC,               // 62
    PSHOT_EXP3PIC,               // 63
    // Lump Start
    HEADPIC,                     // 64
    // Lump Start
    BOLTOBJPIC,                  // 65
    BOLT2OBJPIC,                 // 66
    BOLT3OBJPIC,                 // 67
    // Lump Start
    NUKEOBJPIC,                  // 68
    NUKE2OBJPIC,                 // 69
    NUKE3OBJPIC,                 // 70
    // Lump Start
    TIMEOBJ1PIC,                 // 71
    TIMEOBJ2PIC,                 // 72
    // Lump Start
    COLUMN5PIC,                  // 73
    // Lump Start
    COLUMN1PIC,                  // 74
    // Lump Start
    FFIRE_POTPIC,                // 75
    // Lump Start
    COLUMN2PIC,                  // 76
    // Lump Start
    COLUMN3PIC,                  // 77
    // Lump Start
    COLUMN4PIC,                  // 78
    // Lump Start
    OFIRE_POT1PIC,               // 79
    OFIRE_POT2PIC,               // 80
    // Lump Start
    TOMB1PIC,                    // 81
    // Lump Start
    TOMB2PIC,                    // 82
    // Lump Start
    O_WATER_CHEST1PIC,           // 83
    O_WATER_CHEST2PIC,           // 84
    O_WATER_CHEST3PIC,           // 85
    // Lump Start
    OLD_CHESTPIC,                // 86
    // Lump Start
    EYE_WALK1PIC,                // 87
    EYE_WALK2PIC,                // 88
    EYE_WALK3PIC,                // 89
    EYE_OUCH1PIC,                // 90
    EYE_OUCH2PIC,                // 91
    EYE_DEATH1PIC,               // 92
    EYE_DEATH2PIC,               // 93
    EYE_DEATH3PIC,               // 94
    EYE_DEATH4PIC,               // 95
    EYE_SHOT1PIC,                // 96
    EYE_SHOT2PIC,                // 97
    // Lump Start
    FMAGEWALK1PIC,               // 98
    FMAGEWALK2PIC,               // 99
    FMAGEWALK3PIC,               // 100
    FMAGEATTACK1PIC,             // 101
    FMAGEATTACK2PIC,             // 102
    FMAGEDEATH1PIC,              // 103
    FMAGEDEATH2PIC,              // 104
    FMAGEDEATH3PIC,              // 105
    FMAGESHOT1PIC,               // 106
    FMAGESHOT2PIC,               // 107
    // Lump Start
    FORCE_FIELD_1PIC,            // 108
    FORCE_FIELD_2PIC,            // 109
    FORCE_FIELD_3PIC,            // 110
    FORCE_FIELD_4PIC,            // 111
    // Lump Start
    ROBOTANKWALK1PIC,            // 112
    ROBOTANKWALK2PIC,            // 113
    ROBOTANKWALK3PIC,            // 114
    ROBOTANKWALK4PIC,            // 115
    ROBOTANKOUCHPIC,             // 116
    ROBOTANKATTACK1PIC,          // 117
    ROBOTANKATTACK2PIC,          // 118
    ROBOTANKDEATH1PIC,           // 119
    ROBOTANKDEATH2PIC,           // 120
    ROBOTANKDEATH3PIC,           // 121
    ROBOTANKDEATH4PIC,           // 122
    // Lump Start
    RAMBONEWALK1PIC,             // 123
    RAMBONEWALK2PIC,             // 124
    RAMBONEWALK3PIC,             // 125
    RAMBONEWALK4PIC,             // 126
    RAMBONEATTACK1PIC,           // 127
    RAMBONEATTACK2PIC,           // 128
    RAMBONEATTACK3PIC,           // 129
    RAMBONEDEATH1PIC,            // 130
    RAMBONEDEATH2PIC,            // 131
    RAMBONEDEATH3PIC,            // 132
    RAMBONESHOT1PIC,             // 133
    RAMBONESHOT2PIC,             // 134
    // Lump Start
    STOMPYWALK1PIC,              // 135
    STOMPYWALK2PIC,              // 136
    STOMPYWALK3PIC,              // 137
    STOMPYWALK4PIC,              // 138
    STOMPYATTACK1PIC,            // 139
    STOMPYATTACK2PIC,            // 140
    STOMPYDEATH1PIC,             // 141
    STOMPYDEATH2PIC,             // 142
    STOMPYDEATH3PIC,             // 143
    STOMPYDEATH4PIC,             // 144
    STOMPYSHOT1PIC,              // 145
    STOMPYSHOT2PIC,              // 146
    STOMPYSHOT3PIC,              // 147
    STOMPYSHOT4PIC,              // 148
    // Lump Start
    TROLL1PIC,                   // 149
    TROLL2PIC,                   // 150
    TROLL3PIC,                   // 151
    TROLL4PIC,                   // 152
    TROLLOUCHPIC,                // 153
    TROLLATTACK1PIC,             // 154
    TROLLATTACK2PIC,             // 155
    TROLLDIE1PIC,                // 156
    TROLLDIE2PIC,                // 157
    TROLLDIE3PIC,                // 158
    // Lump Start
    WIZARDWALK1PIC,              // 159
    WIZARDWALK2PIC,              // 160
    WIZARDWALK3PIC,              // 161
    WIZARDWALK4PIC,              // 162
    WIZARDDEATH4PIC,             // 163
    WIZARD_SHOT1PIC,             // 164
    WIZARD_SHOT2PIC,             // 165
    WIZARDATTACK1PIC,            // 166
    WIZARDATTACK2PIC,            // 167
    WIZARDOUCHPIC,               // 168
    WIZARDDEATH1PIC,             // 169
    WIZARDDEATH2PIC,             // 170
    WIZARDDEATH3PIC,             // 171
    // Lump Start
    INVIS_FIZZ1PIC,              // 172
    INVIS_FIZZ2PIC,              // 173
    INVIS_DEATH1PIC,             // 174
    INVIS_DEATH2PIC,             // 175
    INVIS_DEATH3PIC,             // 176
    INVIS_FIZZ3PIC,              // 177
    // Lump Start
    BUG_WALK1PIC,                // 178
    BUG_WALK2PIC,                // 179
    BUG_WALK3PIC,                // 180
    BUG_ATTACK1PIC,              // 181
    BUG_ATTACK2PIC,              // 182
    BUG_DEATH1PIC,               // 183
    BUG_DEATH2PIC,               // 184
    BUG_SHOT1PIC,                // 185
    BUG_SHOT2PIC,                // 186
    // Lump Start
    CYBORG1PIC,                  // 187
    CYBORG2PIC,                  // 188
    CYBORG3PIC,                  // 189
    CYBORG4PIC,                  // 190
    CYBORGATTACK1PIC,            // 191
    CYBORGATTACK2PIC,            // 192
    CYBORGATTACK3PIC,            // 193
    CYBORGOUCHPIC,               // 194
    CYBORGDIE1PIC,               // 195
    CYBORGDIE2PIC,               // 196
    // Lump Start
    GREL1PIC,                    // 197
    GREL2PIC,                    // 198
    GRELATTACKPIC,               // 199
    GRELHITPIC,                  // 200
    GRELDIE1PIC,                 // 201
    GRELDIE2PIC,                 // 202
    GRELDIE3PIC,                 // 203
    GRELDIE4PIC,                 // 204
    GRELDIE5PIC,                 // 205
    GRELDIE6PIC,                 // 206
    SKULL_SHOTPIC,               // 207
    // Lump Start
    RAYFLY1PIC,                  // 208
    RAYFLY2PIC,                  // 209
    RAYFLY3PIC,                  // 210
    RAYSHOT1PIC,                 // 211
    RAYSHOT2PIC,                 // 212
    RAYSHOOT1PIC,                // 213
    RAYSHOOT2PIC,                // 214
    RAYDEATH1PIC,                // 215
    RAYDEATH2PIC,                // 216
    RAYRISEPIC,                  // 217
    // Lump Start
    PORTAL1PIC,                  // 218
    PORTAL2PIC,                  // 219
    PORTAL3PIC,                  // 220
    PORTAL4PIC,                  // 221
    PORTAL5PIC,                  // 222
    PORTAL6PIC,                  // 223
    // Lump Start
    DEMON1PIC,                   // 224
    DEMON2PIC,                   // 225
    DEMON3PIC,                   // 226
    DEMON4PIC,                   // 227
    DEMONATTACK1PIC,             // 228
    DEMONATTACK2PIC,             // 229
    DEMONATTACK3PIC,             // 230
    DEMONOUCHPIC,                // 231
    DEMONDIE1PIC,                // 232
    DEMONDIE2PIC,                // 233
    DEMONDIE3PIC,                // 234
    FIRSTWALLPIC,                // 235
    NEMESISPIC,                  // 236
    ALTARLEFTPIC,                // 237
    ALTARRIGHTPIC,               // 238
    TEMPLEWALLLIGHTPIC,          // 239
    TEMPLEWALLDARKPIC,           // 240
    TORCHWALL1PIC,               // 241
    TORCHWALL2PIC,               // 242
    BRNBRKLIGHTPIC,              // 243
    BRNBRKDARKPIC,               // 244
    BRNBRKEMLIGHTPIC,            // 245
    BRNBRKEMDARKPIC,             // 246
    FIREWALL1PIC,                // 247
    FIREWALL2PIC,                // 248
    FIREWALL3PIC,                // 249
    FIREWALL4PIC,                // 250
    IRONGATEPIC,                 // 251
    BRNFLGLIGHTPIC,              // 252
    BRNFLGDARKPIC,               // 253
    BRNFLGVINELIGHTPIC,          // 254
    BRNFLGVINEDARKPIC,           // 255
    BRNFLGDMGLIGHTPIC,           // 256
    BRNFLGDMGDARKPIC,            // 257
    SPACEDMG1LIGHTPIC,           // 258
    SPACEDMG1DARKPIC,            // 259
    SPACEDMG2LIGHTPIC,           // 260
    SPACEDMG2DARKPIC,            // 261
    SPACE1LIGHTPIC,              // 262
    SPACE1DARKPIC,               // 263
    SPACE2LIGHTPIC,              // 264
    SPACE2DARKPIC,               // 265
    SPACE3LIGHTPIC,              // 266
    SPACE3DARKPIC,               // 267
    SPACE4LIGHTPIC,              // 268
    SPACE4DARKPIC,               // 269
    SPACE5LIGHTPIC,              // 270
    SPACE5DARKPIC,               // 271
    SPACE6LIGHTPIC,              // 272
    SPACE6DARKPIC,               // 273
    SPACE7LIGHTPIC,              // 274
    SPACE7DARKPIC,               // 275
    SPACE8LIGHTPIC,              // 276
    SPACE8DARKPIC,               // 277
    SPACE10LIGHTPIC,             // 278
    SPACE10DARKPIC,              // 279
    RUSTDOORLIGHTPIC,            // 280
    RUSTDOORDARKPIC,             // 281
    SPACE11LIGHTPIC,             // 282
    SPACE11DARKPIC,              // 283
    SPACE12LIGHTPIC,             // 284
    SPACE12DARKPIC,              // 285
    SPACE13LIGHTPIC,             // 286
    SPACE13DARKPIC,              // 287
    SPACE14LIGHTPIC,             // 288
    SPACE14DARKPIC,              // 289
    SPACEDMG5LIGHTPIC,           // 290
    SPACEDMG5DARKPIC,            // 291
    SPACEDMG6LIGHTPIC,           // 292
    SPACEDMG6DARKPIC,            // 293
    SPCDOOR1LIGHTPIC,            // 294
    SPCDOOR1DARKPIC,             // 295
    SPCDOOR2LIGHTPIC,            // 296
    SPCDOOR2DARKPIC,             // 297
    SPCDOOR3LIGHTPIC,            // 298
    SPCDOOR3DARKPIC,             // 299
    SPCDOOR4LIGHTPIC,            // 300
    SPCDOOR4DARKPIC,             // 301
    COLUMNSLIGHTPIC,             // 302
    COLUMNSDARKPIC,              // 303
    DEMONSTATUELIGHTPIC,         // 304
    DEMONSTATUEDARKPIC,          // 305
    TROLLSTATUELIGHTPIC,         // 306
    TROLLSTATUEDARKPIC,          // 307
    BRNDMGVINELIGHTPIC,          // 308
    BRNDMGVINEDARKPIC,           // 309
    HORNDOORPIC,                 // 310
    RUNEDOORPIC,                 // 311
    IRONDMGLIGHTPIC,             // 312
    IRONDMGDARKPIC,              // 313
    IRONLIGHTPIC,                // 314
    IRONDARKPIC,                 // 315
    TROLLLIGHTPIC,               // 316
    TROLLDARKPIC,                // 317
    TROLLBLOODYLIGHTPIC,         // 318
    TROLLBLOODYDARKPIC,          // 319
    STONEDOORLIGHTPIC,           // 320
    STONEDOORDARKPIC,            // 321
    IRONWTR1LIGHTPIC,            // 322
    IRONWTR2LIGHTPIC,            // 323
    IRONWTR3LIGHTPIC,            // 324
    IRONWTR1DARKPIC,             // 325
    IRONWTR2DARKPIC,             // 326
    IRONWTR3DARKPIC,             // 327
    RUSTWTR1LIGHTPIC,            // 328
    RUSTWTR2LIGHTPIC,            // 329
    RUSTWTR3LIGHTPIC,            // 330
    RUSTWTR1DARKPIC,             // 331
    RUSTWTR2DARKPIC,             // 332
    RUSTWTR3DARKPIC,             // 333
    CEMETARYLIGHTPIC,            // 334
    CEMETARYDARKPIC,             // 335
    WGRATE1LIGHTPIC,             // 336
    WGRATE2LIGHTPIC,             // 337
    WGRATE3LIGHTPIC,             // 338
    WGRATE1DARKPIC,              // 339
    WGRATE2DARKPIC,              // 340
    WGRATE3DARKPIC,              // 341
    BWGRATE1PIC,                 // 342
    BWGRATE2PIC,                 // 343
    GWGRATE3PIC,                 // 344
    EGYPT2LIGHTPIC,              // 345
    EGYPT3LIGHTPIC,              // 346
    EGYPT2DARKPIC,               // 347
    EGYPT3DARKPIC,               // 348
    BRNFLGWINDOWLIGHTPIC,        // 349
    BRNFLGWINDOWDARKPIC,         // 350
    ENDPIC,                      // 351
    SGRATEPIC,                   // 352
    EXP_WALL_1PIC,               // 353
    EXP_WALL_2PIC,               // 354
    EXP_WALL_3PIC,               // 355
    WATER_EXP_WALL_1PIC,         // 356
    WATER_EXP_WALL_2PIC,         // 357
    WATER_EXP_WALL_3PIC,         // 358
    MAS_WIN_LIGHTPIC,            // 359
    MAS_WIN_DARKPIC,             // 360
    MAS_VINE1_LIGHTPIC,          // 361
    MAS_VINE1_DARKPIC,           // 362
    MAS_DOOR_LIGHTPIC,           // 363
    MAS_DOOR_DARKPIC,            // 364
    MAS_VINE2_LIGHTPIC,          // 365
    MAS_VINE2_DARKPIC,           // 366
    TAP1PIC,                     // 367
    TAP2PIC,                     // 368
    TAP3PIC,                     // 369
    LASTWALLPIC,                 // 370

    HAND1PICM = 371,

    NORTHICONSPR = 372,
    FIRSTTILESIZE8MASKED = 374,

    LEVEL1TEXT = 663,
    LEVEL2TEXT,                  // 664
    LEVEL3TEXT,                  // 665
    LEVEL4TEXT,                  // 666
    LEVEL5TEXT,                  // 667
    LEVEL6TEXT,                  // 668
    LEVEL7TEXT,                  // 669
    LEVEL8TEXT,                  // 670
    LEVEL9TEXT,                  // 671
    LEVEL10TEXT,                 // 672
    LEVEL11TEXT,                 // 673
    LEVEL12TEXT,                 // 674
    LEVEL13TEXT,                 // 675
    LEVEL14TEXT,                 // 676
    LEVEL15TEXT,                 // 677
    LEVEL16TEXT,                 // 678
    LEVEL17TEXT,                 // 679
    LEVEL18TEXT,                 // 680
    PIRACY,                      // 681
    ENUMEND
} egaGraphicsApocalypse;

const egaGraphStaticData egaGraphApocalypse =
{
    "EGAGRAPH.APC",
    egaGraphOffsetsApocalypse,
    egaDictionaryApocalypse,
    FINALEPIC,
    FIRSTSCALEPIC,
    FIRSTWALLPIC,
    HAND1PICM,
    NORTHICONSPR,
    FIRSTTILESIZE8MASKED,
    LEVEL1TEXT,
    LEVEL18TEXT
};
