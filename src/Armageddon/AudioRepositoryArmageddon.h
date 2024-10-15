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
// AudioRepositoryArmageddon
//
// Data structures required for reading the Catacomb Armageddon audio repository.
//
#pragma once

#include "../Engine/AudioRepository.h"
#include <vector>

const huffmanTable audioDictionaryArmageddon =
{
    { 171, 173 },
    { 256, 175 },
    { 257, 185 },
    { 258, 187 },
    { 259, 206 },
    { 260, 208 },
    { 261, 213 },
    { 262, 215 },
    { 263, 217 },
    { 264, 220 },
    { 265, 222 },
    { 266, 231 },
    { 267, 238 },
    { 268, 248 },
    { 269, 250 },
    { 270, 252 },
    { 271, 253 },
    { 272, 254 },
    { 273, 189 },
    { 274, 224 },
    { 225, 227 },
    { 229, 234 },
    { 247, 275 },
    { 194, 199 },
    { 218, 276 },
    { 277, 230 },
    { 89, 201 },
    { 210, 221 },
    { 226, 233 },
    { 236, 278 },
    { 251, 87 },
    { 279, 203 },
    { 216, 280 },
    { 281, 237 },
    { 161, 166 },
    { 214, 219 },
    { 232, 282 },
    { 94, 133 },
    { 134, 143 },
    { 154, 283 },
    { 284, 285 },
    { 243, 246 },
    { 124, 145 },
    { 169, 286 },
    { 82, 113 },
    { 140, 147 },
    { 287, 202 },
    { 288, 289 },
    { 245, 59 },
    { 73, 131 },
    { 152, 190 },
    { 209, 78 },
    { 101, 126 },
    { 129, 141 },
    { 290, 174 },
    { 188, 291 },
    { 223, 120 },
    { 121, 159 },
    { 164, 168 },
    { 176, 192 },
    { 195, 196 },
    { 204, 205 },
    { 292, 293 },
    { 96, 294 },
    { 142, 295 },
    { 157, 191 },
    { 197, 296 },
    { 297, 244 },
    { 61, 108 },
    { 110, 13 },
    { 106, 298 },
    { 127, 130 },
    { 299, 211 },
    { 8, 12 },
    { 24, 71 },
    { 93, 115 },
    { 193, 200 },
    { 300, 100 },
    { 104, 117 },
    { 136, 138 },
    { 301, 144 },
    { 151, 158 },
    { 177, 302 },
    { 303, 241 },
    { 249, 19 },
    { 55, 75 },
    { 137, 212 },
    { 235, 304 },
    { 66, 68 },
    { 305, 306 },
    { 255, 70 },
    { 91, 92 },
    { 95, 99 },
    { 149, 160 },
    { 165, 172 },
    { 307, 28 },
    { 308, 107 },
    { 111, 116 },
    { 309, 310 },
    { 311, 239 },
    { 36, 37 },
    { 53, 114 },
    { 155, 312 },
    { 7, 58 },
    { 60, 77 },
    { 86, 88 },
    { 119, 313 },
    { 125, 146 },
    { 153, 314 },
    { 315, 316 },
    { 317, 45 },
    { 98, 318 },
    { 9, 80 },
    { 319, 135 },
    { 320, 321 },
    { 322, 323 },
    { 64, 109 },
    { 112, 162 },
    { 167, 182 },
    { 31, 324 },
    { 74, 132 },
    { 170, 69 },
    { 325, 123 },
    { 23, 29 },
    { 51, 103 },
    { 326, 327 },
    { 128, 328 },
    { 21, 27 },
    { 44, 62 },
    { 228, 329 },
    { 330, 32 },
    { 331, 102 },
    { 332, 65 },
    { 72, 186 },
    { 57, 81 },
    { 333, 90 },
    { 334, 335 },
    { 139, 336 },
    { 150, 337 },
    { 338, 339 },
    { 38, 54 },
    { 198, 340 },
    { 341, 342 },
    { 180, 343 },
    { 20, 35 },
    { 39, 52 },
    { 56, 83 },
    { 18, 344 },
    { 345, 97 },
    { 16, 346 },
    { 25, 48 },
    { 50, 347 },
    { 348, 349 },
    { 350, 351 },
    { 15, 33 },
    { 79, 84 },
    { 352, 353 },
    { 354, 355 },
    { 11, 30 },
    { 356, 47 },
    { 357, 358 },
    { 207, 359 },
    { 34, 360 },
    { 361, 362 },
    { 363, 148 },
    { 364, 365 },
    { 2, 40 },
    { 181, 366 },
    { 367, 240 },
    { 26, 368 },
    { 369, 370 },
    { 371, 85 },
    { 41, 372 },
    { 105, 373 },
    { 374, 46 },
    { 375, 376 },
    { 242, 377 },
    { 378, 178 },
    { 379, 380 },
    { 381, 382 },
    { 163, 6 },
    { 17, 383 },
    { 384, 4 },
    { 385, 386 },
    { 387, 388 },
    { 389, 118 },
    { 14, 390 },
    { 391, 392 },
    { 393, 394 },
    { 395, 156 },
    { 396, 397 },
    { 398, 122 },
    { 399, 400 },
    { 401, 402 },
    { 3, 403 },
    { 63, 404 },
    { 405, 76 },
    { 406, 407 },
    { 408, 22 },
    { 409, 5 },
    { 43, 410 },
    { 411, 412 },
    { 413, 184 },
    { 414, 415 },
    { 416, 67 },
    { 417, 418 },
    { 419, 420 },
    { 421, 183 },
    { 422, 423 },
    { 424, 425 },
    { 426, 427 },
    { 428, 429 },
    { 430, 431 },
    { 432, 433 },
    { 179, 434 },
    { 435, 436 },
    { 437, 438 },
    { 439, 440 },
    { 441, 49 },
    { 442, 443 },
    { 444, 445 },
    { 1, 446 },
    { 447, 448 },
    { 449, 450 },
    { 10, 451 },
    { 452, 453 },
    { 454, 455 },
    { 456, 457 },
    { 458, 459 },
    { 42, 460 },
    { 461, 462 },
    { 463, 464 },
    { 465, 466 },
    { 467, 468 },
    { 469, 470 },
    { 471, 472 },
    { 473, 474 },
    { 475, 476 },
    { 477, 478 },
    { 479, 480 },
    { 481, 482 },
    { 483, 484 },
    { 485, 486 },
    { 487, 488 },
    { 489, 490 },
    { 491, 492 },
    { 493, 494 },
    { 495, 496 },
    { 497, 498 },
    { 499, 500 },
    { 501, 502 },
    { 503, 504 },
    { 505, 506 },
    { 507, 0 },
    { 508, 509 },
    { 0, 0 },

};

