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
// EgaGraphArmageddon
//
// Data structures required for reading the Catacomb Armageddon EGA graphics repository.
//
#pragma once

#include "..\Engine\EgaGraph.h"

const huffmanTable egaDictionaryArmageddon =
{
    { 173, 182 },
    { 150, 77 },
    { 154, 165 },
    { 169, 178 },
    { 210, 218 },
    { 86, 172 },
    { 139, 45 },
    { 73, 90 },
    { 89, 186 },
    { 202, 157 },
    { 256, 177 },
    { 181, 106 },
    { 137, 147 },
    { 69, 53 },
    { 138, 82 },
    { 91, 180 },
    { 146, 107 },
    { 54, 185 },
    { 213, 141 },
    { 43, 257 },
    { 212, 205 },
    { 41, 148 },
    { 171, 75 },
    { 83, 153 },
    { 201, 81 },
    { 162, 155 },
    { 203, 149 },
    { 37, 109 },
    { 58, 258 },
    { 209, 217 },
    { 166, 259 },
    { 164, 211 },
    { 163, 189 },
    { 133, 38 },
    { 197, 174 },
    { 233, 260 },
    { 117, 78 },
    { 70, 158 },
    { 145, 161 },
    { 93, 50 },
    { 261, 74 },
    { 179, 234 },
    { 52, 118 },
    { 262, 44 },
    { 98, 263 },
    { 57, 183 },
    { 167, 264 },
    { 26, 87 },
    { 94, 265 },
    { 46, 221 },
    { 237, 266 },
    { 229, 84 },
    { 267, 268 },
    { 156, 269 },
    { 61, 142 },
    { 151, 206 },
    { 66, 49 },
    { 59, 270 },
    { 105, 113 },
    { 271, 76 },
    { 272, 68 },
    { 273, 274 },
    { 275, 187 },
    { 276, 215 },
    { 100, 196 },
    { 222, 277 },
    { 278, 236 },
    { 235, 42 },
    { 198, 35 },
    { 194, 29 },
    { 279, 280 },
    { 34, 121 },
    { 281, 282 },
    { 226, 283 },
    { 39, 140 },
    { 125, 170 },
    { 102, 230 },
    { 284, 99 },
    { 27, 285 },
    { 219, 33 },
    { 51, 228 },
    { 286, 287 },
    { 92, 55 },
    { 18, 288 },
    { 190, 36 },
    { 214, 289 },
    { 290, 291 },
    { 152, 292 },
    { 216, 67 },
    { 293, 294 },
    { 110, 242 },
    { 119, 134 },
    { 295, 184 },
    { 88, 296 },
    { 114, 297 },
    { 22, 71 },
    { 72, 19 },
    { 298, 299 },
    { 238, 25 },
    { 116, 246 },
    { 104, 300 },
    { 200, 204 },
    { 65, 108 },
    { 130, 132 },
    { 123, 175 },
    { 301, 220 },
    { 168, 302 },
    { 245, 303 },
    { 188, 304 },
    { 305, 306 },
    { 307, 85 },
    { 21, 308 },
    { 232, 309 },
    { 17, 176 },
    { 79, 310 },
    { 101, 40 },
    { 136, 311 },
    { 244, 312 },
    { 313, 314 },
    { 315, 208 },
    { 316, 47 },
    { 144, 317 },
    { 103, 115 },
    { 23, 318 },
    { 319, 320 },
    { 321, 20 },
    { 322, 97 },
    { 95, 323 },
    { 324, 325 },
    { 250, 326 },
    { 327, 328 },
    { 225, 329 },
    { 9, 330 },
    { 331, 332 },
    { 333, 334 },
    { 80, 335 },
    { 336, 337 },
    { 30, 11 },
    { 338, 339 },
    { 340, 341 },
    { 135, 342 },
    { 343, 344 },
    { 13, 345 },
    { 346, 347 },
    { 348, 62 },
    { 349, 60 },
    { 126, 350 },
    { 227, 199 },
    { 351, 352 },
    { 353, 195 },
    { 124, 354 },
    { 355, 356 },
    { 357, 358 },
    { 160, 359 },
    { 360, 361 },
    { 120, 122 },
    { 362, 363 },
    { 364, 111 },
    { 365, 193 },
    { 241, 10 },
    { 28, 366 },
    { 143, 367 },
    { 368, 369 },
    { 56, 370 },
    { 371, 372 },
    { 373, 14 },
    { 374, 375 },
    { 376, 129 },
    { 377, 378 },
    { 5, 379 },
    { 380, 131 },
    { 231, 381 },
    { 382, 383 },
    { 384, 385 },
    { 243, 207 },
    { 386, 387 },
    { 388, 389 },
    { 390, 391 },
    { 249, 392 },
    { 393, 394 },
    { 112, 395 },
    { 396, 159 },
    { 397, 398 },
    { 24, 399 },
    { 400, 401 },
    { 402, 403 },
    { 12, 404 },
    { 48, 405 },
    { 406, 407 },
    { 408, 409 },
    { 410, 411 },
    { 412, 251 },
    { 96, 413 },
    { 6, 247 },
    { 239, 414 },
    { 415, 416 },
    { 417, 418 },
    { 419, 420 },
    { 253, 421 },
    { 422, 423 },
    { 424, 191 },
    { 425, 223 },
    { 426, 427 },
    { 428, 429 },
    { 430, 431 },
    { 432, 433 },
    { 434, 16 },
    { 435, 436 },
    { 437, 438 },
    { 8, 439 },
    { 440, 441 },
    { 442, 443 },
    { 444, 2 },
    { 4, 445 },
    { 446, 447 },
    { 448, 449 },
    { 450, 451 },
    { 248, 31 },
    { 15, 64 },
    { 452, 240 },
    { 453, 454 },
    { 252, 32 },
    { 63, 455 },
    { 224, 456 },
    { 7, 457 },
    { 458, 459 },
    { 460, 3 },
    { 461, 192 },
    { 462, 463 },
    { 464, 254 },
    { 465, 127 },
    { 466, 467 },
    { 468, 469 },
    { 470, 471 },
    { 472, 473 },
    { 474, 475 },
    { 1, 476 },
    { 128, 477 },
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
    { 507, 508 },
    { 0, 509 },
    { 0, 0 }
};

