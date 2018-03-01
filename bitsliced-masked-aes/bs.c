#include <string.h>
#include "bs.h"
//#include "testbench/app.h"
//#include <stdio.h>  //mo debug
//#include "utils.h"  //mo debug

#if (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) ||\
        defined(__amd64__) || defined(__amd32__)|| defined(__amd16__)
#define bs2le(x) (x)
#define bs2be(x) (x)
#elif (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) ||\
            (defined(__sparc__))
#define bs2le(x) __builtin_bswap_wordsize(x)
#define bs2be(x) __builtin_bswap_wordsize(x)
#else
#error "endianness not supported"
#endif

word_t rands[256]  //rands has 256 words, each word need 16 bit rng or 2 byte. So in total 512 byte rng needed
/*= 
{(word_t)0x3ff3f30fffcfffc0ULL, (word_t)0xfcc3cfffffcfcc00ULL, (word_t)0xfcc330c30c0fff33ULL, (word_t)0xc3fccff033ccccc3ULL, (word_t)0xfcc0f0cf3c30c00cULL, (word_t)0xcf3300303f00300fULL, 
    (word_t)0xf0c3cf3cfc03cf0ULL, (word_t)0xc330f0fc333ff3f3ULL, (word_t)0xf3c003c3ccfff00fULL, (word_t)0x333fff30ff0033fULL, (word_t)0x3330f0f3c3f03f0fULL, (word_t)0xcc00030ff3303cc3ULL, 
    (word_t)0x3cfc03ccc033f00fULL, (word_t)0x30fc0fff0ffccf00ULL, (word_t)0x3fcfc3fcf0ffcfffULL, (word_t)0x3030ff00f0ccc3ULL, (word_t)0xcf0fccff30cc030ULL, (word_t)0x3ff3cc3cc00fc303ULL, 
    (word_t)0xcf333330cf0f03cULL, (word_t)0x3c3fff3ff0c033f3ULL, (word_t)0xff00fccf33c3c3ULL, (word_t)0xc0000f3f0c00cffcULL, (word_t)0xc0fff0c00c3f0ULL, (word_t)0xccc3ccc33c03ccfcULL, 
    (word_t)0xff0cffccff000030ULL, (word_t)0xf30c0333c3c30f3ULL, (word_t)0x3ccff3f03ff3ccf0ULL, (word_t)0xc0c3c0c00f3f0cf0ULL, (word_t)0xccfcf0c333cfcfccULL, (word_t)0xcf3fccfcfcfcccc3ULL, 
    (word_t)0x30f00c0ccff30cfULL, (word_t)0xfcc33cc00c0330cfULL, (word_t)0xf00f3333c0f33fULL, (word_t)0xf30ff0f0c3cc3c3cULL, (word_t)0x3c3c3c333c30f030ULL, (word_t)0xcff33ff3c3330cfcULL, 
    (word_t)0xf330cf0fc330c3cfULL, (word_t)0x3c00cff0cffc0ffcULL, (word_t)0xc0030003cc300cc3ULL, (word_t)0x303000c3fc3f03fcULL, (word_t)0xc3f03fff3c3033c3ULL, (word_t)0x3cff0f00fcf3c3fULL, 
    (word_t)0x30f0333c0c03c0cfULL, (word_t)0xf030f0033f03cf03ULL, (word_t)0xc03c3333cf3cff30ULL, (word_t)0xc00c033cf3cf33cfULL, (word_t)0xcf033c0fccf3f03cULL, (word_t)0xc0fc33f30f0c030fULL, 
    (word_t)0xcf3ccfcf330330f0ULL, (word_t)0xff33f0c0fff3c33ULL, (word_t)0xff03c0f3ccf30c33ULL, (word_t)0xfcffc3fffc33cfcfULL, (word_t)0xc0fcc0f3cff30f00ULL, (word_t)0xc0f3fccff3c33003ULL, 
    (word_t)0xff0c0ccff0cc0f3cULL, (word_t)0xc0c3cffcc3033fULL, (word_t)0xffccc3cfc3f03c00ULL, (word_t)0x3c0c3c00cc3c0fULL, (word_t)0x330ff3c3fff303cULL, (word_t)0xf0cccffc033f0f0fULL, 
    (word_t)0xc0cf0c3303c3f3fULL, (word_t)0x3f33fc0cf0cfcfffULL, (word_t)0xf0f0cc3fc3f00fULL, (word_t)0xc3f030f0ffc333c3ULL, (word_t)0xf03c0fc3c00ffcc3ULL, (word_t)0xcc0c0ffccc0c0fc3ULL, 
    (word_t)0x33ff00000f33fc0cULL, (word_t)0x33c000f3ff0033c3ULL, (word_t)0xcc0c0ff003c3fcc0ULL, (word_t)0xccfc0033c00f0c30ULL, (word_t)0x3030f333cf30c303ULL, (word_t)0xcc3cc3f00330c3ccULL, 
    (word_t)0xfc3fc0f0c0f0cf33ULL, (word_t)0xfcfff3ffcc3c030cULL, (word_t)0xf00303fc0fffccffULL, (word_t)0xfcf3f0c00f3fcccfULL, (word_t)0xcff3c0f0f030f0ULL, (word_t)0xf3c00c000f00033fULL, 
    (word_t)0xcc00cf3cf03fc3c3ULL, (word_t)0xfcc00cffff0f0fcULL, (word_t)0x333303cf0f0f3f3cULL, (word_t)0xff0033f00030fc33ULL, (word_t)0x3c030fc3c0303cc0ULL, (word_t)0xc33f0f00c0c330c3ULL, 
    (word_t)0x3fcc00fcc33c0c3cULL, (word_t)0xcffccf0fccc3f30cULL, (word_t)0xcc3000cc3cc33f3fULL, (word_t)0x30f3ff3f3c0c0c00ULL, (word_t)0x30f3fcccf3ff00ULL, (word_t)0xcc330c0fc3fc3c0ULL, 
    (word_t)0xccc3fcccf0cfcff3ULL, (word_t)0xcccf000030f30ffULL, (word_t)0xc33f0f0c0c3033fULL, (word_t)0xc00c00ffc3cf3ff0ULL, (word_t)0x30f03f0ff3ffcffcULL, (word_t)0xf333ffc3f3fc3f03ULL, 
    (word_t)0x33f30ff00f3f3ff3ULL, (word_t)0xff0fc00fc30f3cccULL, (word_t)0x3f3cc3cf03cccf3ULL, (word_t)0x33c0c03f303c3cf3ULL, (word_t)0xff33ff3cf3c3fc03ULL, (word_t)0xc033c3ff300fc03fULL, 
    (word_t)0xcc3cf3f3f0cc30ccULL, (word_t)0xfffc3ccffffffc33ULL, (word_t)0xfff3c3cff0f03333ULL, (word_t)0xf3cf3cfc000033f3ULL, (word_t)0x30c0303cc0f3cfULL, (word_t)0x30fcf00f30cfff30ULL, 
    (word_t)0x30cff3300f330f0cULL, (word_t)0xcc3f0c03f0f0cff3ULL, (word_t)0xc300c330ccff03cULL, (word_t)0xf0f333f3000f300cULL, (word_t)0x3fc3ffcf0000cfffULL, (word_t)0xc03f03cc33030c0ULL, 
    (word_t)0x3c3cf30fc33f0f03ULL, (word_t)0x30cffffc3f300f0ULL, (word_t)0xf0f0c0c3ccf33f0fULL, (word_t)0xfc0cf0c30c33f30cULL, (word_t)0xff0ffc33c03cfff0ULL, (word_t)0xfffc3ffc3ff00ffULL, 
    (word_t)0xfc00c3ccfccc30cfULL, (word_t)0xc033cfff0ccffULL, (word_t)0xf0cff000ff300c00ULL, (word_t)0xc0f0333cc00f0f0ULL, (word_t)0x3fc3ffff3033cc03ULL, (word_t)0xfcffff3c03fcccf3ULL, 
    (word_t)0x3f0c30ccf0f033c0ULL, (word_t)0x3fcccf00f303ffffULL, (word_t)0xc300fcfffccc3303ULL, (word_t)0x33ccfc3ff3fc000fULL, (word_t)0xccf003fc003fcfcfULL, (word_t)0x3cf0f0ccc3cc3cccULL, 
    (word_t)0x30ff30f000fff03cULL, (word_t)0xcf33f330cc30ffffULL, (word_t)0xccf3f0c03030c3c0ULL, (word_t)0x30c30ffcfcff0c30ULL, (word_t)0xccfff3fff0c033ULL, (word_t)0xf3c03ff00c033003ULL, 
    (word_t)0x3c3cffccf0f3330fULL, (word_t)0x3c0f0ffc3cf30fccULL, (word_t)0x3300300c0cff3303ULL, (word_t)0x33ffcc0ff3f0c330ULL, (word_t)0x3ff0c330ffccc00cULL, (word_t)0xff30c3333f0f0cULL, 
    (word_t)0xfccff0fff333300fULL, (word_t)0x3ccf0300303c0fccULL, (word_t)0xcf0fcfff33c03ULL, (word_t)0x3c033000fcfcc333ULL, (word_t)0xfc3ccc3cc03cULL, (word_t)0xccf03cc00f33f3ffULL, 
    (word_t)0xf00f03fc03000033ULL, (word_t)0x3c0f3fc3003fc333ULL, (word_t)0xf30fccf3fcc0f3ccULL, (word_t)0xff33ff0ccc0c003cULL, (word_t)0xff3c0fff00030fc3ULL, (word_t)0xf330f00030cf3ff3ULL, 
    (word_t)0x30ff30cf3cfc0fcfULL, (word_t)0xcfffff3f03c0ffc0ULL, (word_t)0xff30030c33fcc30cULL, (word_t)0xf300f3cc3c00fc3fULL, (word_t)0xfc03c000f0f00f33ULL, (word_t)0xc3f03cf3000fcc0fULL, 
    (word_t)0x3ff0ff0c333030ccULL, (word_t)0x3cfff300ffff0f3fULL, (word_t)0x3ff03c3cf33f3cc3ULL, (word_t)0xffcf3000fcfff3c0ULL, (word_t)0xc3300c00cf333f30ULL, (word_t)0xc0ccf0c033cc33cfULL, 
    (word_t)0xc3cfcc0cffccc0f0ULL, (word_t)0xcffc00c3c03f03fULL, (word_t)0xfccf3300033c0c3cULL, (word_t)0x3f0c0303003c3c3cULL, (word_t)0xf03fff3ccf0ccc3fULL, (word_t)0x33cc0c330f0ff0fcULL, 
    (word_t)0x330f03ff03fc3c3fULL, (word_t)0xfff30cf30f33f0fcULL, (word_t)0xc33303c03ff0ffcfULL, (word_t)0xf3fc03ff0fc3c0fULL, (word_t)0xc30300cf3ccff3c0ULL, (word_t)0x3c3f0f3f003f0fc3ULL, 
    (word_t)0xcf3c3cf3cffff0fcULL, (word_t)0xf003f3c0cf30f00ULL, (word_t)0xc3c3c300c3fcffULL, (word_t)0xfc0fc0ff03f0fcc0ULL, (word_t)0x30cc3c3c3cfff300ULL, (word_t)0x33f3ccc3fc3fc00cULL, 
    (word_t)0xfff3c0cc30333c3fULL, (word_t)0xccff0fcfc0c03cc3ULL, (word_t)0xccfc3c3f3fccffc3ULL, (word_t)0xf30cc3f33fff0330ULL, (word_t)0xc03cc330cc0003ffULL, (word_t)0x3fffffc333003033ULL, 
    (word_t)0xffcc33ff3ffcfc3cULL, (word_t)0xcf30c3ffcf0ccccULL, (word_t)0xc0ff0c3cf3c3033fULL, (word_t)0xc33f3c03fcf3cff3ULL, (word_t)0xf00f3c0f03f30c03ULL, (word_t)0xc3fc3333f0c33030ULL, 
    (word_t)0xf33f3f3fffc30000ULL, (word_t)0x3f3fff3c3ccc0f0ULL, (word_t)0x3c330c30cf0c0ULL, (word_t)0xc00ccf0ccc3000ccULL, (word_t)0xccf33cf0cc330ccfULL, (word_t)0xf03fffc03fc0ff0fULL, 
    (word_t)0x330000f0ff003030ULL, (word_t)0xfccc00ccfc3300fULL, (word_t)0x33f03f0c000030f3ULL, (word_t)0xf30000fcc3c03c3cULL, (word_t)0x30330fcc0c0c0033ULL, (word_t)0xcff00fff00cc0c3fULL, 
    (word_t)0xff3fcc33c3cfc3ffULL, (word_t)0xcff3fc3c0ccf00c3ULL, (word_t)0xc00cf033fcff30ffULL, (word_t)0xcff00c03fc3300c3ULL, (word_t)0x3c300f0fc0f30ffULL, (word_t)0xc00fcf3cf3cc3ffULL, 
    (word_t)0xfc3f33cccf030f3cULL, (word_t)0xfccc0fccfc303c33ULL, (word_t)0x3cc3300cc33300c0ULL, (word_t)0xf0f0f30c03f0c003ULL, (word_t)0xfff0003f30f00f3fULL, (word_t)0xc0f3c0cf30003fffULL, 
    (word_t)0xff0fcf30cc3c0330ULL, (word_t)0xcfccc3ccffcc03f0ULL, (word_t)0x3033cf0fcff0fcfULL, (word_t)0x3f3f00fffc3ff03cULL, (word_t)0x3fff033f0ff300ULL, (word_t)0x3cfc33f0cff30c0cULL, 
    (word_t)0xfcccf3c303ff0000ULL, (word_t)0xc33f30cfff000c30ULL, (word_t)0x3c3fcf3f0c03333fULL, (word_t)0xcc03c33cffc303fcULL, (word_t)0x3030c0f3c3fc0300ULL, (word_t)0x3cc0c0ccc3ff03c3ULL, 
    (word_t)0x3f0c3cfc0fc0fc0cULL, (word_t)0xcf3c333f3f3f0ccfULL, (word_t)0xffcffc333f03cf3fULL, (word_t)0x3c33f0cc303cf00cULL, (word_t)0xc0f03f30c303cc30ULL, (word_t)0xc3c3003c330c033ULL, 
    (word_t)0xcf3000033333f0fULL, (word_t)0xcccff0c30f303cc0ULL, (word_t)0xcc0cc303fc3cc3f0ULL, (word_t)0xfc303330c03f0ff3ULL, (word_t)0x30f03c0000c0030cULL, (word_t)0xf00cccf3333f30cfULL, 
    (word_t)0xccff3c00c330fff0ULL, (word_t)0xffcf3cf03c0f0300ULL, (word_t)0xfc0c3fc0f3cc3c30ULL, (word_t)0xcc0330000c0f3ff0ULL, (word_t)0xcc3fcf03ccf303f3ULL, (word_t)0xccff0f0f0ffc033fULL, 
}*/
;
//move m_vector to rands
//word_t rands[256];
//memmove(rands, m_vector+512, 1024);
//dump_word(rands, 256);

