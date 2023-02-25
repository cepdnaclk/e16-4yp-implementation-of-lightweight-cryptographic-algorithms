/* ISAP-A-128 */

#include "isap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../ascon/ascon_permutation.h"
#include "../../../utils/hex_utils.h"
#include <stdio.h>


typedef struct
{
    uint8_t *key;
    uint8_t *nonce;
    uint8_t *message;
    uint32_t message_len;
    uint8_t *associated_data;
    uint32_t adlen;
} Isap_data;

const uint8_t iv_ke[] = {0x03, 128, 64, 1, 12, 12, 12, 12};
const uint8_t iv_ka[] = {0x02, 128, 64, 1, 12, 12, 12, 12};
const uint8_t iv_a[] = {0x01, 128, 64, 1, 12, 12, 12, 12};

void init_data(Isap_data *data, uint8_t *message, uint32_t message_len, uint8_t *associated_data, uint32_t adlen, uint8_t *key, uint8_t *nonce);
void isap_enc(uint8_t *cipher_text, Isap_data *data);
void isap_rk_enc(uint8_t *output, uint8_t *key, uint8_t *input);
void isap_mac(uint8_t *tag, Isap_data *data, uint8_t *ciper_text);
void init_state_mac(uint64_t *state, Isap_data *data);
void absorb_associated_data(uint64_t *state, Isap_data *data);
void absorb_cipher_text(uint64_t *state, Isap_data *data, uint8_t *cipher_text);
void squeeze_tag(uint8_t *tag, uint64_t *state, Isap_data *data);
void isap_rk_mac(uint8_t *output, uint8_t *key, uint8_t *input);
void isap_rk(uint8_t *output, uint8_t *key, uint8_t *input, uint8_t outlen, uint8_t *iv);
void get_block_padded(uint8_t *output, uint8_t *data, uint32_t data_len, uint32_t index);
void permute(uint64_t *state);
void load_state_reversed(uint64_t *output, uint64_t *state);
void load_reversed_64(uint8_t *output, uint8_t *input);
void xor_block(uint8_t *output, uint8_t *left, uint8_t *right, uint8_t count);
uint8_t verify_tag(uint8_t *tag, uint8_t *tag_new);


void encrypt(uint8_t *cipher_text, uint8_t *tag, uint8_t *plain_text, uint32_t plain_text_len, uint8_t *key, uint8_t *associated_data, uint32_t adlen, uint8_t *nonce)
{
    Isap_data data;
    init_data(&data, plain_text, plain_text_len, associated_data, adlen, key, nonce);
    isap_enc(cipher_text, &data);
    isap_mac(tag, &data, cipher_text);
}

uint8_t decrypt(uint8_t *plain_text, uint8_t *tag, uint8_t *cipher_text, uint32_t cipher_text_len, uint8_t *key, uint8_t *associated_data, uint32_t adlen, uint8_t *nonce)
{
    Isap_data data;
    uint8_t tag_new[KEY_SIZE];
    init_data(&data, cipher_text, cipher_text_len, associated_data, adlen, key, nonce);
    isap_mac(tag_new, &data, cipher_text);
    if (!verify_tag(tag, tag_new))
	return 0;
    isap_enc(plain_text, &data);
    return 1;
}

void init_data(Isap_data *data, uint8_t *message, uint32_t message_len, uint8_t *associated_data, uint32_t adlen, uint8_t *key, uint8_t *nonce)
{
    data->message = message;
    data->message_len = message_len;
    data->associated_data = associated_data;
    data->adlen = adlen;
    data->key = key;
    data->nonce = nonce;
}

void isap_enc(uint8_t *cipher_text, Isap_data *data)
{
    uint64_t state[5];
    uint32_t i, n_message_blocks;
    uint8_t offset;
    isap_rk_enc((uint8_t*)state, data->key, data->nonce);
    memcpy((uint8_t*)state + STATE_SIZE - NONCE_SIZE, data->nonce, NONCE_SIZE);

    n_message_blocks = data->message_len / BLOCK_SIZE + (data->message_len % BLOCK_SIZE ? 1 : 0);

    for (i = 0; i < n_message_blocks - 1; i++)
    {
	permute(state);
	*((uint64_t*)cipher_text + i) = *((uint64_t*)data->message + i)^ state[0];
    }
    
    offset = (n_message_blocks - 1) * BLOCK_SIZE;
    permute(state);
    xor_block(cipher_text + offset, data->message + offset, (uint8_t*)state, data->message_len - offset); /* TODO: Optimize % */
}

void isap_rk_enc(uint8_t *output, uint8_t *key, uint8_t *input)
{
    isap_rk(output, key, input, STATE_SIZE - NONCE_SIZE, (uint8_t*)iv_ke);
}

