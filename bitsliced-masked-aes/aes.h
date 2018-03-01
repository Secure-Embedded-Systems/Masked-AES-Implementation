#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

// outputb should have enough space as inputb.
// maskb should be same length as inputb too.

void aesm_ecb_encrypt(uint8_t * outputb, uint8_t * inputb, uint8_t * maskb, size_t size, uint8_t * key);
//void aes_ecb_decrypt(uint8_t * outputb, uint8_t * inputb, size_t size, uint8_t * key);

//void aesm_ctr_encrypt(uint8_t * outputb, uint8_t * inputb, uint8_t * maskb, size_t size, uint8_t * key, uint8_t * iv);
#define aes_ctr_decrypt(outputb,inputb,m,size,key,iv) aesm_ctr_encrypt(outputb,inputb,m,size,key,iv)

#endif
