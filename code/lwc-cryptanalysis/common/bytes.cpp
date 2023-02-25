#include "bytes.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>


Bytes::Bytes() : _data{nullptr}, _size{0}
{
}

Bytes::Bytes(int init_size) : _data{new uint8_t[init_size]}, _size{init_size}
{
}

Bytes::Bytes(const std::string& byte_str) : Bytes(static_cast<int>(byte_str.length()) / 2)
{
    for (int i = 0; i < _size; i++)
    {
	char hex_byte[3] = "00";
	hex_byte[0] = byte_str[2 * i];
	hex_byte[1] = byte_str[2 * i + 1];
	_data[i] = static_cast<uint8_t>(strtol(hex_byte, nullptr, 16));
    }
}

Bytes::Bytes(Bytes&& other) : _data{other._data}, _size{other._size}
{
    other._data = nullptr;
    other._size = 0;
}

Bytes::Bytes(const Bytes& other) : _data{new uint8_t[other._size]}, _size{other._size}
{
    memcpy(_data, other._data, _size);
}

Bytes::Bytes(const uint8_t* buffer, int size) : _data{new uint8_t[size]}, _size{size}
{
    memcpy(_data, buffer, _size);
}

Bytes::~Bytes()
{
    clear();
}

int Bytes::size() const
{
    return _size;
}

uint8_t* Bytes::data() const
{
    return _data;
}

std::string Bytes::to_str() const
{
    char byte_str[_size * 2];
    for (int i = 0; i < _size; i++)
	sprintf(byte_str + 2 * i, "%02X", _data[i]);
    return std::string(byte_str, _size *2);
}

std::string Bytes::to_bit_str() const
{
    char bit_str[_size * 9 - 1];

    for (int i = 0, j = 0; i < _size * 8; i++)
    {
	if (i % 8 == 0 && i > 0)
	    bit_str[j++] = ' ';
	bit_str[j++] = bit(i) ? '1' : '0';
    }

    return std::string(bit_str, _size * 9 - 1);
}

Bytes Bytes::reverse() const
{
    Bytes reversed(_size);
    for (int i = 0; i < _size; i++)
	reversed._data[_size - i - 1] = _data[i];
    return reversed;
}

uint8_t Bytes::bit(uint32_t index) const
{
    uint32_t byte_index = index / 8;
    uint32_t offset = index & 7;
    return (_data[byte_index] >> (7 - offset)) & 0x01;
}

Bytes& Bytes::set_bit(uint32_t index, uint8_t bit)
{
    int byte_index = index / 8;
    int offset = 7 - index & 7;

    if (bit == 0)
	_data[byte_index] &= ~(1 << offset); 
    else
	_data[byte_index] |=  (1 << offset); 

    return *this;
}

Bytes Bytes::sub(int offset, int count) const
{
    Bytes sub_bytes(count);
    for (int i = 0; i < count; i++)
	sub_bytes._data[i] = _data[offset + i];
    return sub_bytes;
}

Bytes Bytes::random(int size)
{
    Bytes bytes(size);
    for (int i = 0; i < size; i++)
	bytes._data[i] = static_cast<uint8_t>(static_cast<float>(rand()) * 256.f / float{RAND_MAX});
    return bytes;
}

bool Bytes::operator==(const Bytes& right)
{
    if (_size != right.size()) return false;
    for (int i = 0; i < _size; i++)
	if (_data[i] != right._data[i])
	    return false;
    return true;
}

Bytes& Bytes::operator=(const Bytes& right)
{
    clear();
    _size = right._size;
    _data = new uint8_t[_size];
    memcpy(_data, right._data, _size);
    return *this;
}

Bytes Bytes::operator^(const Bytes& other)
{
    Bytes result{_size};

    for (int i = 0; i < _size; i++)
	result.data()[i] = _data[i] ^ other._data[i];

    return result;
}

void Bytes::clear()
{
    if (_data != nullptr)
	delete[] _data;
}
