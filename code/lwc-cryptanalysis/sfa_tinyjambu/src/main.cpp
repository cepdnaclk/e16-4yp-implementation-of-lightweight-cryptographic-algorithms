#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include "../../utils/hex_utils.h"
#include "tinyjambu/tinyjambu.h"
#include <vector>
using namespace std;


using Bytes = vector<uint8_t>;
using Bytes_pair = pair<Bytes, Bytes>;

const int n_samples = 100;
const int plain_text_len = 4;

vector<uint8_t> rand_bytes(int size);
vector<Bytes_pair> collect_faulty_ciphertexts(uint8_t key[16]);
uint32_t update_state_reverse(uint32_t state[4], uint8_t key[16], uint8_t round);

double s_distribution[] = {
    0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507,
    0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507, 0.00346507,
    0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764,
    0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764,
    0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764,
    0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764, 0.00370764,
    0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718,
    0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718, 0.00396718,
    0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161,
    0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161, 0.00367161,
    0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864,
    0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864,
    0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864,
    0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864, 0.00392864,
    0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365,
    0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365, 0.00420365,
    0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689,
    0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689, 0.00361689,
    0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009,
    0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009,
    0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009,
    0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009, 0.00387009,
    0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100,
    0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100, 0.00414100,
    0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248,
    0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248, 0.00383248,
    0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077,
    0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077,
    0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077,
    0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077, 0.00410077,
    0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783,
    0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783, 0.00438783
};


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

vector<Bytes> collect_faulty_tags(uint8_t key[16])
{
    vector<Bytes> faulty_tags(n_samples);
    generate(faulty_tags.begin(), faulty_tags.end(), [&]() {
	Bytes plain_text = rand_bytes(plain_text_len);
	Bytes cipher_text(plain_text_len);
	Bytes tag(8);
	encrypt(&cipher_text[0], &tag[0], &plain_text[0], plain_text_len, key, &rand_bytes(12)[0], nullptr, 0);
	return tag;
    });

    return faulty_tags;
}

vector<uint8_t> rand_bytes(int size)
{
    vector<uint8_t> res(size);
    generate(res.begin(), res.end(), []() { return (uint8_t)(rand() * 255 / RAND_MAX); });
    return res;
}

uint8_t s_hat(uint8_t key_byte, Bytes tag)
{
    return key_byte ^ tag[0];
}

int main()
{
    //uint8_t *cipher_text, *plain_text;
    //uint8_t tag[8];
    /* Test vector 451 */
    srand(time(nullptr));
    char message_hex[] = "00";
    char ad_hex[] = "";

    char key_hex[] = "000102030405060708090A0B0C0D0E0F";
    char nonce_hex[] = "000102030405060708090A0B";
    
    uint8_t* message = hex_to_bytes(message_hex, sizeof(message_hex));
    uint32_t message_len = sizeof(message_hex) / 2;
    uint8_t* ad = hex_to_bytes(ad_hex, sizeof(ad_hex));
    uint32_t adlen = sizeof(ad_hex) / 2;
    uint8_t* key = hex_to_bytes(key_hex, sizeof(key_hex));
    uint8_t *nonce = hex_to_bytes(nonce_hex, sizeof(nonce_hex));
    //cipher_text = new uint8_t[message_len];

    //encrypt(cipher_text, tag, message, message_len, key, nonce, ad, adlen);
    vector<Bytes> faulty_tags = collect_faulty_tags(key);

    for (Bytes tag : faulty_tags)
	cout << "0x" << bytes_to_hex(&tag[0], 1) << ", ";
    //for (int i = 0; i < 256; i++)
    //{
    //    uint8_t key_byte = i;
    //    double likelihood = 1.f;
    //    for (auto tag : faulty_tags)
    //	{
    //        //if (s_distribution[s_hat(key_byte, tag)] > 0.00011)
    //        {
    //    	likelihood *=  s_distribution[s_hat(key_byte, tag)];
    //    	//printf("s hat: %02X, prob: %f\n", s_hat(key_byte, tag), s_distribution[s_hat(key_byte, tag)]);
    //    	//printf("%02X %.25lf\n", key_byte, likelihood);
    //        }
    //	}
    //    //printf("%02X %.35lf\n", key_byte, likelihood * 10e100);
    //    //cout << bytes_to_hex(&faulty_tags[i][0], 8) << endl;
    //}

    cout << "done!" << endl;
    return 0;
}
