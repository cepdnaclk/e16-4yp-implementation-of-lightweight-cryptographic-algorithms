#pragma once

#include <stdint.h>

#define KEY_SIZE 16
#define NONCE_SIZE 16 
#define TAG_SIZE 16
#define BLOCK_SIZE 44 


void encrypt(uint8_t *cipher_text, uint8_t *tag, uint8_t *plain_text, uint32_t plain_text_len, uint8_t *key, uint8_t *associated_data, uint32_t adlen, uint8_t *nonce);