uint8_t bitswap[256] = {
    0x0, 0x2, 0x1, 0x3, 0x8, 0xa, 0x9, 0xb, 0x4, 0x6, 0x5, 0x7, 0xc, 0xe, 0xd, 0xf, 
    0x20, 0x22, 0x21, 0x23, 0x28, 0x2a, 0x29, 0x2b, 0x24, 0x26, 0x25, 0x27, 0x2c, 0x2e, 0x2d, 0x2f, 
    0x10, 0x12, 0x11, 0x13, 0x18, 0x1a, 0x19, 0x1b, 0x14, 0x16, 0x15, 0x17, 0x1c, 0x1e, 0x1d, 0x1f, 
    0x30, 0x32, 0x31, 0x33, 0x38, 0x3a, 0x39, 0x3b, 0x34, 0x36, 0x35, 0x37, 0x3c, 0x3e, 0x3d, 0x3f, 
    0x80, 0x82, 0x81, 0x83, 0x88, 0x8a, 0x89, 0x8b, 0x84, 0x86, 0x85, 0x87, 0x8c, 0x8e, 0x8d, 0x8f, 
    0xa0, 0xa2, 0xa1, 0xa3, 0xa8, 0xaa, 0xa9, 0xab, 0xa4, 0xa6, 0xa5, 0xa7, 0xac, 0xae, 0xad, 0xaf, 
    0x90, 0x92, 0x91, 0x93, 0x98, 0x9a, 0x99, 0x9b, 0x94, 0x96, 0x95, 0x97, 0x9c, 0x9e, 0x9d, 0x9f, 
    0xb0, 0xb2, 0xb1, 0xb3, 0xb8, 0xba, 0xb9, 0xbb, 0xb4, 0xb6, 0xb5, 0xb7, 0xbc, 0xbe, 0xbd, 0xbf, 
    0x40, 0x42, 0x41, 0x43, 0x48, 0x4a, 0x49, 0x4b, 0x44, 0x46, 0x45, 0x47, 0x4c, 0x4e, 0x4d, 0x4f, 
    0x60, 0x62, 0x61, 0x63, 0x68, 0x6a, 0x69, 0x6b, 0x64, 0x66, 0x65, 0x67, 0x6c, 0x6e, 0x6d, 0x6f, 
    0x50, 0x52, 0x51, 0x53, 0x58, 0x5a, 0x59, 0x5b, 0x54, 0x56, 0x55, 0x57, 0x5c, 0x5e, 0x5d, 0x5f, 
    0x70, 0x72, 0x71, 0x73, 0x78, 0x7a, 0x79, 0x7b, 0x74, 0x76, 0x75, 0x77, 0x7c, 0x7e, 0x7d, 0x7f, 
    0xc0, 0xc2, 0xc1, 0xc3, 0xc8, 0xca, 0xc9, 0xcb, 0xc4, 0xc6, 0xc5, 0xc7, 0xcc, 0xce, 0xcd, 0xcf, 
    0xe0, 0xe2, 0xe1, 0xe3, 0xe8, 0xea, 0xe9, 0xeb, 0xe4, 0xe6, 0xe5, 0xe7, 0xec, 0xee, 0xed, 0xef, 
    0xd0, 0xd2, 0xd1, 0xd3, 0xd8, 0xda, 0xd9, 0xdb, 0xd4, 0xd6, 0xd5, 0xd7, 0xdc, 0xde, 0xdd, 0xdf, 
    0xf0, 0xf2, 0xf1, 0xf3, 0xf8, 0xfa, 0xf9, 0xfb, 0xf4, 0xf6, 0xf5, 0xf7, 0xfc, 0xfe, 0xfd, 0xff
};


#define bs_applymask(x,y) bs_addroundkey(x,y)

void bs_addroundkey(word_t * B, word_t * rk)
{
    int i;
    for (i = 0; i < BLOCK_SIZE; i++)
        B[i] ^= rk[i];
}

void bs_apply_sbox(word_t * input)
{
    int i;
    for(i=0; i < BLOCK_SIZE; i+=8)
    {
        bs_sbox(input+i);
    }
}

void bs_apply_sbox_rev(word_t * input)
{
    int i;
    for(i=0; i < BLOCK_SIZE; i+=8)
    {
        bs_sbox_rev(input+i);
    }
}

/*July 2011*/
/*Straight-line program for AES s box*/

