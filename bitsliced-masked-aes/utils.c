
#ifndef TEST_FOOTPRINT
#include <stdio.h>
#include "utils.h"


void dump_hex(uint8_t * h, int len)
{
    while(len--)
        printf("%02hhx",*h++);
    printf("\n");
}

void dump_word(word_t * h, int len)
{
    while(len--)
        if ((len+1) % 8) printf("%" WPAD WFMT "\n",*h++);
        else printf("%d:\n%" WPAD WFMT "\n",128-len-1,*h++);

    printf("\n");
}

void dump_block(word_t * h, int len)
{
    while(len-=2 >= 0)
        printf("%" WPAD WFMT"%" WPAD WFMT  "\n",*h++,*h++);
    printf("\n");
}

#endif
