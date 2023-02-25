// 2 Dec 2022
#include "cube.hpp"
#include "configs.hpp"
#include "../../utils/random_utils.hpp"
#include <string>


Cube Cube::random(int size)
{
    Cube cube;
    cube._public_vars.resize(size);
    for (int i = 0; i < size; i++)
	cube._public_vars[i] = random_int(n_public_bits);

    return cube;
}

std::ostream& operator<<(std::ostream& os, const Cube& cube)
{
    os << "[";
    for (int i = 0; i < cube.size(); i++)
    {
	int var_index = cube.public_vars()[i];
	os << (var_index < n_block_bits ? "m" + std::to_string(var_index) : "n" + std::to_string(var_index - n_block_bits));
	if (i < cube.size() - 1)
	    os << ", ";
    }
    os << "] (" << cube.size() << ")";

    return os;
}