/*Input is U[0], U[1],...,U[7]*/
/*Output is S[0], S[1],...,S[7]*/
// http://cs-www.cs.yale.edu/homes/peralta/CircuitStuff/CMT.html
void bs_sbox_rev(word_t U[8])
{
    word_t W[8];
    word_t
        T1,T2,T3,T4,T5,T6,T8,
        T9,T10,T13,T14,T15,T16,
        T17,T18,T19,T20,T22,T23,T24,
        T25, T26, T27;

    word_t
        M1,M2,M3,M4,M5,M6,M7,M8,
        M9,M10,M11,M12,M13,M14,M15,
        M16,M17,M18,M19,M20,M21,M22,
        M23,M24,M25,M26,M27,M28,M29,
        M30,M31,M32,M33,M34,M35,M36,
        M37,M38,M39,M40,M41,M42,M43,
        M44,M45,M46,M47,M48,M49,M50,
        M51,M52,M53,M54,M55,M56,M57,
        M58,M59,M60,M61,M62,M63;

    word_t
        P0,P1,P2,P3,P4,P5,P6,P7,P8,
        P9,P10,P11,P12,P13,P14,
        P15,P16,P17,P18,P19,P20,
        P21,P22,P23,P24,P25,P26,
        P27,P28,P29;

    word_t Y5,
        R5, R13, R17, R18, R19;


    T23 = U[7] ^ U[4];
    T22 = ~(U[6] ^ U[4]);
    T2 = ~(U[7] ^ U[6]);
    T1 = U[4] ^ U[3];
    T24 = ~(U[3] ^ U[0]);
    R5 = U[1] ^ U[0];
    T8 = ~(U[6] ^ T23);
    T19 = T22 ^ R5;
    T9 = ~(U[0] ^ T1);
    T10 = T2 ^ T24;
    T13 = T2 ^ R5;
    T3 = T1 ^ R5;
    T25 = ~(U[5] ^ T1);
    R13 = U[6] ^ U[1];
    T17 = ~(U[5] ^ T19);
    T20 = T24 ^ R13;
    T4 = U[3] ^ T8;
    R17 = ~(U[5] ^ U[2]);
    R18 = ~(U[2] ^ U[1]);
    R19 = ~(U[5] ^ U[3]);
    Y5 = U[7] ^ R17;
    T6 = T22 ^ R17;
    T16 = R13 ^ R19;
    T27 = T1 ^ R18;
    T15 = T10 ^ T27;
    T14 = T10 ^ R18;
    T26 = T3 ^ T16;
    M1 = T13 & T6;
    M2 = T23 & T8;
    M3 = T14 ^ M1;
    M4 = T19 & Y5;
    M5 = M4 ^ M1;
    M6 = T3 & T16;
    M7 = T22 & T9;
    M8 = T26 ^ M6;
    M9 = T20 & T17;
    M10 = M9 ^ M6;
    M11 = T1 & T15;
    M12 = T4 & T27;
    M13 = M12 ^ M11;
    M14 = T2 & T10;
    M15 = M14 ^ M11;
    M16 = M3 ^ M2;
    M17 = M5 ^ T24;
    M18 = M8 ^ M7;
    M19 = M10 ^ M15;
    M20 = M16 ^ M13;
    M21 = M17 ^ M15;
    M22 = M18 ^ M13;
    M23 = M19 ^ T25;
    M24 = M22 ^ M23;
    M25 = M22 & M20;
    M26 = M21 ^ M25;
    M27 = M20 ^ M21;
    M28 = M23 ^ M25;
    M29 = M28 & M27;
    M30 = M26 & M24;
    M31 = M20 & M23;
    M32 = M27 & M31;
    M33 = M27 ^ M25;
    M34 = M21 & M22;
    M35 = M24 & M34;
    M36 = M24 ^ M25;
    M37 = M21 ^ M29;
    M38 = M32 ^ M33;
    M39 = M23 ^ M30;
    M40 = M35 ^ M36;
    M41 = M38 ^ M40;
    M42 = M37 ^ M39;
    M43 = M37 ^ M38;
    M44 = M39 ^ M40;
    M45 = M42 ^ M41;
    M46 = M44 & T6;
    M47 = M40 & T8;
    M48 = M39 & Y5;
    M49 = M43 & T16;
    M50 = M38 & T9;
    M51 = M37 & T17;
    M52 = M42 & T15;
    M53 = M45 & T27;
    M54 = M41 & T10;
    M55 = M44 & T13;
    M56 = M40 & T23;
    M57 = M39 & T19;
    M58 = M43 & T3;
    M59 = M38 & T22;
    M60 = M37 & T20;
    M61 = M42 & T1;
    M62 = M45 & T4;
    M63 = M41 & T2;
    P0 = M52 ^ M61;
    P1 = M58 ^ M59;
    P2 = M54 ^ M62;
    P3 = M47 ^ M50;
    P4 = M48 ^ M56;
    P5 = M46 ^ M51;
    P6 = M49 ^ M60;
    P7 = P0 ^ P1;
    P8 = M50 ^ M53;
    P9 = M55 ^ M63;
    P10 = M57 ^ P4;
    P11 = P0 ^ P3;
    P12 = M46 ^ M48;
    P13 = M49 ^ M51;
    P14 = M49 ^ M62;
    P15 = M54 ^ M59;
    P16 = M57 ^ M61;
    P17 = M58 ^ P2;
    P18 = M63 ^ P5;
    P19 = P2 ^ P3;
    P20 = P4 ^ P6;
    P22 = P2 ^ P7;
    P23 = P7 ^ P8;
    P24 = P5 ^ P7;
    P25 = P6 ^ P10;
    P26 = P9 ^ P11;
    P27 = P10 ^ P18;
    P28 = P11 ^ P25;
    P29 = P15 ^ P20;
    U[7] = P13 ^ P22;
    U[6] = P26 ^ P29;
    U[5] = P17 ^ P28;
    U[4] = P12 ^ P22;
    U[3] = P23 ^ P27;
    U[2] = P19 ^ P24;
    U[1] = P14 ^ P23;
    U[0] = P9 ^ P16;
}

volatile int rand_indx = 0;

word_t __attribute__ ((noinline)) SAND(word_t p, word_t q)
{
    word_t r1 = rands[((rand_indx++) & 0xff)];

    word_t n1 = p & q;

    word_t qswap = bitswap[(uint8_t)q]
#if WORD_SIZE > 8
                   | ((word_t)bitswap[(uint8_t)(q>>8)] << 8)
#endif
#if WORD_SIZE > 16
                   | ((word_t)bitswap[(uint8_t)(q>>16)] << 16)
                   | ((word_t)bitswap[(uint8_t)(q>>24)] << 24)
#endif
#if WORD_SIZE > 32
                   | ((word_t)bitswap[(uint8_t)(q>>32)] << 32)
                   | ((word_t)bitswap[(uint8_t)(q>>40)] << 40)
                   | ((word_t)bitswap[(uint8_t)(q>>48)] << 48)
                   | ((word_t)bitswap[(uint8_t)(q>>56)] << 56)
#endif
    ;
    word_t n3 = p & qswap;
    word_t n4 = r1 ^ n1;
    word_t z = n3 ^ n4;

    return z;
}

/*extern word_t SAND(word_t p, word_t q);
asm(
"SAND:\n"
"	save	%sp, -96, %sp	!\n"
"	sethi	%hi(rand_indx), %g1	!, tmp113\n"
"	ld	[%g1+%lo(rand_indx)], %g2	! rand_indx, rand_indx.3\n"
"	and	%g2, 255, %g3	! rand_indx.3,, tmp116\n"
"	srl	%i1, 8, %o4	! q,, tmp126\n"
"	sll	%g3, 2, %g3	! tmp116,, tmp117\n"
"	and	%o4, 0xff, %o4	! tmp126, tmp127\n"
"	srl	%i1, 24, %o5	! q,, tmp148\n"
"	and	%i1, 0xff, %o2	! q, q\n"
"	sethi	%hi(rands), %g4	!, tmp115\n"
"	or	%g4, %lo(rands), %g4	! tmp115,, tmp114\n"
"	ld	[%g4+%g3], %g4	! rands, r1\n"
"	sethi	%hi(bitswap), %g3	!, tmp125\n"
"	or	%g3, %lo(bitswap), %g3	! tmp125,, tmp124\n"
"	ldub	[%g3+%o4], %o3	! bitswap, tmp128\n"
"	ldub	[%g3+%o5], %o5	! bitswap, tmp149\n"
"	srl	%i1, 16, %o4	! q,, tmp133\n"
"	and	%o4, 0xff, %o4	! tmp133, tmp134\n"
"	ldub	[%g3+%o4], %o4	! bitswap, tmp135\n"
"	ldub	[%g3+%o2], %g3	! bitswap, tmp142\n"
"	add	%g2, 1, %g2	! rand_indx.3,, tmp120\n"
"	sll	%o3, 8, %o3	! tmp128,, tmp130\n"
"	sll	%o5, 24, %o5	! tmp149,, tmp151\n"
"	st	%g2, [%g1+%lo(rand_indx)]	! tmp120, rand_indx\n"
"	sll	%o4, 16, %o4	! tmp135,, tmp137\n"
"	or	%o3, %o4, %o4	! tmp130, tmp137, tmp138\n"
"	or	%o4, %g3, %g3	! tmp138, tmp142, tmp144\n"
"	or	%g3, %o5, %g3	! tmp144, tmp151, tmp152\n"
"	and	%g3, %i0, %g3	! tmp152, p, tmp153\n"
"	and	%i1, %i0, %i0	! q, p, tmp122\n"
"	xor	%i0, %g4, %i0	! tmp122, r1, tmp123\n"
"mov 0, %l0\n"
"	xor	%i0, %g3, %l0	! tmp123, tmp153,\n"
"mov 0, %i0\n"
"mov %l0, %i0\n"
"	jmp	%i7+8\n"
"	 restore\n"
"	.size	SAND, .-SAND\n"
"	.section	\".text.bs_sbox\",#alloc,#execinstr\n"
"	.align 4\n"
"	.global bs_sbox\n"
"	.type	bs_sbox, #function\n"
"	.proc	020\n"
);*/