const std::vector<int32_t> audioRepositoryOffsetsArmageddon =
{
    0,
    29,
    203,
    328,
    398,
    445,
    510,
    624,
    688,
    701,
    735,
    793,
    864,
    949,
    983,
    992,
    1001,
    1067,
    1097,
    1284,
    1313,
    1365,
    1445,
    1491,
    1538,
    1547,
    1579,
    1740,
    1845,
    1938,
    2055,
    2160,
    2250,
    2350,
    2362,
    2428,
    2469,
    2662,
    2799,
    2889,
    2957,
    3046,
    3172,
    3255,
    3286,
    3396,
    3517,
    3607,
    3672,
    3724,
    3745,
    3765,
    3814,
    3853,
    4059,
    4101,
    4166,
    4267,
    4367,
    4424,
    4448,
    4499,
    4519,
    4542,
    4583,
    4773,
    4910,
    5015,
    5127,
    5159,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    6325,
    5300,
    7104
};

typedef enum {
    HITWALLSND,              // 0
    WARPUPSND,               // 1
    WARPDOWNSND,             // 2
    GETBOLTSND,              // 3
    GETNUKESND,              // 4
    GETPOTIONSND,            // 5
    GETKEYSND,               // 6
    GETSCROLLSND,            // 7
    GETPOINTSSND,            // 8
    USEBOLTSND,              // 9
    USENUKESND,              // 10
    USEPOTIONSND,            // 11
    USEKEYSND,               // 12
    NOITEMSND,               // 13
    WALK1SND,                // 14
    WALK2SND,                // 15
    TAKEDAMAGESND,           // 16
    MONSTERMISSSND,          // 17
    GAMEOVERSND,             // 18
    SHOOTSND,                // 19
    BIGSHOOTSND,             // 20
    SHOOTWALLSND,            // 21
    SHOOTMONSTERSND,         // 22
    TAKEDMGHURTSND,          // 23
    BALLBOUNCESND,           // 24
    NOWAYSND,                // 25
    WARPSND,                 // 26
    HIT_GATESND,             // 27
    GETGEMSND,               // 28
    BOOMSND,                 // 29
    GRELM_DEADSND,           // 30
    FREEZETIMESND,           // 31
    TIMERETURNSND,           // 32
    TICKSND,                 // 33
    BODY_EXPLODESND,         // 34
    LASTSOUND
} audioArmageddon;

const audioRepositoryStaticData audioRepositoryArmageddon =
{
    "AUDIO.ARM",
    audioRepositoryOffsetsArmageddon,
    audioDictionaryArmageddon,
    LASTSOUND,
    true
};

