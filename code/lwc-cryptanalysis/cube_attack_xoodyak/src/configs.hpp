// 02 Dec 2022
#pragma once

#include <string>

constexpr int n_linearity_tests = 100;
constexpr int n_key_bits = 128;
constexpr int n_nonce_bits = 128;
constexpr int n_block_bits = 128;
constexpr int n_public_bits = n_nonce_bits + n_block_bits;
const int cipher_text_output_bit = 0;
const int tag_output_bit = 0;
const int n_max_cubes = 1;
const int initial_cube_size = 12;
const int max_cube_size = 18;

const std::string save_file = "cube.txt";