void bs_sbox(word_t U[8])
{
    word_t S[8];
    word_t
        T1,T2,T3,T4,T5,T6,T7,T8,
        T9,T10,T11,T12,T13,T14,T15,T16,
        T17,T18,T19,T20,T21,T22,T23,T24,
        T25, T26, T27;


    word_t
        M1,M2,M3,M4,M5,M6,M7,M8,
        M9,M10,M11,M12,M13,M14,M15,
        M16,M17,M18,M19,M20,M21,M22,
        M23,M24,M25,M26,M27,M28,M29,
        M30,M31,M32,M33,M34,M35,M36,
        M37,M38,M39,M40,M41,M42,M43,
        M44,M45,M46,M47,M48,M49,M50,
        M51,M52,M53,M54,M55,M56,M57,
        M58,M59,M60,M61,M62,M63;

    word_t
        L0,L1,L2,L3,L4,L5,L6,L7,L8,
        L9,L10,L11,L12,L13,L14,
        L15,L16,L17,L18,L19,L20,
        L21,L22,L23,L24,L25,L26,
        L27,L28,L29;



    T1 = U[7] ^ U[4];
    T2 = U[7] ^ U[2];
    T3 = U[7] ^ U[1];
    T4 = U[4] ^ U[2];
    T5 = U[3] ^ U[1];
    T6 = T1 ^ T5;
    T7 = U[6] ^ U[5];
    T8 = U[0] ^ T6;
    T9 = U[0] ^ T7;
    T10 = T6 ^ T7;
    T11 = U[6] ^ U[2];
    T12 = U[5] ^ U[2];
    T13 = T3 ^ T4;
    T14 = T6 ^ T11;
    T15 = T5 ^ T11;
    T16 = T5 ^ T12;
    T17 = T9 ^ T16;
    T18 = U[4] ^ U[0];
    T19 = T7 ^ T18;
    T20 = T1 ^ T19;
    T21 = U[1] ^ U[0];
    T22 = T7 ^ T21;
    T23 = T2 ^ T22;
    T24 = T2 ^ T10;
    T25 = T20 ^ T17;
    T26 = T3 ^ T16;
    T27 = T1 ^ T12;

    /*M1 = T13 & T6;*/
    M1 = SAND(T13,T6);

    /*M2 = T23 & T8;*/
    M2 = SAND(T23,T8);

    M3 = T14 ^ M1;

    /*M4 = T19 & U[0];*/
    M4 = SAND(T19,U[0]);

    M5 = M4 ^ M1;

    /*M6 = T3 & T16;*/
    M6 = SAND(T3,T16);
    /*M7 = T22 & T9;*/
    M7 = SAND(T22,T9);
    
    M8 = T26 ^ M6;

    /*M9 = T20 & T17;*/
    M9 = SAND(T20,T17);

    M10 = M9 ^ M6;

    /*M11 = T1 & T15;*/
    M11 = SAND(T1,T15);
    /*M12 = T4 & T27;*/
    M12 = SAND(T4,T27);

    M13 = M12 ^ M11;

    /*M14 = T2 & T10;*/
    M14 = SAND(T2,T10);

    M15 = M14 ^ M11;
    M16 = M3 ^ M2;
    M17 = M5 ^ T24;
    M18 = M8 ^ M7;
    M19 = M10 ^ M15;
    M20 = M16 ^ M13;
    M21 = M17 ^ M15;
    M22 = M18 ^ M13;
    M23 = M19 ^ T25;
    M24 = M22 ^ M23;

    /*M25 = M22 & M20;*/
    M25 = SAND(M22,M20);
    
    M26 = M21 ^ M25;
    M27 = M20 ^ M21;
    M28 = M23 ^ M25;

    /*M29 = M28 & M27;*/
    M29 = SAND(M28,M27);
    /*M30 = M26 & M24;*/
    M30 = SAND(M26,M24);
    /*M31 = M20 & M23;*/
    M31 = SAND(M20,M23);
    /*M32 = M27 & M31;*/
    M32 = SAND(M27,M31);

    M33 = M27 ^ M25;

    /*M34 = M21 & M22;*/
    M34 = SAND(M21,M22);
    /*M35 = M24 & M34;*/
    M35 = SAND(M24,M34);

    M36 = M24 ^ M25;
    M37 = M21 ^ M29;
    M38 = M32 ^ M33;
    M39 = M23 ^ M30;
    M40 = M35 ^ M36;
    M41 = M38 ^ M40;
    M42 = M37 ^ M39;
    M43 = M37 ^ M38;
    M44 = M39 ^ M40;
    M45 = M42 ^ M41;

    /*M46 = M44 & T6;*/
    M46 = SAND(M44,T6);
    /*M47 = M40 & T8;*/
    M47 = SAND(M40,T8);
    /*M48 = M39 & U[0];*/
    M48 = SAND(M39,U[0]);
    /*M49 = M43 & T16;*/
    M49 = SAND(M43,T16);
    /*M50 = M38 & T9;*/
    M50 = SAND(M38,T9);
    /*M51 = M37 & T17;*/
    M51 = SAND(M37,T17);
    /*M52 = M42 & T15;*/
    M52 = SAND(M42,T15);
    /*M53 = M45 & T27;*/
    M53 = SAND(M45,T27);
    /*M54 = M41 & T10;*/
    M54 = SAND(M41,T10);
    /*M55 = M44 & T13;*/
    M55 = SAND(M44,T13);
    /*M56 = M40 & T23;*/
    M56 = SAND(M40,T23);
    /*M57 = M39 & T19;*/
    M57 = SAND(M39,T19);
    /*M58 = M43 & T3;*/
    M58 = SAND(M43,T3);
    /*M59 = M38 & T22;*/
    M59 = SAND(M38,T22);
    /*M60 = M37 & T20;*/
    M60 = SAND(M37,T20);
    /*M61 = M42 & T1;*/
    M61 = SAND(M42,T1);
    /*M62 = M45 & T4;*/
    M62 = SAND(M45,T4);
    /*M63 = M41 & T2;*/
    M63 = SAND(M41,T2);

    L0 = M61 ^ M62;
    L1 = M50 ^ M56;
    L2 = M46 ^ M48;
    L3 = M47 ^ M55;
    L4 = M54 ^ M58;
    L5 = M49 ^ M61;
    L6 = M62 ^ L5;
    L7 = M46 ^ L3;
    L8 = M51 ^ M59;
    L9 = M52 ^ M53;
    L10 = M53 ^ L4;
    L11 = M60 ^ L2;
    L12 = M48 ^ M51;
    L13 = M50 ^ L0;
    L14 = M52 ^ M61;
    L15 = M55 ^ L1;
    L16 = M56 ^ L0;
    L17 = M57 ^ L1;
    L18 = M58 ^ L8;
    L19 = M63 ^ L4;
    L20 = L0 ^ L1;
    L21 = L1 ^ L7;
    L22 = L3 ^ L12;
    L23 = L18 ^ L2;
    L24 = L15 ^ L9;
    L25 = L6 ^ L10;
    L26 = L7 ^ L9;
    L27 = L8 ^ L10;
    L28 = L11 ^ L14;
    L29 = L11 ^ L17;
    U[7] = L6 ^ L24;
    /*U[6] = ~(L16 ^ L26);*/
    U[6] = 0x55555555 ^ (L16 ^ L26);
    /*U[5] = ~(L19 ^ L28);*/
    U[5] = 0x55555555 ^ (L19 ^ L28);
    U[4] = L6 ^ L21;
    U[3] = L20 ^ L22;
    U[2] = L25 ^ L29;
    /*U[1] = ~(L13 ^ L27);*/
    U[1] = 0x55555555 ^ (L13 ^ L27);
    /*U[0] = ~(L6 ^ L23);*/
    U[0] = 0x55555555 ^ (L6 ^ L23);
}

void bs_transpose(word_t * blocks)
{
    word_t transpose[BLOCK_SIZE];
    memset(transpose, 0, sizeof(transpose));
    bs_transpose_dst(transpose,blocks);
    memmove(blocks,transpose,sizeof(transpose));
}

void bs_transpose_dst(word_t * transpose, word_t * blocks)
{
    int i,k;
    word_t w;
    for(k=0; k < WORD_SIZE; k++)
    {
        int bitpos = ONE << k;
        for (i=0; i < WORDS_PER_BLOCK; i++)
        {
            w = bs2le(blocks[k * WORDS_PER_BLOCK + i]);
            int offset = i << MUL_SHIFT;

#ifndef UNROLL_TRANSPOSE
            int j;
            for(j=0; j < WORD_SIZE; j++)
            {
                // TODO make const time
                transpose[offset + j] |= (w & (ONE << j)) ? bitpos : 0;
            }
#else

            transpose[(offset)+ 0 ] |= (w & (ONE << 0 )) ? (bitpos) : 0;
            transpose[(offset)+ 1 ] |= (w & (ONE << 1 )) ? (bitpos) : 0;
            transpose[(offset)+ 2 ] |= (w & (ONE << 2 )) ? (bitpos) : 0;
            transpose[(offset)+ 3 ] |= (w & (ONE << 3 )) ? (bitpos) : 0;
            transpose[(offset)+ 4 ] |= (w & (ONE << 4 )) ? (bitpos) : 0;
            transpose[(offset)+ 5 ] |= (w & (ONE << 5 )) ? (bitpos) : 0;
            transpose[(offset)+ 6 ] |= (w & (ONE << 6 )) ? (bitpos) : 0;
            transpose[(offset)+ 7 ] |= (w & (ONE << 7 )) ? (bitpos) : 0;
#if WORD_SIZE > 8
            transpose[(offset)+ 8 ] |= (w & (ONE << 8 )) ? (bitpos) : 0;
            transpose[(offset)+ 9 ] |= (w & (ONE << 9 )) ? (bitpos) : 0;
            transpose[(offset)+ 10] |= (w & (ONE << 10)) ? (bitpos) : 0;
            transpose[(offset)+ 11] |= (w & (ONE << 11)) ? (bitpos) : 0;
            transpose[(offset)+ 12] |= (w & (ONE << 12)) ? (bitpos) : 0;
            transpose[(offset)+ 13] |= (w & (ONE << 13)) ? (bitpos) : 0;
            transpose[(offset)+ 14] |= (w & (ONE << 14)) ? (bitpos) : 0;
            transpose[(offset)+ 15] |= (w & (ONE << 15)) ? (bitpos) : 0;
#endif
#if WORD_SIZE > 16
            transpose[(offset)+ 16] |= (w & (ONE << 16)) ? (bitpos) : 0;
            transpose[(offset)+ 17] |= (w & (ONE << 17)) ? (bitpos) : 0;
            transpose[(offset)+ 18] |= (w & (ONE << 18)) ? (bitpos) : 0;
            transpose[(offset)+ 19] |= (w & (ONE << 19)) ? (bitpos) : 0;
            transpose[(offset)+ 20] |= (w & (ONE << 20)) ? (bitpos) : 0;
            transpose[(offset)+ 21] |= (w & (ONE << 21)) ? (bitpos) : 0;
            transpose[(offset)+ 22] |= (w & (ONE << 22)) ? (bitpos) : 0;
            transpose[(offset)+ 23] |= (w & (ONE << 23)) ? (bitpos) : 0;
            transpose[(offset)+ 24] |= (w & (ONE << 24)) ? (bitpos) : 0;
            transpose[(offset)+ 25] |= (w & (ONE << 25)) ? (bitpos) : 0;
            transpose[(offset)+ 26] |= (w & (ONE << 26)) ? (bitpos) : 0;
            transpose[(offset)+ 27] |= (w & (ONE << 27)) ? (bitpos) : 0;
            transpose[(offset)+ 28] |= (w & (ONE << 28)) ? (bitpos) : 0;
            transpose[(offset)+ 29] |= (w & (ONE << 29)) ? (bitpos) : 0;
            transpose[(offset)+ 30] |= (w & (ONE << 30)) ? (bitpos) : 0;
            transpose[(offset)+ 31] |= (w & (ONE << 31)) ? (bitpos) : 0;
#endif
#if WORD_SIZE > 32
            transpose[(offset)+ 32] |= (w & (ONE << 32)) ? (bitpos) : 0;
            transpose[(offset)+ 33] |= (w & (ONE << 33)) ? (bitpos) : 0;
            transpose[(offset)+ 34] |= (w & (ONE << 34)) ? (bitpos) : 0;
            transpose[(offset)+ 35] |= (w & (ONE << 35)) ? (bitpos) : 0;
            transpose[(offset)+ 36] |= (w & (ONE << 36)) ? (bitpos) : 0;
            transpose[(offset)+ 37] |= (w & (ONE << 37)) ? (bitpos) : 0;
            transpose[(offset)+ 38] |= (w & (ONE << 38)) ? (bitpos) : 0;
            transpose[(offset)+ 39] |= (w & (ONE << 39)) ? (bitpos) : 0;
            transpose[(offset)+ 40] |= (w & (ONE << 40)) ? (bitpos) : 0;
            transpose[(offset)+ 41] |= (w & (ONE << 41)) ? (bitpos) : 0;
            transpose[(offset)+ 42] |= (w & (ONE << 42)) ? (bitpos) : 0;
            transpose[(offset)+ 43] |= (w & (ONE << 43)) ? (bitpos) : 0;
            transpose[(offset)+ 44] |= (w & (ONE << 44)) ? (bitpos) : 0;
            transpose[(offset)+ 45] |= (w & (ONE << 45)) ? (bitpos) : 0;
            transpose[(offset)+ 46] |= (w & (ONE << 46)) ? (bitpos) : 0;
            transpose[(offset)+ 47] |= (w & (ONE << 47)) ? (bitpos) : 0;
            transpose[(offset)+ 48] |= (w & (ONE << 48)) ? (bitpos) : 0;
            transpose[(offset)+ 49] |= (w & (ONE << 49)) ? (bitpos) : 0;
            transpose[(offset)+ 50] |= (w & (ONE << 50)) ? (bitpos) : 0;
            transpose[(offset)+ 51] |= (w & (ONE << 51)) ? (bitpos) : 0;
            transpose[(offset)+ 52] |= (w & (ONE << 52)) ? (bitpos) : 0;
            transpose[(offset)+ 53] |= (w & (ONE << 53)) ? (bitpos) : 0;
            transpose[(offset)+ 54] |= (w & (ONE << 54)) ? (bitpos) : 0;
            transpose[(offset)+ 55] |= (w & (ONE << 55)) ? (bitpos) : 0;
            transpose[(offset)+ 56] |= (w & (ONE << 56)) ? (bitpos) : 0;
            transpose[(offset)+ 57] |= (w & (ONE << 57)) ? (bitpos) : 0;
            transpose[(offset)+ 58] |= (w & (ONE << 58)) ? (bitpos) : 0;
            transpose[(offset)+ 59] |= (w & (ONE << 59)) ? (bitpos) : 0;
            transpose[(offset)+ 60] |= (w & (ONE << 60)) ? (bitpos) : 0;
            transpose[(offset)+ 61] |= (w & (ONE << 61)) ? (bitpos) : 0;
            transpose[(offset)+ 62] |= (w & (ONE << 62)) ? (bitpos) : 0;
            transpose[(offset)+ 63] |= (w & (ONE << 63)) ? (bitpos) : 0;
#endif
#endif
                // constant time:
                //transpose[(i<<MUL_SHIFT)+ j] |= (((int64_t)((w & (ONE << j)) << (WORD_SIZE-1-j)))>>(WORD_SIZE-1)) & (ONE<<k);
        }
    }
}

