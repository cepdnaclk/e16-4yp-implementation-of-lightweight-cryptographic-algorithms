// 2 Dec 2022
#pragma once

#include <cstdlib>


/**
 * Genrate a random int in range [start, end)
 */
inline int random_int(int start, int end)
{
    return (rand() / RAND_MAX) * (end - start) + start;
}

/**
 * Genrate a random int in range [0, end)
 */
inline int random_int(int end)
{
    return rand() * end / RAND_MAX;
}
