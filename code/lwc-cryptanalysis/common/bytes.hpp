#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>


class Bytes
{
public:
    Bytes();
    Bytes(int init_size);
    Bytes(const std::string& byte_str);
    Bytes(Bytes&& other);
    Bytes(const Bytes& other);
    Bytes(const uint8_t* buffer, int size);
    ~Bytes();
    static Bytes random(int size);

    int size() const;
    uint8_t* data() const;
    std::string to_str() const;
    std::string to_bit_str() const;
    Bytes reverse() const;
    uint8_t bit(uint32_t index) const;
    Bytes& set_bit(uint32_t index, uint8_t bit);
    Bytes sub(int offset, int count) const;

    bool operator==(const Bytes& right);
    Bytes& operator=(const Bytes& right);

    Bytes operator^(const Bytes& other);

private:
    void clear();
    inline uint32_t bit_index_to_byte_index(int bit_index) const;

    int _size;
    uint8_t* _data;
};

inline uint32_t Bytes::bit_index_to_byte_index(int bit_index) const
{
    uint32_t byte_index = bit_index / 8;
    if (byte_index >= _size)
	throw std::out_of_range{"Bit index " + 
	    std::to_string(bit_index) + " is out of range for " + std::to_string(_size) + " bytes"};
    return byte_index;
}
