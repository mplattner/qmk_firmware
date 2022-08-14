/* Copyright 2020 Chris Paynter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 * 
 * Each of the k__ variables needs to be unique, and typically they follow the format k<row><col>.
 */
#define LAYOUT( \
  K4C, K7B, K7A, K1A, K4A, K19, K1E, K78, K76, K75, K77, K73, K13, K71, K31, K01, K23, \
  K7C, K0C, K0B, K2C, K1C, K79, K7E, K08, K06, K05, K04, K15, K74, K07     , K03, K21, \
  K09			, K0A, K1B, K2A, K29, K2E, K18, K28, K26, K25, K24, K14, K16, K37,      K51, \
  K2B     , K3C, K4B, K3A, K39, K49, K48, K38, K36, K35, K34, K45, K33     , K17, K11, \
  K5D			, K5C, K5B, K5A, K59, K69, K68		 , K58, K56, K55, K44, K52		 , K63, K53, \
  K6G			, K3E, K4F, K6A				   , K67          , K42, K43, K60     , K47, K64, K61 \
) { \
    { KC_NO, K01, KC_NO, K03, K04, K05, K06, K07, K08, K09, K0A, K0B, K0C, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { KC_NO, K11, KC_NO, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, K1C, KC_NO, K1E, KC_NO, KC_NO, }, \
    { KC_NO, K21, KC_NO, K23, K24, K25, K26, KC_NO, K28, K29, K2A, K2B, K2C, KC_NO, K2E, KC_NO, KC_NO, }, \
    { KC_NO, K31, KC_NO, K33, K34, K35, K36, K37, K38, K39, K3A, KC_NO, K3C, KC_NO, K3E, KC_NO, KC_NO, }, \
    { KC_NO, KC_NO, K42, K43, K44, K45, KC_NO, K47, K48, K49, K4A, K4B, K4C, KC_NO, KC_NO, K4F, KC_NO, }, \
    { KC_NO, K51, K52, K53, KC_NO, K55, K56, KC_NO, K58, K59, K5A, K5B, K5C, K5D, KC_NO, KC_NO, KC_NO, }, \
    { K60, K61, KC_NO, K63, K64, KC_NO, KC_NO, K67, K68, K69, K6A, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, K6G, }, \
    { KC_NO, K71, KC_NO, K73, K74, K75, K76, K77, K78, K79, K7A, K7B, K7C, KC_NO, K7E, KC_NO, KC_NO, } \
}
