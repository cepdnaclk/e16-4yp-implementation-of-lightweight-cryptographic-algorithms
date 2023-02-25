// 2 Dec 2022
#pragma once

#include <vector>
#include "../../utils/random_utils.hpp"
#include <ostream>


class Cube
{
public:
    static Cube random(int size);

    inline void add_random_public_var(int range);
    inline void remove_random_public_var();
    inline const std::vector<int>& public_vars() const;
    inline int size() const;


private:
    std::vector<int> _public_vars;
};

inline const std::vector<int>& Cube::public_vars() const
{
    return _public_vars;
}

inline int Cube::size() const
{
    return _public_vars.size();
}

inline void Cube::add_random_public_var(int range)
{
    _public_vars.push_back(random_int(range));
}

inline void Cube::remove_random_public_var()
{
    _public_vars.erase(_public_vars.begin() + random_int(_public_vars.size()));
}

std::ostream& operator<<(std::ostream& os, const Cube& cube);