void bs_transpose_rev(word_t * blocks)
{
    int i,k;
    word_t w;
    word_t transpose[BLOCK_SIZE];
    memset(transpose, 0, sizeof(transpose));
    for(k=0; k < BLOCK_SIZE; k++)
    {
        w = blocks[k];
        word_t bitpos = bs2be(ONE << (k % WORD_SIZE));
        word_t offset = k / WORD_SIZE;
#ifndef UNROLL_TRANSPOSE
        int j;
        for(j=0; j < WORD_SIZE; j++)
        {
            word_t bit = (w & (ONE << j)) ? (ONE << (k % WORD_SIZE)) : 0;
            transpose[j * WORDS_PER_BLOCK + (offset)] |= bit;
        }
#else
        transpose[0  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 0 )) ? bitpos : 0;
        transpose[1  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 1 )) ? bitpos : 0;
        transpose[2  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 2 )) ? bitpos : 0;
        transpose[3  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 3 )) ? bitpos : 0;
        transpose[4  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 4 )) ? bitpos : 0;
        transpose[5  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 5 )) ? bitpos : 0;
        transpose[6  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 6 )) ? bitpos : 0;
        transpose[7  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 7 )) ? bitpos : 0;
#if WORD_SIZE > 8
        transpose[8  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 8 )) ? bitpos : 0;
        transpose[9  * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 9 )) ? bitpos : 0;
        transpose[10 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 10)) ? bitpos : 0;
        transpose[11 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 11)) ? bitpos : 0;
        transpose[12 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 12)) ? bitpos : 0;
        transpose[13 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 13)) ? bitpos : 0;
        transpose[14 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 14)) ? bitpos : 0;
        transpose[15 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 15)) ? bitpos : 0;
#endif
#if WORD_SIZE > 16
        transpose[16 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 16)) ? bitpos : 0;
        transpose[17 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 17)) ? bitpos : 0;
        transpose[18 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 18)) ? bitpos : 0;
        transpose[19 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 19)) ? bitpos : 0;
        transpose[20 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 20)) ? bitpos : 0;
        transpose[21 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 21)) ? bitpos : 0;
        transpose[22 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 22)) ? bitpos : 0;
        transpose[23 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 23)) ? bitpos : 0;
        transpose[24 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 24)) ? bitpos : 0;
        transpose[25 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 25)) ? bitpos : 0;
        transpose[26 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 26)) ? bitpos : 0;
        transpose[27 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 27)) ? bitpos : 0;
        transpose[28 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 28)) ? bitpos : 0;
        transpose[29 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 29)) ? bitpos : 0;
        transpose[30 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 30)) ? bitpos : 0;
        transpose[31 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 31)) ? bitpos : 0;
#endif
#if WORD_SIZE > 32
        transpose[32 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 32)) ? bitpos : 0;
        transpose[33 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 33)) ? bitpos : 0;
        transpose[34 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 34)) ? bitpos : 0;
        transpose[35 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 35)) ? bitpos : 0;
        transpose[36 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 36)) ? bitpos : 0;
        transpose[37 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 37)) ? bitpos : 0;
        transpose[38 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 38)) ? bitpos : 0;
        transpose[39 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 39)) ? bitpos : 0;
        transpose[40 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 40)) ? bitpos : 0;
        transpose[41 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 41)) ? bitpos : 0;
        transpose[42 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 42)) ? bitpos : 0;
        transpose[43 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 43)) ? bitpos : 0;
        transpose[44 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 44)) ? bitpos : 0;
        transpose[45 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 45)) ? bitpos : 0;
        transpose[46 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 46)) ? bitpos : 0;
        transpose[47 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 47)) ? bitpos : 0;
        transpose[48 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 48)) ? bitpos : 0;
        transpose[49 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 49)) ? bitpos : 0;
        transpose[50 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 50)) ? bitpos : 0;
        transpose[51 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 51)) ? bitpos : 0;
        transpose[52 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 52)) ? bitpos : 0;
        transpose[53 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 53)) ? bitpos : 0;
        transpose[54 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 54)) ? bitpos : 0;
        transpose[55 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 55)) ? bitpos : 0;
        transpose[56 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 56)) ? bitpos : 0;
        transpose[57 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 57)) ? bitpos : 0;
        transpose[58 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 58)) ? bitpos : 0;
        transpose[59 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 59)) ? bitpos : 0;
        transpose[60 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 60)) ? bitpos : 0;
        transpose[61 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 61)) ? bitpos : 0;
        transpose[62 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 62)) ? bitpos : 0;
        transpose[63 * WORDS_PER_BLOCK + (offset )] |= (w & (ONE << 63)) ? bitpos : 0;
#endif
#endif
    }
    memmove(blocks,transpose,sizeof(transpose));
}


#define R0          0
#define R1          8
#define R2          16
#define R3          24

#define B0          0
#define B1          32
#define B2          64
#define B3          96

#define R0_shift        (BLOCK_SIZE/4)*0
#define R1_shift        (BLOCK_SIZE/4)*1
#define R2_shift        (BLOCK_SIZE/4)*2
#define R3_shift        (BLOCK_SIZE/4)*3
#define B_MOD           (BLOCK_SIZE)