void isap_mac(uint8_t *tag, Isap_data *data, uint8_t *ciper_text)
{
    uint64_t state[5];
    init_state_mac(state, data);
    absorb_associated_data(state, data);
    absorb_cipher_text(state, data, ciper_text);
    squeeze_tag(tag, state, data);
}

void init_state_mac(uint64_t *state, Isap_data *data)
{
    memcpy(state, data->nonce, NONCE_SIZE);
    memcpy((uint8_t*)state + NONCE_SIZE, iv_a, IV_SIZE);
    state[3] = 0;
    state[4] = 0;
    permute(state);
}

void absorb_associated_data(uint64_t *state, Isap_data *data)
{
    uint32_t i, n_ad_blocks;
    uint64_t adblock;
    n_ad_blocks = data->adlen / BLOCK_SIZE + 1;
    for (i = 0; i < n_ad_blocks; i++)
    {
	get_block_padded((uint8_t*)&adblock, data->associated_data, data->adlen, i);
	state[0] ^= adblock;
	permute(state);
    }
    ((uint8_t*)state)[STATE_SIZE - 1] ^= 0x01;
}

void absorb_cipher_text(uint64_t *state, Isap_data *data, uint8_t *cipher_text)
{
    uint32_t i, n_c_blocks;
    uint64_t cblock;
    n_c_blocks = data->message_len / BLOCK_SIZE + 1;
    for (i = 0; i < n_c_blocks; i++)
    {
	get_block_padded((uint8_t*)&cblock, cipher_text, data->message_len, i);
	state[0] ^= cblock;
	permute(state);
    }
}

/**
 * Returns a index th block of data. Pladded with 1 || 0 *. For plain text, ad or 
 * cipher text.
 */
void get_block_padded(uint8_t *output, uint8_t *data, uint32_t data_len, uint32_t index)
{
    uint32_t offset, count;
    offset = index * BLOCK_SIZE;
    count = ((data_len - offset) < BLOCK_SIZE) ? (data_len - offset) : BLOCK_SIZE;
    memcpy(output, data + offset, count);
    if (count < BLOCK_SIZE)
    {
	output[count] = 0x80;
	memset(output + count + 1, 0, BLOCK_SIZE - count - 1);
    }
}

void squeeze_tag(uint8_t *tag, uint64_t *state, Isap_data *data)
{
    isap_rk_mac((uint8_t*)state, data->key, (uint8_t*)state);
    permute(state);
    memcpy(tag, state, KEY_SIZE);
}

void isap_rk_mac(uint8_t *output, uint8_t *key, uint8_t *input)
{
    isap_rk(output, key, input, NONCE_SIZE, (uint8_t*)iv_ka);
}

void isap_rk(uint8_t *output, uint8_t *key, uint8_t *input, uint8_t outlen, uint8_t *iv)
{
    uint64_t state[5];
    uint8_t i, j, current_bit;
    memcpy(state, key, KEY_SIZE);
    memcpy(state + 2, iv, IV_SIZE);
    state[3] = 0; 
    state[4] = 0;
    permute(state);

    for (i = 0; i < NONCE_SIZE; i++)
    {
	for (j = 0; j < 8; j++)
	{
	    current_bit = (input[i] << j) & 0x80; 
	    *(uint8_t*)state ^= current_bit;
	    permute(state);

	}
    }

    memcpy(output, state, outlen);
}


void permute(uint64_t *state)
{
    uint64_t state_reversed[5];
    load_state_reversed(state_reversed, state);
    permute_a(state_reversed);
    load_state_reversed(state, state_reversed);
}

void load_state_reversed(uint64_t *output, uint64_t *state)
{
    load_reversed_64((uint8_t*)output, (uint8_t*)state);
    load_reversed_64((uint8_t*)(output + 1), (uint8_t*)(state + 1));
    load_reversed_64((uint8_t*)(output + 2), (uint8_t*)(state + 2));
    load_reversed_64((uint8_t*)(output + 3), (uint8_t*)(state + 3));
    load_reversed_64((uint8_t*)(output + 4), (uint8_t*)(state + 4));
}

void load_reversed_64(uint8_t *output, uint8_t *input)
{
    output[0] = input[7];
    output[1] = input[6];
    output[2] = input[5];
    output[3] = input[4];
    output[4] = input[3];
    output[5] = input[2];
    output[6] = input[1];
    output[7] = input[0];
}

void xor_block(uint8_t *output, uint8_t *left, uint8_t *right, uint8_t count)
{
    uint8_t i;
    for (i = 0; i < count; i++)
	output[i] = left[i] ^ right[i];
}

uint8_t verify_tag(uint8_t *tag, uint8_t *tag_new)
{
    uint8_t tag_match, i;
    tag_match = 0;
    for (i = 0; i < KEY_SIZE; i++)
	tag_match |= tag[i] ^ tag_new[i];
    return !tag_match;
}
