#ifndef ISAP_H
#define ISAP_H

#include <stdint.h>

#define N_ROUNDS 2

#define KEY_SIZE 16
#define STATE_SIZE 40
#define BLOCK_SIZE 8
#define NONCE_SIZE 16
#define ROUNDS_H N_ROUNDS 
#define ROUNDS_B N_ROUNDS 
#define ROUNDS_E N_ROUNDS
#define ROUNDS_K N_ROUNDS 
#define IV_SIZE 8
#define TAG_SIZE 16

void encrypt(uint8_t *cipher_text, uint8_t *tag, uint8_t *plain_text, uint32_t plain_text_len, uint8_t *key, uint8_t *associated_data, uint32_t adlen, uint8_t *nonce);

#endif
