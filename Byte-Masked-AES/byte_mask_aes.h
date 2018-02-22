

/*
 *
 * Virginia Tech
 * Secure Embedded Systems Lab
 *
 * Copyright (C) 2017 Virginia Tech
 *
 * Written in 2017 by Yuan Yao. This masked implementation refers to 
 * https://github.com/ermin-sakic/smartcard-aes-fw.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BYTE_AES_MASK
#define BYTE_AES_MASK

#include <stdint.h>

extern uint8_t Mask [10];


static uint8_t getSBoxValue(uint8_t num);
void KeyExpansion(uint8_t* Key);
void mixColumns(uint8_t state[16]);
void shiftRows(uint8_t state[16]);
void aes128(uint8_t state[16]);

void init_masking();
void remask(uint8_t s[16], uint8_t m1, uint8_t m2, uint8_t m3, uint8_t m4, uint8_t m5, uint8_t m6, uint8_t m7, uint8_t m8);
void subBytes_masked(uint8_t state[16]);
void init_masked_round_keys();
void calcInvSbox_masked();
void calcMixColMask();
void addRoundKey_masked(uint8_t state[16],uint8_t round);
void copy_key();

void gen_random_sequence(uint8_t hiding_sequence[16]);
void subBytes_masked_rand(uint8_t state[16],uint8_t hiding_sequence[16]);
#endif

