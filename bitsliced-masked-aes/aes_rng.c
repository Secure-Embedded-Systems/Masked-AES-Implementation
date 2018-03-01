//#include <stdio.h>
#include "aes_core.h"
#include "aes_rng.h"
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>


typedef unsigned char u8;

u8 * prng(int init) {
    unsigned i;
    static u8 key[] = {0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F};

    static u8 txt[] = {0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB,
        0xCC, 0xDD, 0xEE, 0xFF};

    static u8 dout[16];

    static AES_KEY keyexp;

    if (init) {  //key expansion
        AES_set_encrypt_key(key, 128, &keyexp);
    } else {
        
        AES_encrypt(txt, dout, &keyexp);
        
        
        // CT is fed to PL
        for (i=0; i<16; i++) {
            txt[i] = dout[i];
        }
    }
   return txt;

}

