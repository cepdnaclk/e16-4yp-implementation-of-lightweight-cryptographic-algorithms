#include <iostream>
#include <cstring>
#include <cstdint>
#include "../../utils/hex_utils.h"
using namespace std;

/* Returns the first 32 bits of the state of the previouse round */
uint32_t update_state_reverse(uint32_t state[4], uint8_t key[16], uint8_t round)
{
    uint32_t feedback = state[3];
    state[3] = state[2];
    state[2] = state[1];
    state[1] = state[0];
    uint32_t s47 = (state[1] >> 15) | (state[2] << 17);
    uint32_t s70 = (state[2] >> 6) | (state[3] << 26);
    uint32_t s85 = (state[2] >> 21) | (state[3] << 11);
    uint32_t s91 = (state[2] >> 27) | (state[3] << 5);   

    state[0] = feedback ^ s47 ^ (~(s70 & s85)) ^ s91 ^ ((uint32_t*)key)[round & 3];
    return state[0];
}

void update_state_single(uint32_t *state, uint8_t* key, uint32_t count)
{
    uint32_t feedback, s0, s47, s70, s85, s91;
    s0 = state[0];
    s47 = (state[1] >> 15) | (state[2] << 17);
    s70 = (state[2] >> 6) | (state[3] << 26);
    s85 = (state[2] >> 21) | (state[3] << 11);
    s91 = (state[2] >> 27) | (state[3] << 5);

    feedback = s0 ^ s47 ^ (~(s70 & s85)) ^ s91 ^ ((uint32_t*)key)[count & 3];
    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];
    state[3] = feedback;
}

int main()
{
    char state_hex[] = "BB28A2FF7EAE50BB6388414FBC858417";
    char key_hex[] = "000102030405060708090A0B0C0D0E0F";
    uint32_t* state = (uint32_t*)hex_to_bytes(state_hex, 32);
    uint8_t* key = hex_to_bytes(key_hex, 32);

    update_state_single(state, key, 18);
    update_state_single(state, key, 19);
    update_state_reverse(state, key, 19);
    uint32_t s0 = update_state_reverse(state, key, 18);
    cout << bytes_to_hex((uint8_t*)&s0, 4) << endl;

    return 0;
}
