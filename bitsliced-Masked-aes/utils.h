#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>
#include "bs.h"

#ifndef TEST_FOOTPRINT
void dump_hex(uint8_t * h, int len);
void dump_word(word_t * h, int len);
void dump_block(word_t * h, int len);
#else
#define dump_hex(h,len) (0)
#define dump_word(h,len) (0)
#define dump_block(h,len) (0)
#endif

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#endif
