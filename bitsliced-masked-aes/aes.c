
/*
 *
 * Virginia Tech
 * Secure Embedded Systems Lab
 *
 * Copyright (C) 2017 Virginia Tech
 * If you use our code in your publications, please cite the paper below:
 * Yao, Yuan, Mo Yang, Conor Patrick, Bilgiday Yuce, and Patrick Schaumont. "Fault-assisted side-channel 
 * analysis of masked implementations." In 2018 IEEE International Symposium on Hardware Oriented Security 
 * and Trust (HOST), pp. 57-64. IEEE, 2018.
 *
 * Written in 2017 by Conor Patrick, Yuan Yao, Mo Yang. 
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


//#include <stdio.h>//mo debug: for printf
//#include <stdlib.h> //mo debug: for exit(-1)
#include <string.h>
#include "aes.h"
#include "bs.h"
#include "utils.h"

void aesm_ecb_encrypt(uint8_t * outputb, uint8_t * inputb, uint8_t * m_vector, size_t size, uint8_t * key)
{
    uint8_t * maskb = m_vector;
    word_t input_space[BLOCK_SIZE];
    word_t rk[11][BLOCK_SIZE];

    memset(outputb,0,size);
    // word_t * state = (word_t *)outputb;

    bs_expand_key(rk, key);

    int i,j;

  /*  if (size>BS_BLOCK_SIZE/2) { // size should <= BS_BLOCK_SIZE/2
        printf("ERROR: There are %d AES run in parallel, max 16 \n", (int)size/16);
        exit(-1);

    }*/
    memset(input_space,0,BS_BLOCK_SIZE);

    for(i = 0; i < 2*size/16; i += 2)  //mo: change code, but didn't consider if the input is not multiple of 16 bytes
    {
        memmove(input_space + (i * WORDS_PER_BLOCK), inputb, BLOCK_SIZE/8);
        memmove(input_space + (i * WORDS_PER_BLOCK + WORDS_PER_BLOCK), maskb, BLOCK_SIZE/8);

        maskb += BLOCK_SIZE/8;
        inputb += BLOCK_SIZE/8;

        for (j = 0; j < WORDS_PER_BLOCK; j++)
        {
            input_space[ i * WORDS_PER_BLOCK + j] ^= input_space[ i * WORDS_PER_BLOCK + WORDS_PER_BLOCK + j];
        }   //  |p+m|m|p+m|m|... p:certain byte of plaintext  

    }
    bs_cipher(input_space, rk, m_vector);

    for(i = 0; i < WORD_SIZE; i += 2)
    {
        for (j = 0; j < WORDS_PER_BLOCK; j++)
        {
            input_space[ i * WORDS_PER_BLOCK + j] ^= input_space[ i * WORDS_PER_BLOCK + WORDS_PER_BLOCK + j];
        }
    }
    //mo: change memmove    c|m|c|m output every the other byte
    for (i=0; i<size/16; i++) 
    {
        memmove(outputb+i*16,input_space+i*2*WORDS_PER_BLOCK, 16);
    }
}