void bs_shiftrows(word_t * B)
{
    word_t Bp_space[BLOCK_SIZE];
    word_t * Bp = Bp_space;
    word_t * Br0 = B + 0;
    word_t * Br1 = B + 32;
    word_t * Br2 = B + 64;
    word_t * Br3 = B + 96;
    uint8_t offsetr0 = 0;
    uint8_t offsetr1 = 32;
    uint8_t offsetr2 = 64;
    uint8_t offsetr3 = 96;


    int i;
    for(i=0; i<4; i++)
    {
        Bp[B0 + 0] = Br0[0];
        Bp[B0 + 1] = Br0[1];
        Bp[B0 + 2] = Br0[2];
        Bp[B0 + 3] = Br0[3];
        Bp[B0 + 4] = Br0[4];
        Bp[B0 + 5] = Br0[5];
        Bp[B0 + 6] = Br0[6];
        Bp[B0 + 7] = Br0[7];
        Bp[B1 + 0] = Br1[0];
        Bp[B1 + 1] = Br1[1];
        Bp[B1 + 2] = Br1[2];
        Bp[B1 + 3] = Br1[3];
        Bp[B1 + 4] = Br1[4];
        Bp[B1 + 5] = Br1[5];
        Bp[B1 + 6] = Br1[6];
        Bp[B1 + 7] = Br1[7];
        Bp[B2 + 0] = Br2[0];
        Bp[B2 + 1] = Br2[1];
        Bp[B2 + 2] = Br2[2];
        Bp[B2 + 3] = Br2[3];
        Bp[B2 + 4] = Br2[4];
        Bp[B2 + 5] = Br2[5];
        Bp[B2 + 6] = Br2[6];
        Bp[B2 + 7] = Br2[7];
        Bp[B3 + 0] = Br3[0];
        Bp[B3 + 1] = Br3[1];
        Bp[B3 + 2] = Br3[2];
        Bp[B3 + 3] = Br3[3];
        Bp[B3 + 4] = Br3[4];
        Bp[B3 + 5] = Br3[5];
        Bp[B3 + 6] = Br3[6];
        Bp[B3 + 7] = Br3[7];

        offsetr0 = (offsetr0 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;
        offsetr1 = (offsetr1 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;
        offsetr2 = (offsetr2 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;
        offsetr3 = (offsetr3 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;

        Br0 = B + offsetr0;
        Br1 = B + offsetr1;
        Br2 = B + offsetr2;
        Br3 = B + offsetr3;

        Bp += 8;
    }
    memmove(B,Bp_space,sizeof(Bp_space));
}


void bs_shiftrows_rev(word_t * B)
{
    word_t Bp_space[BLOCK_SIZE];
    word_t * Bp = Bp_space;
    word_t * Br0 = Bp + 0;
    word_t * Br1 = Bp + 32;
    word_t * Br2 = Bp + 64;
    word_t * Br3 = Bp + 96;
    uint8_t offsetr0 = 0;
    uint8_t offsetr1 = 32;
    uint8_t offsetr2 = 64;
    uint8_t offsetr3 = 96;


    int i;
    for(i=0; i<4; i++)
    {
        Br0[0] = B[B0 + 0];
        Br0[1] = B[B0 + 1];
        Br0[2] = B[B0 + 2];
        Br0[3] = B[B0 + 3];
        Br0[4] = B[B0 + 4];
        Br0[5] = B[B0 + 5];
        Br0[6] = B[B0 + 6];
        Br0[7] = B[B0 + 7];
        Br1[0] = B[B1 + 0];
        Br1[1] = B[B1 + 1];
        Br1[2] = B[B1 + 2];
        Br1[3] = B[B1 + 3];
        Br1[4] = B[B1 + 4];
        Br1[5] = B[B1 + 5];
        Br1[6] = B[B1 + 6];
        Br1[7] = B[B1 + 7];
        Br2[0] = B[B2 + 0];
        Br2[1] = B[B2 + 1];
        Br2[2] = B[B2 + 2];
        Br2[3] = B[B2 + 3];
        Br2[4] = B[B2 + 4];
        Br2[5] = B[B2 + 5];
        Br2[6] = B[B2 + 6];
        Br2[7] = B[B2 + 7];
        Br3[0] = B[B3 + 0];
        Br3[1] = B[B3 + 1];
        Br3[2] = B[B3 + 2];
        Br3[3] = B[B3 + 3];
        Br3[4] = B[B3 + 4];
        Br3[5] = B[B3 + 5];
        Br3[6] = B[B3 + 6];
        Br3[7] = B[B3 + 7];

        offsetr0 = (offsetr0 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;
        offsetr1 = (offsetr1 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;
        offsetr2 = (offsetr2 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;
        offsetr3 = (offsetr3 + BLOCK_SIZE/16 + BLOCK_SIZE/4) & 0x7f;

        Br0 = Bp + offsetr0;
        Br1 = Bp + offsetr1;
        Br2 = Bp + offsetr2;
        Br3 = Bp + offsetr3;

        B += 8;
    }
    memmove(B - 8 * 4,Bp_space,sizeof(Bp_space));
}


#define A0  0
#define A1  8
#define A2  16
#define A3  24

// Does shift rows and mix columns in same step
void bs_shiftmix(word_t * B)
{
    word_t Bp_space[BLOCK_SIZE];
    word_t * Bp = Bp_space;

    word_t * Br0 = B + 0;
    word_t * Br1 = B + 32;
    word_t * Br2 = B + 64;
    word_t * Br3 = B + 96;

    uint8_t offsetr0 = 0;
    uint8_t offsetr1 = 32;
    uint8_t offsetr2 = 64;
    uint8_t offsetr3 = 96;

        Br0 = B + offsetr0;
        Br1 = B + offsetr1;
        Br2 = B + offsetr2;
        Br3 = B + offsetr3;


    int i;
    for (i = 0; i < 4; i++)
    {
        // B0
        //            2*A0        2*A1              A1           A2           A3
        word_t of =Br0[R0+7]^ Br1[R1+7];
        Bp[A0+0] =                         Br1[R1+0] ^ Br2[R2+0] ^ Br3[R3+0] ^ of;
        Bp[A0+1] = Br0[R0+0] ^ Br1[R1+0] ^ Br1[R1+1] ^ Br2[R2+1] ^ Br3[R3+1] ^ of;
        Bp[A0+2] = Br0[R0+1] ^ Br1[R1+1] ^ Br1[R1+2] ^ Br2[R2+2] ^ Br3[R3+2];
        Bp[A0+3] = Br0[R0+2] ^ Br1[R1+2] ^ Br1[R1+3] ^ Br2[R2+3] ^ Br3[R3+3] ^ of;
        Bp[A0+4] = Br0[R0+3] ^ Br1[R1+3] ^ Br1[R1+4] ^ Br2[R2+4] ^ Br3[R3+4] ^ of;
        Bp[A0+5] = Br0[R0+4] ^ Br1[R1+4] ^ Br1[R1+5] ^ Br2[R2+5] ^ Br3[R3+5];
        Bp[A0+6] = Br0[R0+5] ^ Br1[R1+5] ^ Br1[R1+6] ^ Br2[R2+6] ^ Br3[R3+6];
        Bp[A0+7] = Br0[R0+6] ^ Br1[R1+6] ^ Br1[R1+7] ^ Br2[R2+7] ^ Br3[R3+7];

        //            A0            2*A1        2*A2        A2       A3
        of = Br1[R1+7] ^ Br2[R2+7];
        Bp[A1+0] = Br0[R0+0]                         ^ Br2[R2+0] ^ Br3[R3+0] ^ of;
        Bp[A1+1] = Br0[R0+1] ^ Br1[R1+0] ^ Br2[R2+0] ^ Br2[R2+1] ^ Br3[R3+1] ^ of;
        Bp[A1+2] = Br0[R0+2] ^ Br1[R1+1] ^ Br2[R2+1] ^ Br2[R2+2] ^ Br3[R3+2];
        Bp[A1+3] = Br0[R0+3] ^ Br1[R1+2] ^ Br2[R2+2] ^ Br2[R2+3] ^ Br3[R3+3] ^ of;
        Bp[A1+4] = Br0[R0+4] ^ Br1[R1+3] ^ Br2[R2+3] ^ Br2[R2+4] ^ Br3[R3+4] ^ of;
        Bp[A1+5] = Br0[R0+5] ^ Br1[R1+4] ^ Br2[R2+4] ^ Br2[R2+5] ^ Br3[R3+5];
        Bp[A1+6] = Br0[R0+6] ^ Br1[R1+5] ^ Br2[R2+5] ^ Br2[R2+6] ^ Br3[R3+6];
        Bp[A1+7] = Br0[R0+7] ^ Br1[R1+6] ^ Br2[R2+6] ^ Br2[R2+7] ^ Br3[R3+7];

        //            A0             A1      2*A2        2*A3         A3
        of = Br2[R2+7] ^ Br3[R3+7];
        Bp[A2+0] = Br0[R0+0] ^ Br1[R1+0]                         ^ Br3[R3+0] ^ of;
        Bp[A2+1] = Br0[R0+1] ^ Br1[R1+1] ^ Br2[R2+0] ^ Br3[R3+0] ^ Br3[R3+1] ^ of;
        Bp[A2+2] = Br0[R0+2] ^ Br1[R1+2] ^ Br2[R2+1] ^ Br3[R3+1] ^ Br3[R3+2];
        Bp[A2+3] = Br0[R0+3] ^ Br1[R1+3] ^ Br2[R2+2] ^ Br3[R3+2] ^ Br3[R3+3] ^ of;
        Bp[A2+4] = Br0[R0+4] ^ Br1[R1+4] ^ Br2[R2+3] ^ Br3[R3+3] ^ Br3[R3+4] ^ of;
        Bp[A2+5] = Br0[R0+5] ^ Br1[R1+5] ^ Br2[R2+4] ^ Br3[R3+4] ^ Br3[R3+5];
        Bp[A2+6] = Br0[R0+6] ^ Br1[R1+6] ^ Br2[R2+5] ^ Br3[R3+5] ^ Br3[R3+6];
        Bp[A2+7] = Br0[R0+7] ^ Br1[R1+7] ^ Br2[R2+6] ^ Br3[R3+6] ^ Br3[R3+7];

        //             A0          2*A0           A1       A2      2*A3
        of = Br0[R0+7] ^ Br3[R3+7];
        Bp[A3+0] = Br0[R0+0] ^             Br1[R1+0] ^ Br2[R2+0]             ^ of;
        Bp[A3+1] = Br0[R0+1] ^ Br0[R0+0] ^ Br1[R1+1] ^ Br2[R2+1] ^ Br3[R3+0] ^ of;
        Bp[A3+2] = Br0[R0+2] ^ Br0[R0+1] ^ Br1[R1+2] ^ Br2[R2+2] ^ Br3[R3+1];
        Bp[A3+3] = Br0[R0+3] ^ Br0[R0+2] ^ Br1[R1+3] ^ Br2[R2+3] ^ Br3[R3+2] ^ of;
        Bp[A3+4] = Br0[R0+4] ^ Br0[R0+3] ^ Br1[R1+4] ^ Br2[R2+4] ^ Br3[R3+3] ^ of;
        Bp[A3+5] = Br0[R0+5] ^ Br0[R0+4] ^ Br1[R1+5] ^ Br2[R2+5] ^ Br3[R3+4];
        Bp[A3+6] = Br0[R0+6] ^ Br0[R0+5] ^ Br1[R1+6] ^ Br2[R2+6] ^ Br3[R3+5];
        Bp[A3+7] = Br0[R0+7] ^ Br0[R0+6] ^ Br1[R1+7] ^ Br2[R2+7] ^ Br3[R3+6];

        Bp += BLOCK_SIZE/4;

        offsetr0 = (offsetr0 + BLOCK_SIZE/4) & 0x7f;
        offsetr1 = (offsetr1 + BLOCK_SIZE/4) & 0x7f;
        offsetr2 = (offsetr2 + BLOCK_SIZE/4) & 0x7f;
        offsetr3 = (offsetr3 + BLOCK_SIZE/4) & 0x7f;

        Br0 = B + offsetr0;
        Br1 = B + offsetr1;
        Br2 = B + offsetr2;
        Br3 = B + offsetr3;
    }

    memmove(B,Bp_space,sizeof(Bp_space));
}



void bs_mixcolumns(word_t * B)
{
    word_t Bp_space[BLOCK_SIZE];
    word_t * Bp = Bp_space;
    // to understand this, see
    // https://en.wikipedia.org/wiki/Rijndael_mix_columns
    
    int i = 0;
    for (; i < 4; i++)
    {
    //  of = A0 ^ A1;
    //  A0 = A0 ^ (0x1b & ((signed char)of>>7));

    //// 2 * A0
    //  A0 = A0 ^ (A0 << 1)

    //// + 3 * A1
    //  A0 = A0 ^ (A1)
    //  A0 = A0 ^ (A1<<1)

    //// + A2 + A3
    //  A0 = A0 ^ (A2)
    //  A0 = A0 ^ (A3)
    //          A0.7    A1.7
    word_t of = B[A0+7] ^ B[A1+7];

    //          2*A0     2*A1          A1      A2           A3
    Bp[A0+0] =                     B[A1+0] ^ B[A2+0] ^ B[A3+0] ^ of;
    Bp[A0+1] = B[A0+0] ^ B[A1+0] ^ B[A1+1] ^ B[A2+1] ^ B[A3+1] ^ of;
    Bp[A0+2] = B[A0+1] ^ B[A1+1] ^ B[A1+2] ^ B[A2+2] ^ B[A3+2];
    Bp[A0+3] = B[A0+2] ^ B[A1+2] ^ B[A1+3] ^ B[A2+3] ^ B[A3+3] ^ of;
    Bp[A0+4] = B[A0+3] ^ B[A1+3] ^ B[A1+4] ^ B[A2+4] ^ B[A3+4] ^ of;
    Bp[A0+5] = B[A0+4] ^ B[A1+4] ^ B[A1+5] ^ B[A2+5] ^ B[A3+5];
    Bp[A0+6] = B[A0+5] ^ B[A1+5] ^ B[A1+6] ^ B[A2+6] ^ B[A3+6];
    Bp[A0+7] = B[A0+6] ^ B[A1+6] ^ B[A1+7] ^ B[A2+7] ^ B[A3+7];



    //  of = A1 ^ A2
    //  A1 = A1 ^ (0x1b & ((signed char)of>>7));

    //// A0
    //  A1 = A1 ^ (A0)

    //// + 2 * A1
    //  A1 = A1 ^ (A1 << 1)

    //// + 3 * A2
    //  A1 = A1 ^ (A2)
    //  A1 = A1 ^ (A2<<1)

    //// + A3
    //  A1 = A1 ^ (A3)

    of = B[A1+7] ^ B[A2+7];

    //          A0      2*A1        2*A2      A2        A3
    Bp[A1+0] = B[A0+0]                     ^ B[A2+0] ^ B[A3+0] ^ of;
    Bp[A1+1] = B[A0+1] ^ B[A1+0] ^ B[A2+0] ^ B[A2+1] ^ B[A3+1] ^ of;
    Bp[A1+2] = B[A0+2] ^ B[A1+1] ^ B[A2+1] ^ B[A2+2] ^ B[A3+2];
    Bp[A1+3] = B[A0+3] ^ B[A1+2] ^ B[A2+2] ^ B[A2+3] ^ B[A3+3] ^ of;
    Bp[A1+4] = B[A0+4] ^ B[A1+3] ^ B[A2+3] ^ B[A2+4] ^ B[A3+4] ^ of;
    Bp[A1+5] = B[A0+5] ^ B[A1+4] ^ B[A2+4] ^ B[A2+5] ^ B[A3+5];
    Bp[A1+6] = B[A0+6] ^ B[A1+5] ^ B[A2+5] ^ B[A2+6] ^ B[A3+6];
    Bp[A1+7] = B[A0+7] ^ B[A1+6] ^ B[A2+6] ^ B[A2+7] ^ B[A3+7];
    

    //  of = A2 ^ A3
    //  A2 = A2 ^ (0x1b & ((signed char)of>>7));

    //// A0 + A1
    //  A2 = A2 ^ (A0)
    //  A2 = A2 ^ (A1)

    //// + 2 * A2
    //  A2 = A2 ^ (A2 << 1)

    //// + 3 * A3
    //  A2 = A2 ^ (A3)
    //  A2 = A2 ^ (A3<<1)


    of = B[A2+7] ^ B[A3+7];

    //          A0      A1          2*A2       2*A3         A3
    Bp[A2+0] = B[A0+0] ^ B[A1+0]                     ^ B[A3+0] ^ of;
    Bp[A2+1] = B[A0+1] ^ B[A1+1] ^ B[A2+0] ^ B[A3+0] ^ B[A3+1] ^ of;
    Bp[A2+2] = B[A0+2] ^ B[A1+2] ^ B[A2+1] ^ B[A3+1] ^ B[A3+2];
    Bp[A2+3] = B[A0+3] ^ B[A1+3] ^ B[A2+2] ^ B[A3+2] ^ B[A3+3] ^ of;
    Bp[A2+4] = B[A0+4] ^ B[A1+4] ^ B[A2+3] ^ B[A3+3] ^ B[A3+4] ^ of;
    Bp[A2+5] = B[A0+5] ^ B[A1+5] ^ B[A2+4] ^ B[A3+4] ^ B[A3+5];
    Bp[A2+6] = B[A0+6] ^ B[A1+6] ^ B[A2+5] ^ B[A3+5] ^ B[A3+6];
    Bp[A2+7] = B[A0+7] ^ B[A1+7] ^ B[A2+6] ^ B[A3+6] ^ B[A3+7];
    

    //  A3 = A0 ^ A3
    //  A3 = A3 ^ (0x1b & ((signed char)of>>7));

    //// 3 * A0
    //  A3 = A3 ^ (A0)
    //  A3 = A3 ^ (A0 << 1)

    //// + A1 + A2
    //  A3 = A3 ^ A1
    //  A3 = A3 ^ A2

    //// + 2 * A3
    //  A3 = A3 ^ (A3<<1)

    of = B[A0+7] ^ B[A3+7];

    //        2*A0       A0         A1         A2       2*A3
    Bp[A3+0] = B[A0+0] ^           B[A1+0] ^ B[A2+0]           ^ of;
    Bp[A3+1] = B[A0+1] ^ B[A0+0] ^ B[A1+1] ^ B[A2+1] ^ B[A3+0] ^ of;
    Bp[A3+2] = B[A0+2] ^ B[A0+1] ^ B[A1+2] ^ B[A2+2] ^ B[A3+1];
    Bp[A3+3] = B[A0+3] ^ B[A0+2] ^ B[A1+3] ^ B[A2+3] ^ B[A3+2] ^ of;
    Bp[A3+4] = B[A0+4] ^ B[A0+3] ^ B[A1+4] ^ B[A2+4] ^ B[A3+3] ^ of;
    Bp[A3+5] = B[A0+5] ^ B[A0+4] ^ B[A1+5] ^ B[A2+5] ^ B[A3+4];
    Bp[A3+6] = B[A0+6] ^ B[A0+5] ^ B[A1+6] ^ B[A2+6] ^ B[A3+5];
    Bp[A3+7] = B[A0+7] ^ B[A0+6] ^ B[A1+7] ^ B[A2+7] ^ B[A3+6];
    

    //
    Bp += BLOCK_SIZE/4;
    B  += BLOCK_SIZE/4;
    }


    memmove(B - BLOCK_SIZE,Bp - BLOCK_SIZE,sizeof(Bp_space));
}

void bs_mixcolumns_rev(word_t * B)
{
    // to understand this, see
    // https://en.wikipedia.org/wiki/Rijndael_mix_columns
    // TODO combine with shiftrows for performance on decryption
    word_t Bp_space[BLOCK_SIZE];
    word_t * Bp = Bp_space;

 
    int i = 0;
    for (; i < BLOCK_SIZE / 4; i += BLOCK_SIZE / 16)
    {

    //// state[i][0] = A0*0x0e + A1*0x0b + A2*0x0d + A3*0x09
    // overflow:
                    /* A0 * 0b1110             */   /* A1 * 0b1011     */   /* A2 * 0b1101   */   /* A3 * 0b1001 */
    word_t of0 = (  (B[A0+7] ^ B[A0+6] ^ B[A0+5]) ^ (B[A1 + 7] ^ B[A1+5]) ^ (B[A2+6] ^ B[A2+5]) ^ ( B[A3+5] ));       // 2 bit
    word_t of1 = (            (B[A0+7] ^ B[A0+6]) ^ (            B[A1+6]) ^ (B[A2+7] ^ B[A2+6]) ^ ( B[A3+6] ));       // 3 bit
    word_t of2 = (                      (B[A0+7]) ^ (            B[A1+7]) ^ (          B[A2+7]) ^ ( B[A3+7] ));       // 4 bit

    // inverse:
    //          1110      1011    1101      1001
    // A0 = A0 * 14 + A1 * 11 + A2 * 13 + A3 * 9
    // A0 = A0 * (2+4+8) + A1 * (1+2+8) + A2 * (1+4+8) + A3 * (1+8)

    //         (2*A0   +  4*A0   + 8*A0 )  + (8*A1   +  2*A1   +   A1  ) + ( A2    +   4*A2  +  8*A2 ) + (  A3   + 8*A3)
    Bp[A0+0] =                                                   B[A1+0] ^ B[A2+0]                     ^ B[A3+0]           ^ of0;
    Bp[A0+1] = B[A0+0]                               ^ B[A1+0] ^ B[A1+1] ^ B[A2+1]                     ^ B[A3+1]           ^ of0 ^ of1;
    Bp[A0+2] = B[A0+1] ^ B[A0+0]                     ^ B[A1+1] ^ B[A1+2] ^ B[A2+2] ^ B[A2+0]           ^ B[A3+2]                 ^ of1 ^ of2;
    Bp[A0+3] = B[A0+2] ^ B[A0+1] ^ B[A0+0] ^ B[A1+0] ^ B[A1+2] ^ B[A1+3] ^ B[A2+3] ^ B[A2+1] ^ B[A2+0] ^ B[A3+3] ^ B[A3+0] ^ of0       ^ of2;
    Bp[A0+4] = B[A0+3] ^ B[A0+2] ^ B[A0+1] ^ B[A1+1] ^ B[A1+3] ^ B[A1+4] ^ B[A2+4] ^ B[A2+2] ^ B[A2+1] ^ B[A3+4] ^ B[A3+1] ^ of0 ^ of1;
    Bp[A0+5] = B[A0+4] ^ B[A0+3] ^ B[A0+2] ^ B[A1+2] ^ B[A1+4] ^ B[A1+5] ^ B[A2+5] ^ B[A2+3] ^ B[A2+2] ^ B[A3+5] ^ B[A3+2]       ^ of1 ^ of2;
    Bp[A0+6] = B[A0+5] ^ B[A0+4] ^ B[A0+3] ^ B[A1+3] ^ B[A1+5] ^ B[A1+6] ^ B[A2+6] ^ B[A2+4] ^ B[A2+3] ^ B[A3+6] ^ B[A3+3]             ^ of2;
    Bp[A0+7] = B[A0+6] ^ B[A0+5] ^ B[A0+4] ^ B[A1+4] ^ B[A1+6] ^ B[A1+7] ^ B[A2+7] ^ B[A2+5] ^ B[A2+4] ^ B[A3+7] ^ B[A3+4];



    //// state[i][1] = A0*0x09 + A1*0xe + A2*0x0b + A3*0x0d
    // overflow:
          /* A0 * 0b1001 */   /* A1 * 0b1110             */ /* A2 * 0b101  1     */   /* A3 * 0b1101 */
    of0 = (  (B[A0+5]) ^      (B[A1+7] ^ B[A1+6] ^ B[A1+5]) ^ (B[A2 + 7] ^ B[A2+5]) ^ (B[A3+6] ^ B[A3+5]));       // 2 bit
    of1 = (  (B[A0+6]) ^                (B[A1+7] ^ B[A1+6]) ^ (            B[A2+6]) ^ (B[A3+7] ^ B[A3+6]));       // 3 bit
    of2 = (  (B[A0+7]) ^                          (B[A1+7]) ^ (            B[A2+7]) ^ (          B[A3+7]));       // 4 bit

    // inverse:
    //          1001      1110    1011      1101
    // A1 = A0 * 9 + A1 * 14 + A2 * 11 + A3 * 13
    // A1 = A0 * (1+8) + A1 * (2+4+8) + A2 * (1+2+8) + A3 * (1+4+8)

    //         (1*A0   +  8*A0 ) +(2*A1    +  4*A1   +  8*A1 ) + (1*A2   +  2*A2   +  8*A2 ) + (1*A3   +   4*A3  +  8*A3)
    Bp[A1+0] = B[A0+0] ^                                         B[A2+0] ^                     B[A3+0]                     ^ of0;
    Bp[A1+1] = B[A0+1] ^           B[A1+0]                     ^ B[A2+1] ^ B[A2+0] ^           B[A3+1]                     ^ of0 ^ of1;
    Bp[A1+2] = B[A0+2] ^           B[A1+1] ^ B[A1+0]           ^ B[A2+2] ^ B[A2+1] ^           B[A3+2] ^ B[A3+0]                 ^ of1 ^ of2;
    Bp[A1+3] = B[A0+3] ^ B[A0+0] ^ B[A1+2] ^ B[A1+1] ^ B[A1+0] ^ B[A2+3] ^ B[A2+2] ^ B[A2+0] ^ B[A3+3] ^ B[A3+1] ^ B[A3+0] ^ of0       ^ of2;
    Bp[A1+4] = B[A0+4] ^ B[A0+1] ^ B[A1+3] ^ B[A1+2] ^ B[A1+1] ^ B[A2+4] ^ B[A2+3] ^ B[A2+1] ^ B[A3+4] ^ B[A3+2] ^ B[A3+1] ^ of0 ^ of1;
    Bp[A1+5] = B[A0+5] ^ B[A0+2] ^ B[A1+4] ^ B[A1+3] ^ B[A1+2] ^ B[A2+5] ^ B[A2+4] ^ B[A2+2] ^ B[A3+5] ^ B[A3+3] ^ B[A3+2]       ^ of1 ^ of2;
    Bp[A1+6] = B[A0+6] ^ B[A0+3] ^ B[A1+5] ^ B[A1+4] ^ B[A1+3] ^ B[A2+6] ^ B[A2+5] ^ B[A2+3] ^ B[A3+6] ^ B[A3+4] ^ B[A3+3]             ^ of2;
    Bp[A1+7] = B[A0+7] ^ B[A0+4] ^ B[A1+6] ^ B[A1+5] ^ B[A1+4] ^ B[A2+7] ^ B[A2+6] ^ B[A2+4] ^ B[A3+7] ^ B[A3+5] ^ B[A3+4];


    //// state[i][2] = A0*0x0d + A1*0x09 + A2*0x0e + A3*0x0b
    // overflow:
          /* A1 * 0b1001 */  /* A2 * 0b1110              */ /* A3 * 0b1011      */   /* A0 * 0b1101 */
    of0 = (  (B[A1+5]) ^      (B[A2+7] ^ B[A2+6] ^ B[A2+5]) ^ (B[A3 + 7] ^ B[A3+5]) ^ (B[A0+6] ^ B[A0+5]));       // 2 bit
    of1 = (  (B[A1+6]) ^                (B[A2+7] ^ B[A2+6]) ^ (            B[A3+6]) ^ (B[A0+7] ^ B[A0+6]));       // 3 bit
    of2 = (  (B[A1+7]) ^                          (B[A2+7]) ^ (            B[A3+7]) ^ (          B[A0+7]));       // 4 bit

    // inverse:
    //          1001      1110    1011      1101
    // A2 = A1 * 9 + A2 * 14 + A3 * 11 + A0 * 13
    // A2 = A1 * (1+8) + A2 * (2+4+8) + A3 * (1+2+8) + A0 * (1+4+8)

    //         (1*A1   +  8*A1) + ( 2*A2   +  4*A2   +  8*A2)  + (1*A3      2*A2   +  8*A2) +  (1*A0   +   4*A0  +  8*A0)
    Bp[A2+0] = B[A1+0] ^                                         B[A3+0] ^                     B[A0+0]                     ^ of0;
    Bp[A2+1] = B[A1+1] ^           B[A2+0]                     ^ B[A3+1] ^ B[A3+0] ^           B[A0+1]                     ^ of0 ^ of1;
    Bp[A2+2] = B[A1+2] ^           B[A2+1] ^ B[A2+0]           ^ B[A3+2] ^ B[A3+1] ^           B[A0+2] ^ B[A0+0]                 ^ of1 ^ of2;
    Bp[A2+3] = B[A1+3] ^ B[A1+0] ^ B[A2+2] ^ B[A2+1] ^ B[A2+0] ^ B[A3+3] ^ B[A3+2] ^ B[A3+0] ^ B[A0+3] ^ B[A0+1] ^ B[A0+0] ^ of0       ^ of2;
    Bp[A2+4] = B[A1+4] ^ B[A1+1] ^ B[A2+3] ^ B[A2+2] ^ B[A2+1] ^ B[A3+4] ^ B[A3+3] ^ B[A3+1] ^ B[A0+4] ^ B[A0+2] ^ B[A0+1] ^ of0 ^ of1;
    Bp[A2+5] = B[A1+5] ^ B[A1+2] ^ B[A2+4] ^ B[A2+3] ^ B[A2+2] ^ B[A3+5] ^ B[A3+4] ^ B[A3+2] ^ B[A0+5] ^ B[A0+3] ^ B[A0+2]       ^ of1 ^ of2;
    Bp[A2+6] = B[A1+6] ^ B[A1+3] ^ B[A2+5] ^ B[A2+4] ^ B[A2+3] ^ B[A3+6] ^ B[A3+5] ^ B[A3+3] ^ B[A0+6] ^ B[A0+4] ^ B[A0+3]             ^ of2;
    Bp[A2+7] = B[A1+7] ^ B[A1+4] ^ B[A2+6] ^ B[A2+5] ^ B[A2+4] ^ B[A3+7] ^ B[A3+6] ^ B[A3+4] ^ B[A0+7] ^ B[A0+5] ^ B[A0+4];



    //// state[i][3] = A0*0x0b + A1*0x0d + A2*0x09 + A3*0x0e
    // overflow:
           /* A2 * 0b1001 */  /* A3 * 0b1110             */ /* A0 * 0b1011     */     /* A1 * 0b1101 */
    of0 = (  (B[A2+5]) ^      (B[A3+7] ^ B[A3+6] ^ B[A3+5]) ^ (B[A0 + 7] ^ B[A0+5]) ^ (B[A1+6] ^ B[A1+5]));       // 2 bit
    of1 = (  (B[A2+6]) ^                (B[A3+7] ^ B[A3+6]) ^ (            B[A0+6]) ^ (B[A1+7] ^ B[A1+6]));       // 3 bit
    of2 = (  (B[A2+7]) ^                          (B[A3+7]) ^ (            B[A0+7]) ^ (          B[A1+7]));       // 4 bit

    // inverse:
    //          1001      1110    1011      1101
    // A2 = A2 * 9 + A3 * 14 + A0 * 11 + A1 * 13
    // A2 = A2 * (1+8) + A3 * (2+4+8) + A0 * (1+2+8) + A1 * (1+4+8)

    //         (1*A2   +  8*A2) + ( 2*A3   +  4*A3   +  8*A3)  + (1*A0      2*A0   +  8*A0) +  (1*A1   +   4*A1  +  8*A1)
    Bp[A3+0] = B[A2+0] ^                                         B[A0+0] ^                     B[A1+0]                     ^ of0;
    Bp[A3+1] = B[A2+1] ^           B[A3+0]                     ^ B[A0+1] ^ B[A0+0] ^           B[A1+1]                     ^ of0 ^ of1;
    Bp[A3+2] = B[A2+2] ^           B[A3+1] ^ B[A3+0]           ^ B[A0+2] ^ B[A0+1] ^           B[A1+2] ^ B[A1+0]                 ^ of1 ^ of2;
    Bp[A3+3] = B[A2+3] ^ B[A2+0] ^ B[A3+2] ^ B[A3+1] ^ B[A3+0] ^ B[A0+3] ^ B[A0+2] ^ B[A0+0] ^ B[A1+3] ^ B[A1+1] ^ B[A1+0] ^ of0       ^ of2;
    Bp[A3+4] = B[A2+4] ^ B[A2+1] ^ B[A3+3] ^ B[A3+2] ^ B[A3+1] ^ B[A0+4] ^ B[A0+3] ^ B[A0+1] ^ B[A1+4] ^ B[A1+2] ^ B[A1+1] ^ of0 ^ of1;
    Bp[A3+5] = B[A2+5] ^ B[A2+2] ^ B[A3+4] ^ B[A3+3] ^ B[A3+2] ^ B[A0+5] ^ B[A0+4] ^ B[A0+2] ^ B[A1+5] ^ B[A1+3] ^ B[A1+2]       ^ of1 ^ of2;
    Bp[A3+6] = B[A2+6] ^ B[A2+3] ^ B[A3+5] ^ B[A3+4] ^ B[A3+3] ^ B[A0+6] ^ B[A0+5] ^ B[A0+3] ^ B[A1+6] ^ B[A1+4] ^ B[A1+3]             ^ of2;
    Bp[A3+7] = B[A2+7] ^ B[A2+4] ^ B[A3+6] ^ B[A3+5] ^ B[A3+4] ^ B[A0+7] ^ B[A0+6] ^ B[A0+4] ^ B[A1+7] ^ B[A1+5] ^ B[A1+4];

    Bp += BLOCK_SIZE/4;
    B  += BLOCK_SIZE/4;
    }

    memmove(B - BLOCK_SIZE, Bp - BLOCK_SIZE,sizeof(Bp_space));

}

void bs_expand_key(word_t (* rk)[BLOCK_SIZE], uint8_t * _key)
{
    // TODO integrate this better
    uint8_t key[KEY_SCHEDULE_SIZE];
    memmove(key,_key,BLOCK_SIZE/8);
    expand_key(key);

    int i, j = 0, k;
    for (i = 0; i < KEY_SCHEDULE_SIZE; i += (BLOCK_SIZE/8))
    {
        for (k = 0; k < WORD_SIZE; k += 2)
        {

            memmove(rk[j] + k * WORDS_PER_BLOCK, key + i, BLOCK_SIZE / 8);
            memset(rk[j] + (k+1) * WORDS_PER_BLOCK, 0, BLOCK_SIZE / 8);
        }
        bs_transpose(rk[j]);
        j++;
    }

}

void bs_cipher(word_t state[BLOCK_SIZE], word_t (* rk)[BLOCK_SIZE], uint8_t* maskb)
{
    //add the rng for sand
    //memset(rands,0,256);
    
    for (int i=512; i<1024; i+=2)
    {
        uint8_t tmp1 = *(maskb+i);
        uint8_t tmp2 = *(maskb+i+1);
        uint16_t tmp = ((uint16_t)tmp2 << 8) | tmp1;
        word_t rng = 0;
        for (int j=0; j<16; j++)
        {
            uint8_t a = ((tmp>>j) & 1);
            rng |= a << j*2;
            rng |= a << j*2+1;
        }
        int index = (i-512)/2;
        rands[index] = rng;
    }
    
    //memmove(rands, maskb+512, 1024);
    //for (int i=0; i<256; i++)
    //{   
    //    rands[i] = 0x30003FFE;
    //}
    
    //printf("maskb[1535]=%x \n", maskb[1535]);  
    //printf("rands[255]=%x \n", rands[255]);  
    //dump_word(rands, 256);
    //for (int i=0; i<256; i++) 
    //{   
    //    printf("%x\n",rands[i]);
    //}
    /*for (int i=0; i<256; i++) 
    {   
        word_t n = rands[i];
        while (n) {
            if (n & 1)
                printf("1");
            else
                printf("0");

            n >>= 1;
        }
        printf("\n");
        //printf("%x\n",rands[i]);
    }*/



    int round;
    bs_transpose(state);

    bs_addroundkey(state,rk[0]);

    for (round = 1; round < 10; round++)
    {
        bs_apply_sbox(state);
        
        /*bs_shiftrows(state);*/
        /*bs_mixcolumns(state);*/

        bs_shiftmix(state);
        bs_addroundkey(state,rk[round]);
    }

    bs_apply_sbox(state);
    bs_shiftrows(state);
    bs_addroundkey(state,rk[10]);

    bs_transpose_rev(state);
}

void bs_cipher_rev(word_t state[BLOCK_SIZE], word_t (* rk)[BLOCK_SIZE])
{
    int round;
    bs_transpose(state);


    bs_addroundkey(state,rk[10]);
    for (round = 9; round > 0; round--)
    {
        bs_shiftrows_rev(state);
        bs_apply_sbox_rev(state);
        bs_addroundkey(state,rk[round]);
        bs_mixcolumns_rev(state);
    }
    bs_shiftrows_rev(state);
    bs_apply_sbox_rev(state);
    bs_addroundkey(state,rk[0]);

    bs_transpose_rev(state);
}