const std::vector<int32_t> egaGraphOffsetsArmageddon =
{
0,
709,
716,
732,
1913,
13792,
20084,
20767,
20776,
21382,
21986,
22648,
23337,
23957,
24382,
24445,
24504,
24559,
24615,
24671,
24682,
24693,
24704,
25664,
26607,
27552,
28531,
29456,
30315,
31234,
32319,
33395,
34094,
34811,
35556,
36301,
37488,
38732,
39927,
41133,
42026,
42920,
43823,
44619,
45253,
46048,
46690,
47218,
48023,
48323,
48605,
49144,
49783,
50516,
51333,
52127,
52920,
53718,
54511,
55229,
55983,
56926,
57654,
57948,
58241,
58531,
58823,
59122,
59412,
59841,
60270,
60889,
61568,
61941,
62195,
62463,
62718,
62985,
63211,
63438,
63663,
63889,
64117,
64346,
64585,
64822,
65057,
65291,
66019,
66319,
66601,
67110,
67667,
68126,
69224,
70236,
71334,
72373,
73569,
74782,
75924,
77053,
78404,
79435,
80208,
80981,
81748,
82227,
82859,
83266,
83672,
84100,
84564,
84991,
85417,
85820,
86173,
87318,
88457,
89252,
90083,
90814,
91367,
91945,
92546,
93113,
93589,
93930,
95074,
96202,
97324,
98546,
99727,
100847,
102239,
103295,
104328,
105066,
105684,
106568,
107383,
108209,
109046,
109837,
110705,
111306,
111906,
112975,
114052,
115071,
116094,
117109,
118083,
119034,
120161,
121307,
122444,
124019,
124930,
125808,
126735,
127597,
128524,
129478,
130414,
131298,
132254,
132896,
133289,
134358,
135470,
136572,
137676,
138744,
139837,
140928,
142067,
143159,
144198,
144515,
145010,
145478,
146218,
147342,
148196,
149065,
149845,
150709,
151768,
152912,
154094,
155062,
155836,
156571,
157216,
157636,
158100,
158521,
158982,
159660,
160477,
161313,
162134,
163097,
163964,
164557,
165365,
166303,
166967,
167667,
168735,
169594,
171150,
172399,
173036,
174264,
175251,
176212,
177281,
178022,
178599,
179131,
179667,
180197,
181006,
181820,
182728,
184075,
185399,
186689,
188063,
188856,
188870,
190764,
192664,
194564,
196462,
197874,
199291,
200705,
202119,
203146,
204104,
205077,
206075,
207626,
208432,
209110,
210416,
211384,
212663,
213950,
214713,
216249,
217828,
218988,
219908,
221228,
222293,
223445,
224379,
225647,
226498,
227468,
228106,
229220,
230065,
231593,
232719,
233617,
234383,
235615,
236540,
237715,
238544,
240142,
241395,
242712,
243658,
245660,
247273,
248371,
249932,
251202,
252770,
254402,
255956,
257263,
258459,
259682,
260929,
261892,
262870,
263871,
264882,
265910,
266958,
267823,
268700,
269594,
270760,
271753,
273185,
274328,
275278,
276221,
277171,
277878,
278579,
279284,
280124,
281041,
281919,
282903,
283928,
284813,
286252,
287624,
288768,
289704,
290469,
291459,
292286,
293835,
295179,
295821,
297019,
298120,
299429,
300607,
301941,
302974,
304204,
305116,
306519,
307752,
309161,
310403,
311380,
312210,
313465,
314628,
316275,
317688,
319319,
320314,
321589,
322909,
322925,
324435,
324467,
325759,
326070,
326087,
326227,
326364,
326502,
326641,
326716,
326790,
326864,
326940,
327064,
327129,
327239,
327348,
327426,
327502,
327616,
327688,
327819,
327928,
328034,
328148,
328235,
328332,
328444,
328526,
328634,
328749,
328854,
328957,
329037,
329129,
329206,
329298,
329389,
329508,
329591,
329701,
329786,
329935,
330011,
330136,
330223,
330321,
330455,
330576,
330685,
330803,
330919,
331037,
331143,
331240,
331336,
331432,
331522,
331614,
331706,
331799,
331891,
331985,
332075,
332166,
332259,
332364,
332469,
332576,
332677,
332801,
332908,
333024,
333116,
333245,
333355,
333456,
333566,
333684,
333812,
333912,
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
-1,
-1,
-1,
-1,
334012,
334058,
334139,
334222,
334291,
334379,
334452,
334522,
334598,
334686,
334764,
334835,
334921,
334988,
335077,
335166,
335253,
335330,
335416,
335497,
335569,
335633,
335720,
335802,
335892,
335979,
336054,
336124,
336170,
336216,
336262,
336308,
336354,
336400,
336446,
-1,
-1,
336496,
336638,
336763,
336904,
337038,
337150,
337262,
337376,
337474,
337577,
337671,
337773,
337802,
337831,
337860,
337889,
337918,
337947,
338083,
338229,
338346,
338480,
338581,
338719,
338829,
338877,
338997,
339078,
339201,
339230,
339321,
339461,
339600,
339734,
339873,
340007,
340112,
340227,
340349,
340445,
340570,
340688,
340812,
340949,
341084,
341207,
341344,
341470,
341499,
341600,
341680,
341837,
341953,
342004,
342129,
342229,
342380,
342500,
342595,
342761,
342841,
342912,
342981,
343109,
343216,
343300,
343409,
343538,
343642,
343724,
343831,
343924,
344389,
344807,
345415,
345903,
346563,
346928,
347267,
347670,
348106,
348660,
349065,
349400,
349761,
350165,
350715,
351200,
351629,
355832
};

