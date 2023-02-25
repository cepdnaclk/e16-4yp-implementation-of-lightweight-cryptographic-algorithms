// 29 Nov 2022
#pragma once

#include <vector>
#include <functional>
#include "../../common/bytes.hpp"
#include "cube.hpp"
#include "configs.hpp"
#include "isap/isap.h"



class Cipher_output_finder 
{
public:
    virtual uint8_t get_cipher_output(const Bytes& public_vars, const Bytes& key) const = 0;
};

class Linearity_tester
{
public:
    enum Linearity_test_result
    {
        linear, 
        constant,
        non_linear
    };

    virtual Linearity_test_result is_linear(const Cube& cube) const = 0;
};

class BLR_linearity_tester : public Linearity_tester
{
public:
    BLR_linearity_tester(Cipher_output_finder& cipher_output_finder) : _cipher_output_finder{&cipher_output_finder} { }
    Linearity_test_result is_linear(const Cube& cube) const override;

private:
    Cipher_output_finder* _cipher_output_finder;
};

inline std::ostream& operator<<(std::ostream& os, Linearity_tester::Linearity_test_result result)
{
    switch (result)
    {
	case Linearity_tester::Linearity_test_result::constant:
	    os << "constant";
	    break;
	case Linearity_tester::Linearity_test_result::linear:
	    os << "linear";
	    break;
	case Linearity_tester::Linearity_test_result::non_linear:
	    os << "non linear";
	    break;
    }
    return os;
}


class Cipher_text_cipher_output_finder : public Cipher_output_finder
{
public:
    uint8_t get_cipher_output(const Bytes& public_vars, const Bytes& key) const override 
    {
	uint8_t cipher_text[BLOCK_SIZE], tag[TAG_SIZE];
	encrypt(cipher_text, tag, public_vars.data(), BLOCK_SIZE, key.data(), public_vars.data(), BLOCK_SIZE, public_vars.data() + BLOCK_SIZE);

	return cipher_text[cipher_text_output_bit >> 3] >> (7 - cipher_text_output_bit);
    }
};

class Tag_cipher_output_finder : public Cipher_output_finder
{
public:
    uint8_t get_cipher_output(const Bytes& public_vars, const Bytes& key) const override 
    {
	uint8_t cipher_text[BLOCK_SIZE], tag[TAG_SIZE];
	encrypt(cipher_text, tag, public_vars.data(), BLOCK_SIZE, key.data(), public_vars.data(), BLOCK_SIZE, public_vars.data() + BLOCK_SIZE);
	return tag[tag_output_bit >> 3] >> (7 - tag_output_bit & 7);
    }
};


uint8_t sum_over_cube(const Cube& cube, const Bytes& key, const Cipher_output_finder& cipher_output_finder);
