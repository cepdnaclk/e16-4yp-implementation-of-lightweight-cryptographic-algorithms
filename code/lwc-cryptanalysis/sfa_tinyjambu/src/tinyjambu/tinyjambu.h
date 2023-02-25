#include <stdint.h>

#ifndef TINYJAMBU_H
#define TINYJAMBU_H

void encrypt(uint8_t* cipher_text, uint8_t *tag, uint8_t *message, const uint32_t message_len, uint8_t *key, uint8_t *nonce, uint8_t *ad, uint32_t adlen);

#endif