typedef enum {
    FINALEPIC=4,
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
    FIRSTSTRIPPIC,               // 20
    FIRSTSCALEPIC,               // 21
    // Lump Start
    SKELETON_1PIC,               // 22
    SKELETON_2PIC,               // 23
    SKELETON_3PIC,               // 24
    SKELETON_4PIC,               // 25
    SKELETON_ATTACK_1PIC,        // 26
    SKELETON_ATTACK_2PIC,        // 27
    SKELETON_ATTACK_3PIC,        // 28
    SKELETON_OUCHPIC,            // 29
    SKELETON_DEATH_1PIC,         // 30
    SKELETON_DEATH_2PIC,         // 31
    // Lump Start
    TOMB1PIC,                    // 32
    TOMB2PIC,                    // 33
    TOMB3PIC,                    // 34
    // Lump Start
    OBJ_WARP1PIC,                // 35
    OBJ_WARP2PIC,                // 36
    OBJ_WARP3PIC,                // 37
    OBJ_WARP4PIC,                // 38
    // Lump Start
    EYE_WALK1PIC,                // 39
    EYE_WALK2PIC,                // 40
    EYE_WALK3PIC,                // 41
    EYE_OUCH1PIC,                // 42
    EYE_OUCH2PIC,                // 43
    EYE_DEATH1PIC,               // 44
    EYE_DEATH2PIC,               // 45
    EYE_DEATH3PIC,               // 46
    EYE_SCOWLPIC,                // 47
    EYE_SHOT1PIC,                // 48
    EYE_SHOT2PIC,                // 49
    // Lump Start
    ZOMB_APPEAR1PIC,             // 50
    ZOMB_APPEAR2PIC,             // 51
    ZOMB_APPEAR3PIC,             // 52
    ZOMB_APPEAR4PIC,             // 53
    ZOMB_WALK1PIC,               // 54
    ZOMB_WALK2PIC,               // 55
    ZOMB_WALK3PIC,               // 56
    ZOMB_OUCHPIC,                // 57
    ZOMB_ATTACKPIC,              // 58
    ZOMB_DIE1PIC,                // 59
    ZOMB_DIE2PIC,                // 60
    ZOMB_DIE3PIC,                // 61
    // Lump Start
    BOLTOBJPIC,                  // 62
    BOLT2OBJPIC,                 // 63
    BOLT3OBJPIC,                 // 64
    // Lump Start
    NUKEOBJPIC,                  // 65
    NUKE2OBJPIC,                 // 66
    NUKE3OBJPIC,                 // 67
    // Lump Start
    TIMEOBJ1PIC,                 // 68
    TIMEOBJ2PIC,                 // 69
    // Lump Start
    O_WATER_CHEST1PIC,           // 70
    O_WATER_CHEST2PIC,           // 71
    // Lump Start
    POTIONOBJPIC,                // 72
    // Lump Start
    RKEYOBJPIC,                  // 73
    // Lump Start
    YKEYOBJPIC,                  // 74
    // Lump Start
    GKEYOBJPIC,                  // 75
    // Lump Start
    BKEYOBJPIC,                  // 76
    RGEM1PIC,                    // 77
    RGEM2PIC,                    // 78
    GGEM1PIC,                    // 79
    GGEM2PIC,                    // 80
    BGEM1PIC,                    // 81
    BGEM2PIC,                    // 82
    YGEM1PIC,                    // 83
    YGEM2PIC,                    // 84
    PGEM1PIC,                    // 85
    PGEM2PIC,                    // 86
    CHESTOBJPIC,                 // 87
    PSHOT1PIC,                   // 88
    PSHOT2PIC,                   // 89
    PSHOT_EXP1PIC,               // 90
    PSHOT_EXP2PIC,               // 91
    PSHOT_EXP3PIC,               // 92

    RED_DEMON1PIC,               // 93
    RED_DEMON2PIC,               // 94
    RED_DEMON3PIC,               // 95
    RED_DEMON4PIC,               // 96
    RED_DEMONATTACK1PIC,         // 97
    RED_DEMONATTACK2PIC,         // 98
    RED_DEMONATTACK3PIC,         // 99
    RED_DEMONOUCHPIC,            // 100
    RED_DEMONDIE1PIC,            // 101
    RED_DEMONDIE2PIC,            // 102
    RED_DEMONDIE3PIC,            // 103

    MAGE1PIC,                    // 104
    MAGE2PIC,                    // 105
    MAGEOUCHPIC,                 // 106
    MAGEATTACKPIC,               // 107
    MAGEDIE1PIC,                 // 108
    MAGEDIE2PIC,                 // 109

    BAT1PIC,                     // 110
    BAT2PIC,                     // 111
    BAT3PIC,                     // 112
    BAT4PIC,                     // 113
    BATDIE1PIC,                  // 114
    BATDIE2PIC,                  // 115

    GREL1PIC,                    // 116
    GREL2PIC,                    // 117
    GRELATTACKPIC,               // 118
    GRELHITPIC,                  // 119
    GRELDIE1PIC,                 // 120
    GRELDIE2PIC,                 // 121
    GRELDIE3PIC,                 // 122
    GRELDIE4PIC,                 // 123
    GRELDIE5PIC,                 // 124
    GRELDIE6PIC,                 // 125
	SKULL_SHOTPIC,               // 126

    GODESS_WALK1PIC,             // 127
    GODESS_WALK2PIC,             // 128
    GODESS_WALK3PIC,             // 129
    GODESS_ATTACK1PIC,           // 130
    GODESS_ATTACK2PIC,           // 131
    GODESS_ATTACK3PIC,           // 132
    GODESS_STATUEPIC,            // 133
    GODESS_OUCHPIC,              // 134
    GODESS_DEATH1PIC,            // 135
    GODESS_DEATH2PIC,            // 136

    ANT_EGG1PIC,                 // 137
    ANT_EGG2PIC,                 // 138
    ANT_WALK1PIC,                // 139
    ANT_WALK2PIC,                // 140
    ANT_WALK3PIC,                // 141
    ANT_ATTACKPIC,               // 142
    ANT_DEATH1PIC,               // 143
    ANT_DEATH2PIC,               // 144
    ANT_DEATH3PIC,               // 145

    FATDEMON_WALK1PIC,           // 146
    FATDEMON_WALK2PIC,           // 147
    FATDEMON_WALK3PIC,           // 148
    FATDEMON_WALK4PIC,           // 149
    FATDEMON_ATTACK1PIC,         // 150
    FATDEMON_ATTACK2PIC,         // 151
    FATDEMON_OUCHPIC,            // 152
    FATDEMON_BLOWUP1PIC,         // 153
    FATDEMON_BLOWUP2PIC,         // 154
    FATDEMON_BLOWUP3PIC,         // 155
    FATDEMON_EXPLODEPIC,         // 156
    FATDEMON_FEETPIC,            // 157

    SUCCUBUS_WALK1PIC,           // 158
    SUCCUBUS_WALK2PIC,           // 159
    SUCCUBUS_WALK3PIC,           // 160
    SUCCUBUS_WALK4PIC,           // 161
    SUCCUBUS_ATTACK1PIC,         // 162
    SUCCUBUS_ATTACK2PIC,         // 163
    SUCCUBUS_OUCHPIC,            // 164
    SUCCUBUS_DEATH1PIC,          // 165
    SUCCUBUS_DEATH2PIC,          // 166
    SUCCUBUS_SHOT1PIC,           // 167

    TREE_IDLEPIC,                // 168
    TREE_AWAKENINGPIC,           // 169
    TREE_WALK1PIC,               // 170
    TREE_WALK2PIC,               // 171
    TREE_ATTACK1PIC,             // 172
    TREE_ATTACK2PIC,             // 173
    TREE_ATTACK3PIC,             // 174
    TREE_DEATH1PIC,              // 175
    TREE_DEATH2PIC,              // 176
    TREE_DEATH3PIC,              // 177

    DRAGON_BUBBLES1PIC,          // 178
    DRAGON_BUBBLES2PIC,          // 179
    DRAGON_EYESPIC,              // 180
    DRAGON_RISE1PIC,             // 181
    DRAGON_RISE2PIC,             // 182
    DRAGON_WALK1PIC,             // 183
    DRAGON_WALK2PIC,             // 184
    DRAGON_WALK3PIC,             // 185
    DRAGON_WALK4PIC,             // 186
    DRAGON_ATTACK1PIC,           // 187
    DRAGON_ATTACK2PIC,           // 188
    DRAGON_ATTACK3PIC,           // 189
    DRAGON_OUCHPIC,              // 190
    DRAGON_DEATH1PIC,            // 191
    DRAGON_DEATH2PIC,            // 192
    DRAGON_DEATH3PIC,            // 193

    BUNNY_LEFT1PIC,              // 194
    BUNNY_LEFT2PIC,              // 195
    BUNNY_RIGHT1PIC,             // 196
    BUNNY_RIGHT2PIC,             // 197
    BUNNY_META1PIC,              // 198
    BUNNY_META2PIC,              // 199
    BUNNY_WALK1PIC,              // 200
    BUNNY_WALK2PIC,              // 201
    BUNNY_OUCHPIC,               // 202
    BUNNY_DEATH1PIC,             // 203
    BUNNY_DEATH2PIC,             // 204

    ARCH1PIC,                    // 205
	ARCH2PIC,                    // 206
    ARCH3PIC,                    // 207
    ARCH4PIC,                    // 208
    ARCH5PIC,                    // 209
    ARCH6PIC,                    // 210
    ARCH7PIC,                    // 211
    ARCH8PIC,                    // 212
    ARCH9PIC,                    // 213
    ARCH10PIC,                   // 214
    ARCH11PIC,                   // 215
    ARCH12PIC,                   // 216
    ARCH13PIC,                   // 217

    ANT_HILLPIC,                 // 218
    COLUMNPIC,                   // 219
    SULPHUR_GAS_1PIC,            // 220
    SULPHUR_GAS_2PIC,            // 221
    SULPHUR_GAS_3PIC,            // 222
    FIRE_POT_1PIC,               // 223
    FIRE_POT_2PIC,               // 224
    SKEL_HANGPIC,                // 225
    FORCE_FIELD_1PIC,            // 226
    FORCE_FIELD_2PIC,            // 227
    FORCE_FIELD_3PIC,            // 228
    FORCE_FIELD_4PIC,            // 229
    WFOUNTAINPIC,                // 230

    FIRSTWALLPIC,                // 231
    CRYSTAL_LIGHT_1PIC,          // 232
    CRYSTAL_LIGHT_2PIC,          // 233
    CRYSTAL_LIGHT_3PIC,          // 234
    CRYSTAL_LIGHT_4PIC,          // 235
    CRYSTAL_DARK_1PIC,           // 236
    CRYSTAL_DARK_2PIC,           // 237
    CRYSTAL_DARK_3PIC,           // 238
    CRYSTAL_DARK_4PIC,           // 239
    FIRE_WALL_1PIC,              // 240
    FIRE_WALL_2PIC,              // 241
    FIRE_WALL_3PIC,              // 242
    FIRE_WALL_4PIC,              // 243
    BRN_STONE_GATEPIC,           // 244
    BRN_STONE_WALL_1PIC,         // 245
    BRN_STONE_WALL_2PIC,         // 246
    KUDZU_LIGHT_WALLPIC,         // 247
    KUDZU_DARK_WALLPIC,          // 248
    HEDGE_WALLPIC,               // 249
    HEDGE_EYESPIC,               // 250
    BRN_WINDOW_LIGHTPIC,         // 251
    ALTAR_LEFTPIC,               // 252
    ALTAR_RIGHTPIC,              // 253
    GRAY_LIGHT_WALLPIC,          // 254
    GRAY_DARK_WALLPIC,           // 255
    GRAY_LIGHT_SIGNPIC,          // 256
    GRAY_DARK_SIGNPIC,           // 257
    MANICLE_LIGHT_BLOODYPIC,     // 258
    MANICLE_DARK_BLOODYPIC,      // 259
    LIGHT_CURTAIN_WINDOWPIC,     // 260
    LIGHT_CURTAIN_WALLPIC,       // 261
    DARK_CURTAIN_WINDOWPIC,      // 262
    DARK_CURTAIN_WALLPIC,        // 263
    BRN_LIGHT_SIGNPIC,           // 264
    BRN_DARK_SIGNPIC,            // 265
    LIGHT_STONE_WALLPIC,         // 266
    DARK_STONE_WALLPIC,          // 267
    BRN_FLAGSTONE_LIGHT_2PIC,    // 268
    BRN_FLAGSTONE_DARK_2PIC,     // 269
    RUST_METAL_LIGHTPIC,         // 270
    RUST_METAL_DARKPIC,          // 271
    GRAY_METAL_LIGHTPIC,         // 272
    GRAY_METAL_DARKPIC,          // 273
    WEAK_STONE_LIGHTPIC,         // 274
    WEAK_STONE_DARKPIC,          // 275
    WEAK_GRAY_RFGSTN_LIGHTPIC,   // 276
    WEAK_GRAY_RFGSTN_DARKPIC,    // 277
    WEAK_CRYSTAL_LIGHTPIC,       // 278
    WEAK_CRYSTAL_DARKPIC,        // 279
    RED_MUD_LIGHTPIC,            // 280
    BRN_MUD_DARKPIC,             // 281
    RED_MUD_WEAK_LIGHTPIC,       // 282
    BRN_MUD_WEAK_DARKPIC,        // 283
    HORN_DOORPIC,                // 284
    CLOSED_DOOR_1PIC,            // 285
    DOOR_2PIC,                   // 286
    WATER_LIGHT_WEAK_1PIC,       // 287
    WATER_LIGHT_WEAK_2PIC,       // 288
    WATER_LIGHT_WEAK_3PIC,       // 289
    WATER_DARK_WEAK_1PIC,        // 290
    WATER_DARK_WEAK_2PIC,        // 291
    WATER_DARK_WEAK_3PIC,        // 292
    WATER_LIGHT_1PIC,            // 293
    WATER_LIGHT_2PIC,            // 294
    WATER_LIGHT_3PIC,            // 295
    WATER_DARK_1PIC,             // 296
    WATER_DARK_2PIC,             // 297
    WATER_DARK_3PIC,             // 298
    TROLL_LIGHT_STONEPIC,        // 299
    TROLL_DARK_STONEPIC,         // 300
    TROLL_BLOODY_LT_STONEPIC,    // 301
    TROLL_BLOODY_DK_STONEPIC,    // 302
    LIGHT_BREATH_1PIC,           // 303
    LIGHT_BREATH_2PIC,           // 304
    LIGHT_BREATH_3PIC,           // 305
    DARK_BREATH_1PIC,            // 306
    DARK_BREATH_2PIC,            // 307
    DARK_BREATH_3PIC,            // 308
    EXP_WALL_1PIC,               // 309
    EXP_WALL_2PIC,               // 310
    EXP_WALL_3PIC,               // 311
    WATER_EXP_WALL_1PIC,         // 312
    WATER_EXP_WALL_2PIC,         // 313
    WATER_EXP_WALL_3PIC,         // 314
    W_GEN_DOOR1PIC,              // 315
    W_GEN_DOOR2PIC,              // 316
    W_CRYSTAL_DOORPIC,           // 317
    DMG_BRN_FSTN_LTPIC,          // 318
    DMG_BRN_FSTN_DKPIC,          // 319
    DMG_FIN_FSTN_LTPIC,          // 320
    DMG_FIN_FSTN_DKPIC,          // 321
    STEEL_DOOR1PIC,              // 322
    STEEL_DOOR2PIC,              // 323
    BRN_WINDOW_DARKPIC,          // 324
    GRY_DOOR_LTPIC,              // 325
    GRY_DOOR_DKPIC,              // 326
    BRN_DOOR_LTPIC,              // 327
    BRN_DOOR_DKPIC,              // 328
    GRY_FGSTN_LTPIC,             // 329
    GRY_FGSTN_DKPIC,             // 330
    KUDZU_WEAK_LIGHTPIC,         // 331
    KUDZU_WEAK_DARKPIC,          // 332
    LT_SKEL1PIC,                 // 333
    DK_SKEL1PIC,                 // 334
    LT_SKEL2PIC,                 // 335
    DK_SKEL2PIC,                 // 336
    MANICLE_LIGHT_WALLPIC,       // 337
    MANICLE_DARK_WALLPIC,        // 338
    TAP_1PIC,                    // 339
    TAP_2PIC,                    // 340
    TAP_3PIC,                    // 341
    TAP_4PIC,                    // 342
    TAP_5PIC,                    // 343
    FINALWALLPIC,                // 344
    WATER_DOOR1_PIC,             // 345
    WATER_DOOR2_PIC,             // 346
    LASTWALLPIC,                 // 347

    HAND1PICM=348,

    NORTHICONSPR=349,
    FIRSTTILESIZE8=350,
    FIRSTTILESIZE8MASKED = 351,

    LEVEL1TEXT=640,
    LEVEL2TEXT,                  // 641
    LEVEL3TEXT,                  // 642
    LEVEL4TEXT,                  // 643
    LEVEL5TEXT,                  // 644
    LEVEL6TEXT,                  // 645
    LEVEL7TEXT,                  // 646
    LEVEL8TEXT,                  // 647
    LEVEL9TEXT,                  // 648
    LEVEL10TEXT,                 // 649
    LEVEL11TEXT,                 // 650
    LEVEL12TEXT,                 // 651
    LEVEL13TEXT,                 // 652
    LEVEL14TEXT,                 // 653
    LEVEL15TEXT,                 // 654
    LEVEL16TEXT,                 // 655
    LEVEL17TEXT,                 // 656
    PIRACY,                      // 657
    ENUMEND
} egaGraphicsArmageddon;

const egaGraphStaticData egaGraphArmageddon =
{
    "EGAGRAPH.ARM",
    egaGraphOffsetsArmageddon,
    egaDictionaryArmageddon,
    FINALEPIC,
    FIRSTSCALEPIC,
    FIRSTWALLPIC,
    HAND1PICM,
    NORTHICONSPR,
    FIRSTTILESIZE8,
    FIRSTTILESIZE8MASKED,
    LEVEL1TEXT,
    LEVEL17TEXT,
    HAND1PICM
};