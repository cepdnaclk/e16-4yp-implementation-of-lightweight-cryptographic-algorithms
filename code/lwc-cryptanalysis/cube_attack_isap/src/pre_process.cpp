// 29 Nov 2022
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "configs.hpp"
#include "pre_process.hpp"
#include <forward_list>
#include "../../utils/random_utils.hpp"
#include "../../common/bytes.hpp"
#include "isap/isap.h"

/**
 * Recursively generate all the bit permutation of the byte.size and call func 
 * for each permuation 
 */
void permute_cube(Bytes& permute, const Cube& cube, int next_index, std::function<void(Bytes&)> callback_func);

std::vector<Cube> random_walk()
{
    std::vector<Cube> cubes;
    Cipher_text_cipher_output_finder cipher_text_cipher_output_finder{};
    Tag_cipher_output_finder tag_cipher_output_finder{};
    const Linearity_tester& linearity_tester = BLR_linearity_tester(tag_cipher_output_finder);

    while (cubes.size() < max_cube_size)
    {
	Cube cube = Cube::random(initial_cube_size);
	std::cout << "Random cube selected : " << cube << "------------" << std::endl;
	while (true)
	{
	    std::cout << "Testing linearity of: " << cube << std::endl;
	    Linearity_tester::Linearity_test_result test_result = linearity_tester.is_linear(cube);   
	    std::cout << "Test result: " << test_result << std::endl;

	    if (test_result == Linearity_tester::non_linear)
	    {
		if (cube.size() < max_cube_size)
		{
		    cube.add_random_public_var(n_public_bits);
		    std::cout << "   var added: " << std::endl;
		}
		else 
		    break;
	    }
	    else if (test_result == Linearity_tester::constant)
	    {
		if (cube.size() > 1)
		{
		    cube.remove_random_public_var();
		    std::cout << "   var removed: " << std::endl;
		}
		else
		    break;
	    }
	    else 
		cubes.push_back(cube);
	}
    }
    
    return cubes;
}

Linearity_tester::Linearity_test_result BLR_linearity_tester::is_linear(const Cube& cube) const
{
    Bytes key0{KEY_SIZE};
    uint8_t sum0 = sum_over_cube(cube, key0, *_cipher_output_finder);
    bool is_constant = true;

    for (int i = 0; i < n_linearity_tests; i++)
    {
	Bytes key1 = Bytes::random(KEY_SIZE);
	Bytes key2 = Bytes::random(KEY_SIZE);
	uint8_t sum1 = sum_over_cube(cube, key1, *_cipher_output_finder); 
	uint8_t sum2 = sum_over_cube(cube, key2, *_cipher_output_finder); 
	//std::cout << "(sum0, sum1, sum2): (" << std::to_string(sum0) << ", " << std::to_string(sum1) << ", " << std::to_string(sum2) << ") " << std::endl;
	if (is_constant && !(sum1 == sum0 && sum2 == sum0))
	    is_constant = false;
	if ((sum0 ^ sum1 ^ sum2) != sum_over_cube(cube, key1 ^ key2, *_cipher_output_finder))
	    return Linearity_tester::non_linear;
	//std::cout << i <<  " (key1, key2): " << "(" << key1.to_str() << ", " << key2.to_str() << ")" << std::endl;
    }

    return is_constant ? Linearity_tester::constant : Linearity_tester::linear;
}


uint8_t sum_over_cube(const Cube& cube, const Bytes& key, const Cipher_output_finder& cipher_output_finder)
{
    //std::cout << "sum over: " << cube << std::endl;
    Bytes public_vars{n_public_bits / 8};
    uint8_t sum = 0;
    int ones = 0, zeros = 0; 

    permute_cube(public_vars, cube, 0, [&key, &sum, &ones, &zeros, &cipher_output_finder](Bytes& public_vars) {
	//std::cout << "public vars: " << public_vars.to_str() << std::endl;
	//std::cout << "ct: " << Bytes{cipher_text, BLOCK_SIZE}.to_str() << std::endl; 
	uint8_t cipher_output = cipher_output_finder.get_cipher_output(public_vars, key);
	//std::cout << "_ct:" << Bytes{cipher_text, BLOCK_SIZE}.to_str() << std::endl; 
	if (cipher_output) ones++;
	else zeros++;
	sum ^= cipher_output;
	
	//std::cout << "sum " <<  std::to_string(sum) << " ct:" << Bytes{cipher_text, BLOCK_SIZE}.to_str() << " tag: " << Bytes(tag, TAG_SIZE).to_str() << " pv: " << public_vars.to_str() << std::endl;
    });

    std::cout << "ones: " << ones << ", zeros: " << zeros << " | sum: " << static_cast<int>(sum) << std::endl;
    if (sum == 1)
	std::cout << "sum is one" << std::endl;

    return sum;
}

void permute_cube(Bytes& permute, const Cube& cube, int next_index, std::function<void(Bytes&)> callback_func)
{
    if (next_index == cube.size())
    {
	callback_func(permute);
	//std::cout << "permute: " << permute.to_str() << std::endl;
	return;
    }

    permute.set_bit(cube.public_vars()[next_index], 0);
    permute_cube(permute, cube, next_index + 1, callback_func);
    permute.set_bit(cube.public_vars()[next_index], 1);
    permute_cube(permute, cube, next_index + 1, callback_func);
}

int main()
{
    srand(std::time(nullptr));
    random_walk();

    printf("done!\n");
    return 0;

    std::cout << "done!" << std::endl;
    return 0;
}
